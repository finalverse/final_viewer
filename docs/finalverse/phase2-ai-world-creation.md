# Phase 2: AI world creation

Starting viewer: `cb7e386cd5ec3e14b9bb602ac1dd4a689b094724`. Starting MutSea: `13c546ce413e49e957e387547d97e79800cf33ca`. Existing unrelated viewer modifications and deferred Linux work remain in their original checkouts. No push is authorized.

## Implementation map

1. Add an opt-in MutSea region module exposing `FinalverseWorld` through the existing authenticated avatar Seed capability lifecycle. Scope inspection to the avatar, selection and a bounded radius/count. Never pass session IDs or capability URLs to a model.
2. Define versioned JSON contracts and a strict allowlist. A provider proposes values; server code binds actor/region, checks ownership and simulator permissions, finite geometry, placement, counts and stale state. Prepared plans are immutable, session-bound and expire.
3. Reuse `Scene.AddNewSceneObject`, `SceneObjectGroup.UpdateGroupPosition`, group update scheduling, `Scene.DeleteSceneObject` and existing simulator backup. Primitive construction reuses `PrimitiveBaseShape`; no second scene/runtime or protocol rewrite.
4. Persist semantic entities, Lumi identity/home/memory and append-only WorldLine beside simulator data. Composite creation uses prevalidated geometry, durable intent, compensation and exact recovery records. It is not an atomic database transaction across simulator/sidecar stores.
5. Add a development-only Python gateway with a provider interface and one local Ollama adapter. Python avoids a new Rust/C++ FFI/toolchain dependency in this bounded first service. The gateway has no execution capability. Deterministic test providers are explicitly labeled.
6. Add one compact C++/XUI floater through LLFloaterReg and an existing menu entry. It captures selected UUID and anchor, requests authoritative inspection, displays the plan, and supports Build/Cancel/Undo. Existing renderer and desktop UI remain.
7. Verify schemas, policy, generator, stale/conflicting operations, partial failure/compensation, journal recovery and semantics with automated tests. Verify real capability requests and UDP observations against an isolated Harbor copy, then restart and inspect persistent state. Finally verify the built macOS app in-world.

## Source evidence

Viewer: `indra/newview/llviewerregion.cpp` Seed capability list; `llselectmgr.*` selection/property lifecycle; `llagent.*` avatar position; `llcorehttputil.h` JSON coroutine adapter; `llviewerfloaterreg.cpp` and XUI registration. Server: `SimulatorFeaturesModule.cs` per-avatar capability registration; `Scene.Permissions.cs` rez/edit/delete/entry checks; `Scene.cs` insertion/deletion; `SceneObjectGroup.cs` transforms/backup; inherited SQLite scene persistence.

## Release gate

Live read → selected-object move → approved Lumi home → authoritative visible geometry → logout/server restart → home/semantics/Lumi/WorldLine retained → guarded undo. Compilation alone does not pass the gate. Unsupported actions must fail explicitly. Avatar appearance, platform ports and production/public deployment remain separate gaps.

## Implemented candidate — 2026-10-07

**Phase 2 is not complete.** The live backend/model/protocol loop passes. The human-facing Create demonstration remains blocked because computer-use reports the Mac is locked and cannot unlock it. No Phase 2 screenshot, user-clicked Build or visual home verification is claimed. Unlocking the Mac is the remaining external dependency for that gate.

Source and artifact locations:

- Viewer: `~/Finalverse/dev/worktrees/viewer-ai-world`, branch `codex/finalverse-ai-world`; one C++/XUI Create floater, capability registration, explicit-grid login guard and regression coverage.
- MutSea: `~/Finalverse/dev/worktrees/mutsea-worldops`, branch `codex/mutsea-worldops`; separate `Finalverse/World` module/kernel, `tools/Finalverse.WorldTests`, `tools/Finalverse.WorldSmoke`, and reproducible `deploy/worldops/Dockerfile`.
- Planner: separate local Git repository `~/Finalverse/services/ai-gateway`, branch `main`, no remote. Provider protocol, Ollama adapter, strict schemas, scoped context and deterministic CI tests.
- App: `~/Finalverse/dev/phase2/Finalverse Test.app`, Intel x86-64 macOS build through Rosetta. Native ARM64 viewer remains unverified.
- Fixture: `mutsea-finalverse-phase2`, native ARM64 server, isolated persistent volume, ports bound only to `127.0.0.1:18092` TCP/UDP. Existing Harbor/source checkouts remain separate.

MutSea starting commit: `13c546ce413e49e957e387547d97e79800cf33ca`. Local runtime/deployment baseline: `6bdfe1c46c`; ignored-project tracking repair: `a4a6686641`; kernel/module: `eb719c4be1`; stronger live grounding/memory assertions: `4a2374e472`. Planner root milestone: `cec43c6`. Viewer implementation-map milestone: `c22f1e4f83`; shell/security milestone: `8939d1f47f`. The final documentation commit is reported in Git history and the delivery report. No push or primary integration was performed.

## Measured verification

| Check | Result / limit |
|---|---|
| Kernel policy, generator, compensation, recovery | 38 passed on host and native .NET 8 ARM64 image build |
| Planner/schema/context tests | 14 passed; explicitly mocked providers, no paid inference |
| macOS build | `RelWithDebInfoOS`, Xcode 26.3, pinned autobuild 3.10.2; succeeded |
| Viewer C++ test groups in final build | 682 cases: 678 passed, 4 inherited known-failure skips, zero failures |
| Packaging tests | 8 passed; inherited manifest compatibility repair retained |
| Bundle/provenance | Actual final app identity/artwork/notices/update isolation passed; changed XML parses |
| Real model read/move/home | Ollama `llama3.2:3b`; grounded read, selected UUID, two-meter relative move/undo, 17-component home, idempotent retry, Lumi ownership and WorldLine passed |
| Independent API families | Creation, rotation, scaling, name, description, color, restricted clone/delete and restoration passed against real capability/UDP |
| Normal server restart | All 17 home components/positions/scales, semantic owner, Lumi home/memory and WorldLine retained |
| Guarded home undo | Passed after restart, removal observed over UDP |
| Restart after undo | All 17 remained absent; Lumi/semantic relationship removed; undo history retained |
| Client login/world entry | Explicit local grid reached `STATE_STARTED`; Create UI workflow remains unverified |
| Visual release acceptance | **Blocked: locked Mac** |

The four skips are inherited `llmainthreadtask` hang, flaky `LLHost`, `LLSphere` SNOW-620 and architecture-sensitive `m3math_h` cases. Counts are from the final build, not sums of repeated builds. The probe independently observes UDP existence/position/scale; rotation/color/name execution is checked against authoritative simulator snapshots rather than every independently decoded UDP attribute.

The live read answer named eight actual starter entities, including Workshop Walk, Arrival Marker and Welcome Plaza. Read output proposes no action. Explicit move/home requests narrow tool choice; the model resolves IDs and parameters. Trusted code computes direction, dry-ground elevation and the 17 components. Invalid provider results fail without mutation. This is a bounded command slice, not general language understanding.

The add-on image inherits the verified base runtime/user/entrypoint, runs native kernel tests and copies one disabled-by-default DLL. Base image ID: `bf75cb36aa53f09b0e1ccce14ebfab91a0c9a2f7cbb3897a78a98386ffb9c559`; world-ops ARM64 image ID: `e8d8c16ace7df9162ba5e6ad47f207699c104ecc0bf84d649ac2e75d5d4df568`. Deployed module SHA-256: `c3175e0ee900d26d530bd9c591eefb7d2dfde14f8b2c5b043d8d4ed560c33694`. Final client executable SHA-256: `4b4e036fc92898adece7697ccc37fe8fdfd5b574bd6f54c5b3fbb6c6fd5e9c2f`. These are development artifacts, not signed releases.

## Finish the visual gate

Use the server's `Finalverse/World/README.md` for the image/module and protocol probe. Use the planner README for the private virtual environment, local Ollama model and loopback endpoint. No cloud key is required.

1. Start the owned local fixture and planner. Use its generated development account privately. Launch the isolated client profile with the verified `MutSeaHarborAI` grid. Source-compatible grid fields are `keyname`, `grid_login_id`, `login_uri`, `helper_uri`.
2. Bootstrap using a private one-session `--sessionsettings` file and remove it after successful entry. Do not persist a password with `--settings`.
3. With the Mac unlocked, move near the practice objects. Open **Finalverse → Ask / Create**, inspect Nearby, ask “What objects are around me?”, and verify names in the visible panel.
4. Select Practice Cube; ask “Move this two meters toward the Harbor Pavilion Floor.” Review ID and exact before/after values, click Build / Apply, observe the change, inspect History and safely undo.
5. Stand on dry clear ground near `(172,154)`. If a backend test home remains, use its guarded undo before creating a replacement. Ask “Create a small lakeside home for Lumi here.” Review 17 primitives, approve and capture the visible result.
6. Query the authenticated semantic/Lumi API. Log out before using the same account in a protocol probe. Shut down the owned simulator normally, restart, relaunch/login, visually verify retained geometry/history, then verify guarded undo/restoration.
7. Record screenshots and behavioral results. Repair any selection/panel/async errors before primary integration or a completion claim.

During initial launch verification an incorrectly keyed custom grid entry was ignored and inherited fallback login was attempted. The generated development credential was rotated in both affected isolated fixtures and tokens revoked. The unknown-grid auto-login regression now prevents fallback credential transmission. No user/cloud credential was changed. Log-based world entry is separate from the pending visual demonstration.

## Evidence and preserved work

Private evidence under `~/Finalverse/dev/phase2`: `worldops-image-build-final.log`, `kernel-tests-final.log`, `gateway-tests-final.log`, `viewer-build-delivery.log`, `manifest-tests.log`, `branding-check-delivery.log`, `live-apply-final.log`, `acceptance-final.json`, `restart-verify-final.log`, `restart-undo-final.log`, `undo-persistence-final.log`, `live-restore-demo.log`, `restart-verify-demo.log`, model traces and the isolated profile's viewer log. Account data, raw logs, journals, database backups and binaries are excluded from Git. No Phase 2 screenshot is available while the Mac is locked.

Primary viewer remains `contribute` at `cb7e386cd5`, preserving 9,332 status entries and porcelain SHA-256 `3c3986254ed6e78a948e9a4905c879ed9cf1e998e06e404b8437a4f7786d028c`. Deferred Linux work retains 27 changes and hash `4ced83584b82b79c62473b5e03008d362b82f86f5f5000af4af83a4484a5210b`. Primary MutSea remains clean at its starting commit.

## Limits and next milestone

Intent/compensation are logical transactions; simulator database and journal are not atomic, and legacy edits do not share the module lock. Recovery requires administrator review. Clone/delete/undo support restricted primitive state. Existing-object transforms do not include a general collision solver. Regional snapshots/journals need production indexing and quotas. The loopback planner lacks production authentication/rate limits; capability theft retains inherited user authority. Provider claims are unsigned.

Lumi has semantic identity, a home and creation-request memory; no avatar, navigation, voice, visitor memory or autonomous citizen runtime. Manual legacy deletion is not reconciled into Lumi's projection. Avatar cloud appearance, native Apple Silicon viewer, other platform clients, public simulator deployment and distribution clearance remain separate gaps. Renderer, economy and geography work were deferred.

Finish visual acceptance, then harden recovery and semantic reconciliation before persistent Lumi memory/navigation through the same authorized API.
