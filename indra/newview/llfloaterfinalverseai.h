// Copyright (c) 2026 Finalverse contributors. SPDX-License-Identifier: LGPL-2.1-only
#ifndef LL_FLOATER_FINALVERSE_AI_H
#define LL_FLOATER_FINALVERSE_AI_H
#include "llfloater.h"

class LLFloaterFinalverseAI : public LLFloater
{
public:
    explicit LLFloaterFinalverseAI(const LLSD& key) : LLFloater(key) {}
    bool postBuild() override;
private:
    void run(const std::string& operation);
    void cancel();
    void busy(bool active, bool mutation = false);
    void show(const std::string& text);
    U32 mGeneration = 0;
    bool mBusy = false;
    bool mMutation = false;
    LLSD mPrepared;
    std::string mLastPlan;
};
#endif
