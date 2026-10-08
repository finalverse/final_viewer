// Copyright (c) 2026 Finalverse contributors. SPDX-License-Identifier: LGPL-2.1-only
#ifndef LL_FINALVERSE_PRESENTATION_H
#define LL_FINALVERSE_PRESENTATION_H
#include "llsd.h"
#include <string>
// Human-readable deterministic preview of validated steps. Never fabricates
// recipes or hides deletions. Protocol identifiers belong in technical details.
std::string llfinalversePlanSummary(const LLSD& plan);
// Vendor account benefits are optional on a non-system grid. An advertised
// service still requires its normal error notification if initialization fails.
bool llfinalverseBenefitsExpected(bool system_grid, const LLSD& response);
#endif
