// Copyright (c) 2026 Finalverse contributors. SPDX-License-Identifier: LGPL-2.1-only
#include "llviewerprecompiledheaders.h"
#include "llfloaterfinalverseai.h"
#include "llfinalversepresentation.h"
#include "llfloaterreg.h"
#include "lltextbox.h"
#include "llviewercontrol.h"
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
    getChild<LLCheckBoxCtrl>("details")->setCommitCallback([this](LLUICtrl*, const LLSD&) { renderActivity(); });
    show("Ask about the world, select an owned object to edit, or stand on clear ground to create Lumi's home. Every change needs your approval.");
    busy(false);
    return true;
}
void LLFloaterFinalverseAI::openRequest(const std::string& text, bool citizen, bool submit)
{
    LLTimer timer;
    auto self = dynamic_cast<LLFloaterFinalverseAI*>(LLFloaterReg::showInstance("finalverse_ai"));
    if (!self || self->mBusy) return;
    self->mPrepared = LLSD(); self->mCitizenPrepared = LLSD();
    self->getChild<LLCheckBoxCtrl>("citizen_mode")->set(citizen);
    self->getChild<LLButton>("ask")->setLabel(citizen ? "Send" : "Plan");
    self->getChild<LLLineEditor>("request")->setText(text);
    self->show(citizen ? "Talk to Lumi. Ask about her home, memories, or what she is doing." : "Ask about nearby objects or describe a change. Review and approve before anything is built.");
    self->busy(false);
    self->getChild<LLLineEditor>("request")->setFocus(true);
    if (gSavedSettings.getBOOL("FinalverseUXMetrics")) LL_INFOS("FinalverseUX") << "ai_open_ms=" << timer.getElapsedTimeF64().value()*1000 << LL_ENDL;
    if (submit) self->run(citizen ? "citizen_say" : "plan");
}
void LLFloaterFinalverseAI::renderActivity()
{
    getChild<LLTextEditor>("activity")->setText(mHumanText + (getChild<LLCheckBoxCtrl>("details")->get() && !mTechnicalText.empty() ? "\n\nTechnical details\n" + mTechnicalText : ""));
}
void LLFloaterFinalverseAI::show(const std::string& text, const std::string& details)
{
    mHumanText = text; mTechnicalText = details; renderActivity();
    auto object = LLSelectMgr::getInstance()->getSelection()->getFirstRootObject();
    auto node = LLSelectMgr::getInstance()->getSelection()->getFirstRootNode();
    const std::string name = object ? node && !node->mName.empty() ? node->mName : "Selected object" : "No object selected";
    getChild<LLTextBox>("context")->setText((gAgent.getRegion() ? gAgent.getRegion()->getName() : "Enter a world") + " · " + name);
}
void LLFloaterFinalverseAI::busy(bool active, bool mutation)
{
    mBusy = active; mMutation = active && mutation;
    for (const char* name : {"ask", "inspect", "history", "citizen_inspect", "citizen_pause", "citizen_resume", "citizen_cancel", "citizen_undo"}) getChild<LLButton>(name)->setEnabled(!active);
    // Human overrides can interrupt reasoning without waiting for the model.
    for (const char* name : {"citizen_pause", "citizen_cancel"}) getChild<LLButton>(name)->setEnabled(!mMutation);
    const bool citizen = getChild<LLCheckBoxCtrl>("citizen_mode")->get();
    getChild<LLButton>("build")->setVisible(!citizen);
    getChild<LLButton>("build")->setEnabled(!active && mPrepared.has("plan_id"));
    getChild<LLButton>("citizen_approve")->setVisible(citizen);
    getChild<LLButton>("citizen_approve")->setEnabled(!active && mCitizenPrepared.has("plan_id"));
    for (const char* name : {"citizen_inspect", "citizen_pause", "citizen_resume", "citizen_cancel", "citizen_undo"}) getChild<LLButton>(name)->setVisible(citizen);
    getChild<LLButton>("undo")->setEnabled(!active && !mLastPlan.empty());
    getChild<LLButton>("cancel")->setEnabled(!mMutation);
    getChild<LLLineEditor>("request")->setEnabled(!active);
    getChild<LLCheckBoxCtrl>("citizen_mode")->setEnabled(!active);
}
void LLFloaterFinalverseAI::cancel()
{
    if (mMutation) return;
    if (mCitizenPrepared.has("plan_id")) { run("citizen_cancel"); return; }
    ++mGeneration; mPrepared = LLSD(); mCitizenPrepared = LLSD(); busy(false);
    show("Proposal canceled. No world changes were requested.");
}
void LLFloaterFinalverseAI::run(const std::string& operation)
{
    const bool override = operation == "citizen_pause" || operation == "citizen_cancel";
    if (mBusy && (!override || mMutation)) return;
    if (operation == "commit")
    {
        if (!mPrepared.has("plan_id")) return;
        auto selected = LLSelectMgr::getInstance()->getSelection()->getFirstRootObject();
        if ((selected ? selected->getID() : LLUUID::null) != mPlanSelection)
        { cancel(); show("Your selection changed. Plan again for the current object."); return; }
    }
    if (operation == "citizen_approve" && !mCitizenPrepared.has("plan_id")) return;
    // A proposal is actionable only while its own preview is displayed.
    if (operation != "commit") mPrepared = LLSD();
    if (operation != "citizen_approve") mCitizenPrepared = LLSD();
    busy(false);
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
    show(operation == "plan" ? "Looking around and planning…" : operation == "citizen_say" ? "Lumi is considering your message… You can pause or cancel while she thinks." : "Waiting for the world service…");
    LLCoros::instance().launch("FinalverseWorld", [handle, generation, region_id, capability, operation, body, prompt]()
    {
        auto current = [&]() -> LLFloaterFinalverseAI*
        {
            auto self = dynamic_cast<LLFloaterFinalverseAI*>(handle.get());
            if (!self || self->mGeneration != generation) return nullptr;
            if (!gAgent.getRegion() || gAgent.getRegion()->getRegionID() != region_id)
            { self->mPrepared = LLSD(); self->mCitizenPrepared = LLSD(); self->mLastPlan.clear(); self->busy(false); self->show("Your region changed. Please inspect and plan again."); return nullptr; }
            return self;
        };
        LLSD reply = post(capability, body);
        auto self = current(); if (!self) return;
        if (reply.has("error")) { self->mPrepared = LLSD(); self->mCitizenPrepared = LLSD(); self->busy(false); self->show(reply["error"].asString()); return; }
        if (operation.compare(0, 8, "citizen_") == 0)
        {
            const LLSD result = reply["result"];
            self->mCitizenPrepared = LLSD();
            std::ostringstream display, technical;
            display << "Lumi: " << result["answer"].asString() << "\n\n";
            if (operation == "citizen_inspect")
            {
                const LLSD identity = result["citizen"]["identity"];
                display << "Activity: " << identity["lifecycle"].asString() << "\n";
                technical << "Home: " << identity["home_entity_id"].asString() << "\n";
                technical << "Capabilities: ";
                for (const auto& capability : llsd::inArray(identity["capabilities"])) technical << capability.asString() << " ";
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
                self->mCitizenPrepared = result["preview"];
                display << "\n" << llfinalversePlanSummary(result["preview"]) << "\n";
                for (const auto& step : llsd::inArray(result["preview"]["steps"]))
                { technical << "• " << step["after"]["name"].asString() << "\n"; describe(technical, step["after"]); }
            }
            if (operation == "citizen_inspect")
            {
                display << "\nPersistent memories\n";
                for (const auto& memory : llsd::inArray(result["memories"])) display << "• " << memory["content"].asString() << "\n";
                technical << "\nRecent agent events\n";
                for (const auto& entry : llsd::inArray(result["recent_events"])) technical << "• " << entry["kind"].asString() << "\n";
            }
            self->show(display.str(), technical.str()); self->busy(false); return;
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
            self->show("Checking placement and permissions…");
            LLSD prepare; prepare["op"] = "prepare"; prepare["plan"] = generated["plan"];
            LLSD validated = post(capability, prepare);
            self = current(); if (!self) return;
            if (validated.has("error")) { self->busy(false); self->show(validated["error"].asString()); return; }
            self->mPrepared = validated["result"];
            self->mPlanSelection = LLUUID(body["selected_id"].asString());
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
            // The validated steps determine the approval copy. The model's
            // answer can describe a different stage or imply an unmade change.
            preview << "\nPlanner answer: " << generated["answer"].asString();
            self->show(llfinalversePlanSummary(self->mPrepared), preview.str()); self->busy(false); return;
        }
        std::ostringstream text, technical;
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
            text << (status == "committed" ? "Done. Your world has changed. Use Undo last to restore it." : status == "undone" ? "Undone. The previous world state was restored." : "World operation: " + status) << "\n";
            if (status == "committed") self->mLastPlan = reply["result"]["plan_id"].asString();
            if (status == "undone") self->mLastPlan.clear();
            technical << "Operation: " << reply["result"]["operation_id"].asString() << "\n";
            if (reply["result"]["error"].isString()) text << reply["result"]["error"].asString();
            self->mPrepared = LLSD();
        }
        self->show(text.str(), technical.str()); self->busy(false);
    });
}
