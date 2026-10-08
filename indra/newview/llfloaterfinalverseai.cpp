// Copyright (c) 2026 Finalverse contributors. SPDX-License-Identifier: LGPL-2.1-only
#include "llviewerprecompiledheaders.h"
#include "llfloaterfinalverseai.h"
#include "llagent.h"
#include "llviewerregion.h"
#include "llselectmgr.h"
#include "llviewerobject.h"
#include "lllineeditor.h"
#include "lltexteditor.h"
#include "llbutton.h"
#include "llcheckboxctrl.h"
#include "llcoros.h"
#include "llcorehttputil.h"
#include <iomanip>
#include <sstream>

namespace
{
LLSD post(const std::string& url, const LLSD& body)
{
    LLCoreHttpUtil::HttpCoroutineAdapter adapter("FinalverseWorld", LLCore::HttpRequest::DEFAULT_POLICY_ID);
    auto request = std::make_shared<LLCore::HttpRequest>();
    auto options = std::make_shared<LLCore::HttpOptions>();
    options->setTimeout(110);
    LLSD result = adapter.postJsonAndSuspend(request, url, body, options);
    auto status = LLCoreHttpUtil::HttpCoroutineAdapter::getStatusFromLLSD(result[LLCoreHttpUtil::HttpCoroutineAdapter::HTTP_RESULTS]);
    if (!status && !result.has("error")) result["error"] = "The service is unavailable. No result was confirmed; inspect history before retrying.";
    result.erase(LLCoreHttpUtil::HttpCoroutineAdapter::HTTP_RESULTS);
    return result;
}
LLSD modelContext(const LLSD& world)
{
    LLSD result;
    for (const char* key : {"region", "avatar", "terrain"}) result[key] = world[key];
    auto entity = [](const LLSD& source)
    {
        LLSD value;
        if (!source.isMap()) return value;
        for (const char* key : {"id", "name", "shape", "position", "scale", "editable"}) value[key] = source[key];
        return value;
    };
    result["selected_object"] = entity(world["selected_object"]);
    result["nearby_entities"] = LLSD::emptyArray();
    for (const auto& item : llsd::inArray(world["nearby_entities"]))
    {
        LLSD next; next["entity"] = entity(item["entity"]); next["distance"] = item["distance"];
        result["nearby_entities"].append(next);
    }
    return result;
}
void describe(std::ostringstream& text, const LLSD& value)
{
    text << "  ID: " << value["id"].asString() << "\n";
    for (const char* key : {"position", "scale", "rotation", "color"})
    {
        text << "  " << key << ": ";
        bool first = true;
        for (const auto& number : llsd::inArray(value[key]))
        {
            if (!first) text << ", ";
            text << std::fixed << std::setprecision(3) << number.asReal();
            first = false;
        }
        text << "\n";
    }
    text << "  Shape: " << value["shape"].asString() << "\n  Description: " << value["description"].asString() << "\n";
}
}

bool LLFloaterFinalverseAI::postBuild()
{
    getChild<LLButton>("ask")->setCommitCallback([this](LLUICtrl*, const LLSD&) { run(getChild<LLCheckBoxCtrl>("citizen_mode")->get() ? "citizen_say" : "plan"); });
    getChild<LLButton>("inspect")->setCommitCallback([this](LLUICtrl*, const LLSD&) { run("inspect"); });
    getChild<LLButton>("build")->setCommitCallback([this](LLUICtrl*, const LLSD&) { run("commit"); });
    getChild<LLButton>("cancel")->setCommitCallback([this](LLUICtrl*, const LLSD&) { cancel(); });
    getChild<LLButton>("undo")->setCommitCallback([this](LLUICtrl*, const LLSD&) { run("undo"); });
    getChild<LLButton>("history")->setCommitCallback([this](LLUICtrl*, const LLSD&) { run("journal"); });
    getChild<LLLineEditor>("request")->setCommitCallback([this](LLUICtrl*, const LLSD&) { run(getChild<LLCheckBoxCtrl>("citizen_mode")->get() ? "citizen_say" : "plan"); });
    for (const auto& binding : {std::pair<const char*, const char*>{"citizen_inspect", "inspect"}, {"citizen_pause", "pause"}, {"citizen_resume", "resume"}, {"citizen_cancel", "cancel"}, {"citizen_approve", "approve"}, {"citizen_undo", "undo"}})
    {
        const std::string operation = std::string("citizen_") + binding.second;
        getChild<LLButton>(binding.first)->setCommitCallback([this, operation](LLUICtrl*, const LLSD&) { run(operation); });
    }
    getChild<LLCheckBoxCtrl>("citizen_mode")->setCommitCallback([this](LLUICtrl*, const LLSD&) {
        cancel();
        const bool citizen = getChild<LLCheckBoxCtrl>("citizen_mode")->get();
        getChild<LLButton>("ask")->setLabel(citizen ? "Send" : "Plan");
        show(citizen ? "Talk to Lumi. Ask who she is, what she remembers, or say 'Lumi, go home.' Garden creation needs separate approval." : "Ask about the world or propose an approved world change.");
    });
    show("Ask about the world, select an owned object to edit, or stand on clear ground to create Lumi's home. Every change needs your approval.");
    busy(false);
    return true;
}
void LLFloaterFinalverseAI::show(const std::string& text) { getChild<LLTextEditor>("activity")->setText(text); }
void LLFloaterFinalverseAI::busy(bool active, bool mutation)
{
    mBusy = active; mMutation = active && mutation;
    for (const char* name : {"ask", "inspect", "history", "citizen_inspect", "citizen_pause", "citizen_resume", "citizen_cancel", "citizen_approve", "citizen_undo"}) getChild<LLButton>(name)->setEnabled(!active);
    // Human overrides can interrupt reasoning without waiting for the model.
    for (const char* name : {"citizen_pause", "citizen_cancel"}) getChild<LLButton>(name)->setEnabled(!mMutation);
    getChild<LLButton>("build")->setEnabled(!active && mPrepared.has("plan_id"));
    getChild<LLButton>("undo")->setEnabled(!active && !mLastPlan.empty());
    getChild<LLButton>("cancel")->setEnabled(!mMutation);
    getChild<LLLineEditor>("request")->setEnabled(!active);
    getChild<LLCheckBoxCtrl>("citizen_mode")->setEnabled(!active);
}
void LLFloaterFinalverseAI::cancel()
{
    if (mMutation) return;
    ++mGeneration; mPrepared = LLSD(); busy(false);
    show("Proposal canceled. No world changes were requested.");
}
void LLFloaterFinalverseAI::run(const std::string& operation)
{
    const bool override = operation == "citizen_pause" || operation == "citizen_cancel";
    if (mBusy && (!override || mMutation)) return;
    // A proposal is actionable only while its own preview is displayed.
    if (operation != "commit") { mPrepared = LLSD(); busy(false); }
    auto region = gAgent.getRegion();
    if (!region) { show("Enter MutSea Harbor first."); return; }
    const std::string capability = region->getCapability("FinalverseWorld");
    if (capability.empty()) { show("This region has not enabled Finalverse world actions."); return; }
    const LLUUID region_id = region->getRegionID();
    const std::string prompt = getChild<LLLineEditor>("request")->getText();
    if (operation == "plan" && (prompt.empty() || prompt.size() > 2048)) { show("Enter a request of up to 2048 characters."); return; }
    const bool citizen = operation.compare(0, 8, "citizen_") == 0;
    if (citizen && operation == "citizen_say" && (prompt.empty() || prompt.size() > 2048)) { show("Enter a message for Lumi of up to 2048 characters."); return; }
    LLSD body; body["op"] = citizen ? "citizen" : operation == "plan" ? "inspect" : operation;
    if (citizen) { body["command"] = operation.substr(8); body["text"] = prompt; }
    if (auto object = LLSelectMgr::getInstance()->getSelection()->getFirstRootObject()) body["selected_id"] = object->getID().asString();
    if (operation == "commit") body["plan_id"] = mPrepared["plan_id"];
    if (operation == "undo") body["plan_id"] = mLastPlan;
    const U32 generation = ++mGeneration;
    const LLHandle<LLFloater> handle = getHandle();
    busy(true, operation == "commit" || operation == "undo" || operation == "citizen_approve" || operation == "citizen_undo");
    show(operation == "plan" ? "Reading the world and planning…" : operation == "citizen_say" ? "Lumi is considering your message… You can pause or cancel while she thinks." : "Waiting for the world service…");
    LLCoros::instance().launch("FinalverseWorld", [handle, generation, region_id, capability, operation, body, prompt]()
    {
        auto current = [&]() -> LLFloaterFinalverseAI*
        {
            auto self = dynamic_cast<LLFloaterFinalverseAI*>(handle.get());
            if (!self || self->mGeneration != generation) return nullptr;
            if (!gAgent.getRegion() || gAgent.getRegion()->getRegionID() != region_id)
            { self->mPrepared = LLSD(); self->mLastPlan.clear(); self->busy(false); self->show("Your region changed. Please inspect and plan again."); return nullptr; }
            return self;
        };
        LLSD reply = post(capability, body);
        auto self = current(); if (!self) return;
        if (reply.has("error")) { self->mPrepared = LLSD(); self->busy(false); self->show(reply["error"].asString()); return; }
        if (operation.compare(0, 8, "citizen_") == 0)
        {
            const LLSD result = reply["result"];
            std::ostringstream display;
            display << "Lumi: " << result["answer"].asString() << "\n\n";
            if (operation == "citizen_inspect")
            {
                const LLSD identity = result["citizen"]["identity"];
                display << "Status: " << identity["lifecycle"].asString() << " · Autonomy " << identity["autonomy_level"].asInteger() << "\n";
                display << "Home: " << identity["home_entity_id"].asString() << "\n";
                display << "Capabilities: ";
                for (const auto& capability : llsd::inArray(identity["capabilities"])) display << capability.asString() << " ";
                display << "\n";
                const LLSD goals = result["citizen"]["goals"];
                if (goals.size())
                {
                    const LLSD goal = goals[goals.size() - 1];
                    display << "Goal: " << goal["description"].asString() << " · " << goal["status"].asString() << "\n";
                    display << "Intention: " << goal["intention"].asString() << "\n";
                    if (!goal["reason"].asString().empty()) display << "Reason: " << goal["reason"].asString() << "\n";
                }
            }
            if (result["preview"].isMap())
            {
                display << "\nGarden preview · approval required\n";
                for (const auto& step : llsd::inArray(result["preview"]["steps"]))
                { display << "• " << step["after"]["name"].asString() << "\n"; describe(display, step["after"]); }
            }
            if (operation == "citizen_inspect")
            {
                display << "\nPersistent memories\n";
                for (const auto& memory : llsd::inArray(result["memories"])) display << "• " << memory["content"].asString() << "\n";
                display << "\nRecent agent events\n";
                for (const auto& entry : llsd::inArray(result["recent_events"])) display << "• " << entry["kind"].asString() << "\n";
            }
            self->show(display.str()); self->busy(false); return;
        }
        if (operation == "inspect" || operation == "plan") self->mLastPlan = reply["result"]["undoable_plan_id"].asString();
        if (operation == "plan")
        {
            LLSD request; request["request"] = prompt; request["world_context"] = modelContext(reply["result"]);
            LLSD generated = post("http://127.0.0.1:18765/v1/world/plan", request);
            self = current(); if (!self) return;
            if (generated.has("error")) { self->busy(false); self->show(generated["error"].asString()); return; }
            self->mPrepared = LLSD();
            if (!generated["plan"].isMap()) { self->busy(false); self->show(generated["answer"].asString()); return; }
            LLSD prepare; prepare["op"] = "prepare"; prepare["plan"] = generated["plan"];
            LLSD validated = post(capability, prepare);
            self = current(); if (!self) return;
            if (validated.has("error")) { self->busy(false); self->show(validated["error"].asString()); return; }
            self->mPrepared = validated["result"];
            std::ostringstream preview;
            preview << generated["answer"].asString() << "\n\n" << self->mPrepared["summary"].asString() << "\n"
                    << "Region: " << region_id.asString() << "\n\n";
            for (const auto& step : llsd::inArray(self->mPrepared["steps"]))
            {
                const LLSD value = step["after"].isMap() ? step["after"] : step["before"];
                preview << "• " << step["type"].asString() << " " << value["name"].asString() << "\n";
                if (step["before"].isMap() && step["after"].isMap())
                {
                    preview << "  Before: " << step["before"]["name"].asString() << "\n";
                    describe(preview, step["before"]);
                    preview << "  After: " << value["name"].asString() << "\n";
                }
                describe(preview, value);
            }
            preview << "\nModel: " << generated["plan"]["provider"].asString() << " / " << generated["plan"]["model"].asString() << "\nApprove within two minutes. Build applies this exact proposal.";
            self->show(preview.str()); self->busy(false); return;
        }
        std::ostringstream text;
        if (operation == "inspect")
        {
            text << "Nearby world objects\n\n";
            for (const auto& item : llsd::inArray(reply["result"]["nearby_entities"]))
                text << "• " << item["entity"]["name"].asString() << " (" << (S32)item["distance"].asReal() << "m)\n";
            text << "\nInspection is limited to 64 meters and 64 objects.";
        }
        else if (operation == "journal")
        {
            text << "WorldLine · your recent operations\n\n";
            for (const auto& entry : llsd::inArray(reply["result"]))
                text << entry["status"].asString() << " · " << entry["proposal"]["request"].asString() << "\n";
        }
        else
        {
            const std::string status = reply["result"]["status"].asString();
            text << "World operation: " << status << "\n";
            if (status == "committed") self->mLastPlan = reply["result"]["plan_id"].asString();
            if (status == "undone") self->mLastPlan.clear();
            text << "Operation: " << reply["result"]["operation_id"].asString() << "\n";
            if (reply["result"]["error"].isString()) text << reply["result"]["error"].asString();
            self->mPrepared = LLSD();
        }
        self->show(text.str()); self->busy(false);
    });
}
