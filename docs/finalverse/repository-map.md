# Repository and component map

This audit covers the direct Second Life-derived `finalverse/final_viewer` fork. It is distinct from the Rust workspace at `~/ClaudeProjects/finalverse`; that workspace has no Git metadata and is not a viewer baseline. No code or architecture from that workspace is assumed to work here.

## Repository-wide inventory

There are 9,412 tracked paths at the starting commit: 1,329 `.cpp`, 1,338 `.h`, 8 `.mm`, 219 `.glsl`, 5,145 `.xml`, 78 `.cmake` and 36 `.py` files. [source-inventory.json](source-inventory.json) contains all tracked paths and counts. The inventory is repository-wide; subsystem conclusions below come from focused source inspection rather than a claim of reviewing every line.

| Location | Existing responsibility | Preservation / seam |
|---|---|---|
| `indra/newview` | Application, world state, UI integration, networking orchestration, rendering pipeline, avatars, edit tools | Keep runtime; add one adapter |
| `indra/llcommon` | LLSD, UUID, events, coroutines, work queues, logging, threads/processes | Reuse values/events selectively |
| `indra/llmath` | Transform, quaternion, vector, volume, octree, ray tests | Useful math; whole library requires SSE2 |
| `indra/llmessage` | UDP circuits, message templates, serialization, transfers, HTTP helpers | Preserve simulator protocol |
| `indra/llcorehttp` | curl-based async HTTP, queue, policy and response machinery | Reuse network lifecycle for client bridge |
| `indra/viewer_components/login` | Coroutine and event-driven login | Preserve authentication |
| `indra/llinventory` | Inventory values, permissions, parcels, environment settings | Separate data from viewer cache and network |
| `indra/llfilesystem` | Platform paths, disk cache, file access | Keep desktop; storage interface for mobile |
| `indra/llprimitive` | Primitive, mesh loaders, texture entries, glTF materials | Not portable wholesale: links llrender/physics |
| `indra/llcharacter` | Skeleton, joints, motions, gestures, state machines | Preserve mature avatar behavior |
| `indra/llappearance` | Wearables, morphs, avatar appearance and baking inputs | Rendering/network ties limit portability |
| `indra/llimage`, `llimagej2coj`, `llkdu` | Codecs and JPEG2000 alternatives | Preserve; prefer open path in baseline |
| `indra/llrender` | OpenGL buffers, textures, shaders, render targets, fonts | Preserve renderer |
| `indra/llwindow` | Win32, Cocoa/Objective-C++, Linux SDL, input and GL contexts | Desktop-only boundary |
| `indra/llui`, `llxml` | Widgets, XML/XUI, controls, settings | Existing desktop AI floater host |
| `indra/llaudio`, `llwebrtc` | Audio and WebRTC integration | Preserve voice; model speech is a separate feature |
| `indra/llplugin`, `media_plugins` | Isolated plugin processes, CEF/Dullahan, LibVLC | Preserve desktop media; mobile replacement evaluated later |
| `indra/llmeshoptimizer` | Geometry optimizer wrapper | Candidate behind an asset interface |
| `indra/test`, `integration_tests` | TUT suites and image utility integration | Enable LL_TESTS explicitly |
| `indra/*crash_logger` | Platform crash handling | Release endpoint/consent audit |
| `indra/cmake` | Dependency imports, platform flags, tests, packaging | Use pinned baseline before editing |
| `scripts` | Message template, verification, asset tools, metrics, performance bots | Maintain protocol evidence |
| `doc` | Source/artwork licenses, contributors, manual rendering test plans | Retain notices and regression plans |
| `.github/workflows` | Build/sign/release and fork synchronization | Fork-specific review before CI activation |
| `autobuild.xml`, `build.sh` | Prebuilt manifest and CI-oriented build orchestration | Existing build infrastructure |

## Local alternatives

| Checkout under `~/FinalProjects/legacy` | Branch / starting commit | Provenance |
|---|---|---|
| `final_viewer` (audit target) | `contribute` / `218de297a4a2cc286aba54e5df40e5e9e69caf43` | origin `finalverse/final_viewer`, upstream `qwy16/secondlife_viewer` |
| `finalviewer` | `master` / `26e18e6978ce5c6f47e2d37d24ccc5a02425ff40` | origin `finalverse/finalviewer`, upstream `qwy16/firestorm` |
| `fv-metal` | `master` / `19accfd672baa4ef091a267d9905b44c2c292fbe` | Firestorm-derived alternative, origin `finalverse/finalviewer` |

The latter two have one gitlink (`fs-build-variables`) and file-mode differences; their name or an old build directory is not evidence of working Metal rendering or a verified current build. No cross-fork merge or deletion was performed.
