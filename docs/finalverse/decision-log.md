# Phase 0 decision log

Date: 2026-10-07 (Asia/Shanghai).

| ID | Decision | Source reason / consequence |
|---|---|---|
| D000 | Audit direct `finalverse/final_viewer` (now `~/Finalverse/viewer`) | GitHub identifies it as the SL client; distinct from Firestorm alternatives and unrelated Rust hub. Selection communicated after user supplied Finalverse organization. |
| D001 | Preserve existing checkout including file modes and untracked build flags | 9,330 mode differences plus `.gitignore` content changes and `build-variables/`; clean baseline uses detached temporary worktree |
| D002 | Pin starting commit `218de297a4a2cc286aba54e5df40e5e9e69caf43` | No fetch/merge/rebase/rebranding used for baseline |
| D003 | First baseline is open Intel desktop configuration on Apple Silicon via compatibility runtime | SSE2 intrinsics/architecture mapping exclude native ARM at this commit; Rosetta availability probed |
| D004 | Existing renderer stays | `LLPipeline`/llrender/GL window sources are mature and coupled; no AI requirement justifies replacement |
| D005 | First client seam stays C++ | Existing event/edit protocols are directly reusable; introducing Rust FFI now adds cost without evidence |
| D006 | Semantic model is a sidecar/projection | LLViewerObject carries render/asset/region state and partial world cache lifetime |
| D007 | Reuse structured nearby-object and autopilot APIs behind policy | LLAgentListener is already structured; unrestricted LLEvent/LEAP exposure would bypass authorization |
| D008 | Transactions initially mean logical lifecycle + compensation | No server-side atomic transaction or action idempotency key is present in legacy edit protocol |
| D009 | First write is bounded own-object move | Existing MultipleObjectUpdate path, observable transform and feasible inverse; excludes destructive/sale/script operations |
| D010 | Provider routing/secrets live behind neutral boundary | Cloud keys cannot be shipped in client; local endpoint support remains separately configurable |
| D011 | Mobile/Studio are later separate clients | No mobile targets/dependency closure; llui/GL/plugin assumptions cannot be carried wholesale; Blender remains an external add-on |
| D012 | Initial audit is not release clearance | LGPL/file notices, artwork/trademarks, mixed/proprietary manifest packages and service policy require distribution-specific evidence |

| D013 | Move world projects under `~/Finalverse` with old-path links | User explicitly authorized all Finalverse-related project moves; 30 repository Git states/inodes preserved |
| D014 | Use private, fresh local MutSea account and data | User requested new development user/local simulator; no original simulator state reused |
| D015 | Report appearance and standalone test gaps | Actual world/movement/cube/undo work; cloud avatar and Python import failure prevent full-product validation |
| D016 | Record runtime unresponsiveness without asserting its cause | UI retrieval timed out; SIGTERM did not stop the viewer. Sample was captured after SIGTERM and cannot prove the original trigger. Verified test process was force-stopped; simulator shut down normally. |

No proposed Finalverse module above is implemented by this audit. Build deviations and verification status are recorded separately in `upstream-baseline.md`.

## Phase 1 decisions

| ID | Decision | Reason / consequence |
|---|---|---|
| D017 | Implement deeper product identity following the user's explicit instruction | Distinct channel, artwork, native menus, local welcome, profile and packaging; no AI or renderer replacement |
| D018 | Develop on `codex/finalverse-branding`, then integrate verified local commits by fast-forward | Preserve the original checkout's unrelated edits and restore its original modes; retain a runnable app separately |
| D019 | Use Finalverse profile and protocol ownership | No automatic profile migration, upstream process closure or shared URL-handler takeover by installers |
| D020 | Disable the inherited updater and use manual GitHub release discovery | No trusted Finalverse signed-update service exists at this phase |
| D021 | Keep service-specific Second Life identity and source attribution accurate | Credentials, network compatibility, legal notices and supplier provenance must not be relabeled |
| D022 | Make upstream synchronization a manual read-only inspection | Product releases require reviewed integration; automated merge/push is unsuitable |
| D023 | Repair packaging tests against current library APIs | Preserve actual library semantics while restoring executable regression coverage |
| D024 | Use supplied Finalverse logo/icons/Lumi; preserve MutSea server identity | User explicitly selected existing brand materials and confirmed server naming; record asset hashes, preserve original files and existing rights |

See [rebranding.md](rebranding.md) for actual implementation and measured verification.

## Phase 2 decisions

| ID | Decision | Reason / consequence |
|---|---|---|
| D025 | Adopt the user-supplied AI world-creation vertical slice as Phase 2 | Bundle inspection, action policy, one shell, persistent home and Lumi semantics; defer broad porting/facelift work |
| D026 | Use opt-in authoritative MutSea Seed capability/module | Reuse authenticated avatar lifecycle, server permissions, scene APIs, backup and UDP; no second world runtime |
| D027 | Keep the first planner in a separate Python service with a provider protocol | Small loopback development service, no new viewer FFI or cloud key; local Ollama supplies actual live inference |
| D028 | Treat intent/compensation as logical transactions | Durable file and simulator database are separate stores; interrupted operations stop AI writes; no atomicity claim |
| D029 | Generate a home from 17 inherited primitives | Deterministic bounded geometry with real apertures and furniture; no external asset generator or renderer change |
| D030 | Project semantics/Lumi from private WorldLine | Keep object layouts compatible; human owns geometry, Lumi has a semantic binding; citizen autonomy deferred |
| D031 | Isolate Phase 2 in viewer/server worktrees | Preserve 9,332 primary viewer status entries and 27 deferred Linux changes; no primary integration before visual gate |
| D032 | Disable automatic login for unknown explicit grids | Source-verified grid keys and one-session settings prevent generated credentials falling back to another service |
| D033 | Keep visual acceptance as a separate required gate | Real model/capability/UDP/restart tests and compilation pass; a locked Mac prevents claiming a completed UI demonstration |
