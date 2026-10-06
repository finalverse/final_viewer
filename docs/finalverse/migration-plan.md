# Phased migration and upstream separation

No features or mass rebranding are implemented in Phase 0. Each phase requires source research, baseline evidence, behavioral verification and a coherent local commit; passing compilation alone is insufficient.

The Phase 0 build passed and bounded world interactions were observed. The next repair gate covers the existing Python manifest-test import failure, incomplete avatar appearance in the local fixture and viewer unresponsiveness after object tests. The post-SIGTERM sample cannot diagnose the original cause. Resolve or explicitly scope these compatibility/reliability gaps before claiming a healthy product baseline.

| Phase | Deliverable | Exit gate |
|---|---|---|
| 0 | Source/provenance audit, exact baseline build, test and run evidence | Honest build status and unresolved gates recorded; passing runtime before product work |
| 1 | Controlled Finalverse identity and profile/service configuration | Login/About/bundle identity verified, notices preserved, old viewer profile isolated |
| 2 | Dependency-isolated contracts/core and one viewer adapter | No GL/UI imports in value/schema/policy code; unit and existing regression tests |
| 3 | AI floater, provider bridge, bounded structured inspection | Real nearby-object query from a working world; provider receives structured data |
| 4 | First semantic action: owned-object bounded move | Validated proposal visibly changes sandbox object; denied cases and undo conflict covered |
| 5 | Hardened kernel, grants, WorldLine and logical transactions | Durable lifecycle/results; restart/replay/unknown outcome and compensation behavior |
| 6 | Blender add-on / Studio bridge | Authenticated asset/action interchange, glTF, clear rights/provenance; no Blender runtime client dependency |
| 7 | Desktop product hardening | Mac/Windows/Linux functional smoke, perf/reconnect/update/privacy/release tests |
| 8 | Shared mobile core extraction | ARM-safe schema/policy/protocol subset builds independently of viewer UI/GL |
| 9 | iOS/iPadOS MVP | Native UI, independently evaluated Metal renderer, real world entry/action loop |
| 10 | Android MVP | Native UI, independently evaluated Vulkan path, real world entry/action loop |
| 11 | Cross-device session/world continuity | Authenticated synchronization and conflict/reconnect behavior |
| 12 | Measured rendering modernization | Representative-scene parity, measurable gain and rollback |
| 13 | Progressive native systems | Replace a subsystem only with explicit evidence/benefit and compatibility gate |

## Separation strategy

Keep upstream-derived directories, symbol names, protocol formats and notices intact. New domain/schema/policy code belongs in clearly named additive Finalverse modules, with CMake target boundaries and narrow adapter references. UI registration is a small LLUI/XUI patch; product identity is configuration/assets. Do not fork global singleton patterns into the portable core.

Use small patches and conventional commits for each milestone. Record the inherited starting commit and every deliberate deviation. Maintain a source/provenance ledger rather than a misleading statement that the entire fork is newly authored. Vendor-specific AI adapters belong behind the brain/provider interface and must not define the WorldAction protocol.

Upstream merges are deliberate reviewable work, not an hourly auto-push assumption. Pin external build variables/dependencies, run meaningful regression gates and preserve user changes. A passing baseline may require a dedicated build-repair milestone first; do not bury that repair inside branding or AI work.

## First meaningful AI milestone

Real sandbox login and normal avatar/object/inventory behavior must remain intact. Open AI, ask “What objects are around me?”, obtain a structured loaded-world snapshot, then request a small move of a selected owned root object. AI proposes; trusted policy and current viewer permissions validate; existing editing/networking performs it; server state is observed; the scene visibly changes; the user can inspect the result and perform a validated compensating undo. This gate is not satisfied by logos, a mock world, a JSON fixture or a UI screenshot alone.
