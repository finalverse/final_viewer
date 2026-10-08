// Copyright (c) 2026 Finalverse contributors. SPDX-License-Identifier: LGPL-2.1-only
#ifndef LL_FINALVERSE_LEGACY_APPEARANCE_H
#define LL_FINALVERSE_LEGACY_APPEARANCE_H
#include "lluuid.h"
#include <string>
class LLViewerTexLayerSet;
class LLRenderTarget;
// The region protocol bit is authoritative. Never downgrade a system grid or
// infer client baking from a temporarily missing appearance capability.
inline bool llfinalverseClientBakingRequired(bool region, bool server_bakes, bool system_grid)
{ return region && !server_bakes && !system_grid; }
bool llfinalverseLegacyAppearanceEnabled();
void llfinalverseStartLegacyAppearance();
void llfinalverseInvalidateLegacyBake(U8 texture_index);
void llfinalverseUploadLegacyBake(LLViewerTexLayerSet* layer, S32 x, S32 y, S32 width, S32 height,
                                LLRenderTarget* bound_target);
#endif
