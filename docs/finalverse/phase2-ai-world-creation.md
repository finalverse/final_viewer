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
