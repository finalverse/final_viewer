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

## Visual acceptance — 2026-10-07–08 (Asia/Shanghai)

The previously blocked Mac was unlocked. Actual Finalverse UI inspection, selected-object preview/apply, stale-plan rejection, History, move undo, valid home creation, post-restart viewing and composite home undo have now executed. All 15 practical release gates pass. The final packaged app entered Harbor and displayed live Nearby objects; its branding/metadata verification passed. This is a bounded development slice, not a production/public release.

Active sources:

| Component | Path | Branch | Continuation starting HEAD |
|---|---|---|---|
| Viewer | `~/Finalverse/dev/worktrees/viewer-ai-world` | `codex/finalverse-ai-world` | `bdd81b196e6663a11668cf646324a06fb8c5fb27` |
| MutSea | `~/Finalverse/dev/worktrees/mutsea-worldops` | `codex/mutsea-worldops` | `4a2374e47292d36a3405da2ccf4599d41ecc70ba` |
| Gateway | `~/Finalverse/services/ai-gateway` | `main` | `cec43c6aff05bf0108487ce90710771d857ab54a` |

Gateway repository name is **ai-gateway**, intended future location **finalverse/ai-gateway**, with no remote configured or created. Viewer origin is `finalverse/final_viewer`, with `qwy16/secondlife_viewer` as upstream. MutSea origin is `finalverse/mutsea-o`. All milestones are local; no push or integration into the original checkouts was performed. Continuation code commits are viewer `ab6f822682`, MutSea `ffab4ba878`, and gateway `07b9d45`/`7652282`. The documentation milestone and final full HEADs are recorded in the delivery report and Git history.

Client copy: `~/Finalverse/dev/phase2/Finalverse AI.app`, Intel x86-64 macOS through Rosetta. The executable retains the build-channel name `Finalverse Test`. Development bundle ID is `com.finalverse.viewer.phase2`; production default stays `com.finalverse.viewer`. Both Info.plist and packaging metadata must carry the same identifier. Native ARM64 viewer remains unverified.

Fixture: `mutsea-finalverse-phase2`, native ARM64 server, persistent `finalverse-phase2-data` volume, only loopback TCP/UDP port 18092. Local grid alias is `MutSeaHarborAI`, login URI `http://127.0.0.1:18092/`, region **MutSea Harbor**, account **MutSea Developer**. Private credentials, logs, screenshots and receipts remain outside Git.

## Measured tests and builds

| Check | Result / limit |
|---|---|
| Current kernel suite | 40 passed, zero failures; explicit fake-world policy/generator/compensation/recovery tests |
| Previously built native .NET 8 ARM64 image | 38 passed before the two new bounds cases; deployed module code is unchanged |
| Gateway | 26 passed, zero failures; self-contained mocked inference and temporary loopback HTTP peers |
| Full macOS viewer build | 1,199 cases: 1,195 passed, four inherited known-failure skips, zero failures; build succeeded |
| Packaging unit suite | Eight passed, zero failures in the final continuation run |
| Final package/identity | Package-only step succeeded; generated CMake copy/package commands forward the same bundle ID; seven branding/provenance check groups passed |
| Final packaged viewer | Actual Harbor entry and live Nearby panel verified visually |
| WorldSmoke probe build | Succeeded with zero warnings/errors; references the separately built protocol SDK |
| Actual local inference | Ollama `llama3.2:3b`: grounded read, relative move, valid home; no paid/cloud provider |
| UI-created home capture | 17 exact IDs observed over UDP, reciprocal Lumi semantics retrieved through capability API |
| Normal simulator restart | IDs, positions, rotations, scales, every semantic scalar, Lumi ID/home/memory and WorldLine retained |
| Composite UI undo | All components disappear visibly; API/protocol cleanup survived a second normal restart |

The four inherited skips are `llmainthreadtask` build-time hang, flaky `LLHost`, `LLSphere` SNOW-620 and architecture-sensitive `m3math_h`. Counts describe one full build, not sums of repeated builds. The earlier delivery's 682 cases came from an incremental run; the continuation full build executed 1,199. No static analyzer, native ARM64 viewer or Windows/Linux/mobile build is claimed.

The first identity verification caught an upstream fallback in `build_data.json`: the Darwin packaging target omitted the bundle-ID argument. The source now forwards it for both copy and package paths; the verifier compares actual app and metadata IDs. After the full C++ build passed, a redundant direct-CMake rebuild was deliberately interrupted (exit 130). The final package-only command ran in the CMake build/newview working directory against the completed executable and succeeded; no C++ feature source changed afterward. The final executable SHA-256 is `4b4e036fc92898adece7697ccc37fe8fdfd5b574bd6f54c5b3fbb6c6fd5e9c2f`. A development-specific ID and isolated profile avoid selecting the regular viewer during UI automation.

## Runtime evidence

**Inspection.** “What objects are around me?” answered using eight actual Harbor starter entities, with no write plan. The visible panel named real nearby objects. Named Pavilion Floor was outside 64m; a small model initially substituted an unrelated tree in a preview. That proposal was never applied. The gateway now resolves unique exact inspected names, binds selected/destination IDs in the output schema, and rejects substitutions at the provider boundary. The UI then displayed a clear unavailable-target response without enabling Build.

**Stale state and move.** Practice Cube ID `77a0db68-744a-a75d-8015-256136336a8c` was manually edited using the inherited Edit tool from Y=128 to Y=129.898 while an AI preview was pending. Build rejected it with “The target changed since this plan was prepared. Please plan again.” The human edit survived. Replanning “Move this two meters toward the Practice Sphere.” moved `[64,129.898,26.3]` to `[65.907,129.295,26.3]`, visibly and in the Edit inspector. Commit `7ec34ac3-206a-4b80-ae22-8b7d15466686` appeared in History. UI undo `4351ab41-00bc-4fc2-8267-c559156af9fb` restored the pre-AI position, including the human's Y edit.

**Placement.** “Create a small lakeside home for Lumi here.” at `[172,153.454376,25]` was correctly refused by conservative lake-path clearance. The inspected anchor was copied exactly by the model. No overlap/radius/permission rule was weakened. The avatar was moved to `(172,156,27)` and settled on clear dry terrain. The approved proposal used anchor `[172,156,25]`, width 6m, depth 5m, height 3m, yaw zero, Lumi owner ID and 17 deterministic components. The readable UI preview preceded an explicit Build / Apply click.

**Home.** Commit `9d6dee15-e688-42ea-82df-9649438019f5`, plan `8ac56a65-1001-40d8-8d21-6f9859487652`, created a floor, walls with a door and window openings, roof, table, pedestal and seat. Structure/floor UUID is `d8ad7327-b8e4-5a2a-6e4f-ac1a3964aaab`. The whole house was visible from `(172,146,27)`; History showed the committed request. The house is primitive based, not externally generated mesh content.

**Semantic API.** A read-only WorldSmoke `capture` mode was added so the viewer-created home can be verified without creating a replacement. It uses actual login/Seed capability, `lumi`, `semantic`, `journal` and independent UDP observations. It never prepares, commits or undoes a world edit. Both links passed:

```text
Lumi.agent_id       = b151468b-cbcc-4af2-ae2d-6967e8938bb4
Lumi.home_entity_id = d8ad7327-b8e4-5a2a-6e4f-ac1a3964aaab
home.owner_agent_id = b151468b-cbcc-4af2-ae2d-6967e8938bb4
home.semantic_type = home
provider/model     = ollama / llama3.2:3b
```

Human ownership remains the actual simulator permission owner; Lumi's ownership is a semantic relationship. Creation-request memory is retained; `avatar_binding` remains null.

**Persistence.** The viewer logged out normally, the probe captured the exact UI operation and logged out, and MutSea received its normal `shutdown` console command and exited with status zero. The same container/volume was started again. The probe observed all 17 original UUIDs and position/rotation/scale values, compared every semantic scalar, Lumi identity/home/memory and the committed WorldLine operation. No identity remapping was needed. The viewer re-entered Harbor and the same home remained visible; Nearby and History stayed queryable. A probe comparison initially compared escaped raw JSON timestamp text; decoded values were unchanged and the assertion was corrected.

**Composite undo.** After restart, Nearby loaded the last reversible plan and the viewer's Undo last removed all 17 components. UI reported reversal `3ef27b6b-13dc-4d9a-994b-e3566e329ef7`; the site visibly cleared. A second normal logout/shutdown/restart passed: all 17 stayed absent over UDP and inspection, Lumi.home_entity_id was null, semantic retrieval reported found=false and undo history remained. The acceptance home was deliberately undone; the site is clear for another build.

## Release gates

| Gate | Result | Evidence |
|---|---|---|
| 1 Enter MutSea Harbor | PASS | Actual original and final packaged viewer entry |
| 2 Grounded nearby-object question | PASS | Real local inference and visible scene-object names |
| 3 Selected cube AI proposal | PASS | Selected UUID and exact approved before/after preview |
| 4 Visible move | PASS | Scene and inherited Edit inspector |
| 5 History records move | PASS | UI History and authoritative journal |
| 6 Undo restores cube | PASS | Visible pre-AI position, human edit retained |
| 7 Reject stale preview | PASS | Manual Edit after preview, clear refusal |
| 8 Reject invalid home placement | PASS | Lake-path overlap refused without policy changes |
| 9 Valid home build | PASS | 17-component UI preview and explicit Build |
| 10 Visible home | PASS | External entrance view |
| 11 Lumi semantic relationship | PASS | Authenticated semantic/Lumi API, reciprocal UUIDs |
| 12 Restart persistence | PASS | Original UUIDs/full transforms/semantics/history and visible home |
| 13 Gateway tests | PASS | 26 self-contained tests plus real local inference |
| 14 Relevant viewer/server tests | PASS | Full viewer 1,195 passes/four known skips, kernel 40, packaging eight, live probes |
| 15 Reproducible local instructions | PASS | Source-matched guide; build/package/test, launch/grid, gateway sample and normal server/account-console workflow |

The scope is the development vertical slice. A PASS does not claim production hardening, public registration, full avatar appearance or cross-platform release readiness.

## Failure-mode coverage

| Failure | Evidence |
|---|---|
| Named target outside scope / ambiguous name | Actual UI refusal plus gateway and kernel tests |
| No selected object for “this” | Gateway test: no provider call and no plan |
| Manual edit after preview / stale target | Actual Edit-tool change, UI rejection, preserved human edit; kernel test |
| Missing/invalid destination | Gateway missing/ambiguous/substitution tests; kernel missing/out-of-radius reference tests |
| Overlap/path clearance | Actual rejected home site; kernel tests including linked-child extents |
| Excessive structure dimensions / bounds | Kernel dimensions, scale, volume, counts and region-bound tests |
| Unauthorized edits/rez | Kernel permission/actor/session tests; inherited simulator policy is authoritative |
| Malformed/invalid provider output | Schema unit tests and injected HTTP errors with sanitized 502, no plan |
| Provider unavailable / timeout | Injected HTTP tests; no world execution or paid fallback |
| Mid-structure operation failure | Fake-world injection verifies compensation; concurrent edits are preserved and failed compensation locks writes |
| Interrupted intent / journal write failure | Kernel recovery/write-lock tests; not a deliberately crashed live simulator |

These distinguish unit fault injection from real runtime evidence. Fault tests do not claim a live simulator crash or production outage recovery. Existing API-family checks also cover create/rotate/scale/name/description/color/restricted clone/delete/restoration and idempotent commit.

## Evidence and reproducibility

See [local-development.md](local-development.md), gateway `README.md`, and MutSea `Finalverse/World/README.md` for actual build/run/test/grid/account commands. Public signup is not implemented. Accounts are operator managed through `create user`, `show account`, and `reset user password`; password prompts are hidden. Use Ctrl-P then Ctrl-Q to detach Docker without stopping the world. Credentials never enter Git examples or command arguments.

Private evidence directory: `~/Finalverse/dev/phase2`.

| Evidence | Files |
|---|---|
| Inspection, target refusal, stale rejection | `ui-world-read.png`, `ui-target-unavailable.png`, `ui-stale-rejected.png` |
| Move/History/undo | `ui-move-preview-current.png`, `ui-move-committed.png`, `ui-move-history.png`, `ui-move-undone.png` |
| Invalid/clear site and preview | `ui-home-proposal.json`, `ui-home-clear-156.png`, `ui-home-preview.png` |
| Home committed/visible/history | `ui-home-committed.png`, `ui-home-visible.png`, `ui-home-history.png` |
| Semantic/protocol capture | `ui-acceptance.json`, `ui-capture.log` |
| Restart verification and visible result | `ui-restart-verify-full-transform.log`, `ui-home-post-restart.png`, `ui-home-history-post-restart.png` |
| Final packaged app | `ui-final-package-nearby.png`, `ui-final-package-launch.log` |
| UI undo and visible cleanup | `ui-home-undone.png`, `ui-home-cleanup-visible.png`, `ui-undo-persistence.log` |
| Tests/builds | `kernel-tests-visual.log`, `gateway-tests-visual.log`, `viewer-build-identity.log`, `viewer-package-identity.log`, `world-smoke-build-visual.log`, `viewer-package-source-final.log`, `branding-check-visual-final.log`, `manifest-tests-visual.log` |

Raw logs may contain inherited ephemeral capability URLs and remain private. Screenshot files, receipts, account files, databases, journals, model traces and app bundles are not committed. Gateway tracked-file/ignore checks and the workspace credential guard pass; no credential-bearing remote exists.

Primary viewer remains `contribute` at `cb7e386cd5ec3e14b9bb602ac1dd4a689b094724`, preserving 9,332 status entries and porcelain SHA-256 `3c3986254ed6e78a948e9a4905c879ed9cf1e998e06e404b8437a4f7786d028c`. Deferred Linux work retains 27 changes and hash `4ced83584b82b79c62473b5e03008d362b82f86f5f5000af4af83a4484a5210b`. Primary MutSea remains clean at `13c546ce413e49e957e387547d97e79800cf33ca`.

## Limits and next milestone

Intent/compensation are logical transactions. Simulator SQLite and WorldLine are separate stores, and legacy edits do not share the module lock. Interrupted or uncertain operations block AI writes and need administrator review. Clone/delete/undo support restricted primitives; moving existing objects has no general collision solver. Manual legacy deletion is not reconciled into Lumi's projection. WorldLine retains actor, region scope, proposed/approved plan, provider/model assertions, before/after/observed entities and reversals; it does not retain the full inspection snapshot or a complete cross-service correlation chain. Provider claims remain unsigned.

The loopback planner has no production authentication, TLS, quotas, streaming, retries or cloud adapter. Its provider interface remains vendor neutral and credentials stay out of the client. Inspection/edit scope is 64m; prepared approvals expire after two minutes. Homes are bounded dry-level-ground composites. General furniture/environment generation is not supported.

Lumi has semantic identity, home and creation-request memory, but no avatar/navigation/voice/goals/visitor memory or autonomous runtime. The inherited avatar remains a cloud, and local login shows inherited benefits/home-location warnings. Those limitations do not prevent the scoped world-operation demonstration, but prevent a production-client claim. Native Apple Silicon viewer, Windows/Linux/mobile, public deployment/registration and distribution clearance are unverified or deferred. Renderer, economy and geography were not changed.

All 15 gates pass. Recommend Phase 3 — persistent AI citizens: first reconcile legacy edits and harden recovery, then bind Lumi to an avatar, retain bounded episodic memory, and implement “Lumi, go home” through the same authorized actions with human override. No Phase 3 code was started.
