# Avatar appearance audit — Phase 4B

2026-10-08–09, macOS ARM64; viewer starting commit `f0b70e89c2691d16ccb318a3f6afa348fdcec8ea`, MutSea `e7cd5f9b27f09ec2dba7dbe46072ed54bc7ce5c7`. This records the compatibility repair and its measured development acceptance. Phase 4B content and distribution acceptance remain separate.

## Observed baseline

The preserved Phase 4A app again enters MutSea Harbor and visibly renders its local avatar as a cloud. Native PNG `arrival-before.png` was exported at 1280 × 738, UI scale 1.00, 2026-10-08 20:34:56 Asia/Shanghai; private original filename `explore-facade_2026-10-08_20345601.png`. The simulator initially became unavailable with Docker Desktop's unresponsive engine. The supported stop/start CLI recovered it; only the named Finalverse sandbox was explicitly started. Other Docker restart-policy containers resumed automatically. No volumes were removed.

The viewer log at 11:09:17 UTC contains `Stale appearance received #-1 attempt to roll back from #-1... dropping.` At 11:09:18 it starts `setWearableOutfit`; its Current Outfit fetch completes six wearable links. Thus an empty inventory alone does not explain this existing account's cloud. Wearable parameter-count warnings and missing material assets are separate findings, not evidence that every wearable fetch failed.

## Source evidence

| Seam | Finding |
|---|---|
| MutSea `LLClientView.SendRegionHandshake` | RegionProtocols sets bit 63 (extended bakes) and leaves bit 0 (server-side baking) clear |
| MutSea `LLClientView.SendAppearance` | Sends visual parameters and texture entries with no AppearanceData/COF version block |
| Viewer `LLViewerRegion` | Initializes central bake mode to 1, then derives it from handshake bit 0 |
| Viewer `LLVOAvatar::processAvatarAppearance` | Self COF stale check compares both initial/missing versions at -1 and rejects the legacy update |
| Viewer `LLVOAvatar::getBakedTextureImage` | Fetches every ordinary baked ID from the appearance-service URL; no legacy asset-service branch |
| Viewer `LLAgentWearables::setWearableOutfit` | Loads local wearables/parameters but does not select local compositing outside appearance editing |
| Viewer `LLViewerTexLayerSetBuffer` | Retains local compositor but has no bake upload/publishing path |
| Viewer `sendDummyAgentWearablesUpdate` | Publishes four intentional placeholder item IDs for the system-grid compatibility path |
| Git history | `d58e7cfbfc` (2013-09-19), “SH-3455 WIP - removing bake upload code”, removed the client bake upload implementation; its parent retains RGBHM readback and AgentSetAppearance protocol |

The inherited runtime protocols and the modern server-baking-only assumptions do not match. A cosmetic removal of cloud particles would leave missing appearance distribution/persistence, so it is insufficient.

## Compatibility repair

`llfinalverselegacyappearance` restores a limited adapter only when a non-system grid's region explicitly advertises client baking. It reuses local wearable textures, the existing compositor, five-channel JPEG2000 RGBHM encoding, authenticated UploadBakedTexture capability and AgentSetAppearance/AgentIsNowWearing messages. System-grid/server-baking behavior remains gated away from this adapter. Bake results are rejected after session, region or per-layer generation changes. Capability URLs and credentials are not added to model context.

Self unversioned simulator appearances do not override its actual local wearables in client-bake mode. Remote baked IDs use the inherited asset texture fetch path in regions declaring that mode. Actual inventory item IDs replace dummy items only in that same protocol mode. Required body/texture readiness is still checked by the inherited avatar code; no forced loaded/cloud flag is introduced.

The RGBHM readback and message structure derive from inherited Linden code and retain LGPL provenance. Upload implementation uses current coroutine HTTP APIs, rather than restoring obsolete VFS/responders wholesale. Retry behavior, region transitions and multi-layer wearable compatibility need further verification before distribution. The legacy wearables message currently publishes the first item per type; multi-layer clothing is not a completed gate.

The non-AIS Current Outfit fetch also needed repair: its inherited recursive-fetch timer had been stopped without restoring the normal fetch state. An incomplete folder could then wait indefinitely until the user opened inventory. The adapter restores `FETCH_NONE`, marks incomplete versions unknown and schedules the existing background folder fetch. A fresh isolated profile reached a visibly loaded avatar automatically in 20 seconds, with six resolved outfit links. No manual inventory opening was needed.

## Clothing color save defect

The old build visibly reproduced the reported failure: changing pants from red to `#384B60`, then saving and leaving appearance editing, restored red. The wearable upload succeeded, but `update_inventory_item` unconditionally called AIS `UpdateItem` on this non-AIS grid. MutSea therefore retained the old wearable asset reference. The COF link replacement also used an unavailable AIS create path.

For a non-system grid without AIS, `LLViewerInventoryItem::updateServer` now sends the inherited authenticated `UpdateInventoryItem` transaction message. For transaction-backed asset saves, an inventory observer waits for the server's actual `UpdateCreateInventoryItem` response with matching item, asset and name. MutSea returns callback ID zero, so registering a callback ID alone would not complete this save. Completion publishes the existing real wearables message and releases the existing appearance callback; a 60-second missing acknowledgement produces the inherited save-error notification. Session/logout changes cancel the observer. Metadata-only updates do not enter this asset acknowledgement wait.

Client-bake mode keeps the existing Current Outfit link, whose target item ID is unchanged when its asset changes. The regular AIS/server-bake COF behavior is retained outside that mode. Simulator ownership/permission checks and asset transaction handling remain authoritative.

The new build's log records `Saved wearable pants` followed by `Legacy inventory asset save acknowledged`. Closing the editor visibly retains the new pants. A read-only copy of MutSea's inventory/asset database contains the changed original pants asset and wearable parameters 806/807/808 = 0.22/0.29/0.38, matching the selected color after wearable quantization. A separate red `Default Pants (new)` item created during earlier Save As testing remains untouched.

## Executed checks

- Universal ARM64/x86-64 RelWithDebInfo build succeeds; actual UI execution is on Apple Silicon.
- CTest: 116/116 passed after the appearance repair, again after the COF fetch repair, and after the pants-save repair (145.52 seconds for the latter run).
- Final scoped save observer build succeeds; CTest again passes 116/116 (36.03 seconds).
- Ordinary level-zero fresh account provisions six body/outfit items, six Current Outfit links and persistent appearance through existing services.
- Fresh-profile automatic avatar loading and actual pants edit/save/editor-close checks pass.
- A different account using `tools/MutSea.AppearanceSmoke` receives the target's real UDP avatar appearance, 253 visual parameters, and downloads head/upper/lower baked textures through negotiated `GetTexture`; all three are five-channel JPEG2000 RGBHM codestreams. This proves distribution to a second protocol client, not a second viewer's rendered screenshot.
- Normal simulator console shutdown/start and viewer relaunch preserve the original pants asset reference and visibly retain gray-blue pants. The same second-client probe passes after restart; the lower bake hash differs from the earlier red-pants baseline.

Private logs, credentials, database snapshots and receipts remain under `~/Finalverse/dev/phase4b`, outside Git. Bake uploads are temporary simulator/cache assets; relogin recreates them from persisted wearables. Saved outfit durability must not be confused with a guarantee that a particular temporary bake UUID is permanent.

GUI automation limitation: the inherited LLUI exposes mostly one native window/menu rather than accessible controls, so visual checks use screenshots/coordinates. After the last restart, some automated mouse clicks produced hover but no activation, while keyboard commands still worked. The visible restart check and protocol/database receipts were retained; an additional native PNG export was not completed. Do not repeat unchanged click attempts indefinitely or treat a failed automated click as a proven viewer defect. The immediate viewer relaunch also encountered a stale online-session refusal; normal simulator restart cleared it. Login/session cleanup deserves a separate regression test.

## Provisioning is a separate problem

MutSea `UserAccountService.CreateUser` stores the identity first, then warns on password/inventory setup failures and can still report success. Default appearance is conditional on `CreateDefaultAvatarEntries`; `XInventoryService.CreateUserInventory` creates folders, not a complete appearance. `CreateDefaultAppearanceEntries` creates six default body/clothing items, links Current Outfit and calls AvatarService, but does not establish a recoverable all-or-ready provisioning transaction.

Phase 4B needs a fresh private operator-created account with resolvable body/outfit and observed restart persistence. Phase 4B.1 must add idempotent pending→ready provisioning using those existing services, dependency/asset validation and explicit failure recovery before public 4C registration. The package validator's required-slot declarations alone do not prove appearance readiness.
