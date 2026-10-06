# Finalverse Phase 0 audit

Audit date: 2026-10-07. Baseline repository: `finalverse/final_viewer`, branch `contribute`, commit `218de297a4a2cc286aba54e5df40e5e9e69caf43`. Canonical local location: `/Users/wenyan/Finalverse/viewer`.

This is repository archaeology, architecture and baseline verification. No Finalverse features, mass rebranding, renderer rewrite or cloud provider integration were implemented. Existing user changes were isolated from the build and preserved. Proposed module names and future behavior are explicitly distinguished from the inherited runtime.

## Requested findings

| # | Finding | Evidence and report |
|---|---|---|
| 1 | Exact Git state, starting revision and existing user work | [git-state.json](git-state.json), [baseline](upstream-baseline.md) |
| 2 | Complete tracked-path inventory and component map | [repository map](repository-map.md), [source inventory](source-inventory.json) |
| 3 | External dependencies and internal coupling | [dependency map](dependency-map.md), [manifest](dependency-manifest.json), [CMake link declarations](cmake-link-map.json) |
| 4 | Autobuild/CMake/platform packaging and CI | [build system](build-system.md) |
| 5 | Desktop platform support versus verified builds | [build matrix](build-matrix.md), [desktop architecture](desktop-architecture.md) |
| 6 | OpenGL pipeline, draw pools, resources and migration constraints | [renderer architecture](renderer-modernization.md) |
| 7 | Login, reliable UDP, HTTP and regional capabilities | [architecture](architecture.md#networking-architecture) |
| 8 | Streamed scene/object identity, update and editing paths | [architecture](architecture.md#scene-and-object-architecture), [World API](world-api.md) |
| 9 | Avatar appearance/animation/own-agent movement | [architecture](architecture.md#avatar-architecture) |
| 10 | Inventory identity, permissions, asset and texture/mesh streaming | [architecture](architecture.md#inventory-and-assets) |
| 11 | LLUI/XUI floaters, settings and input | [architecture](architecture.md#ui-architecture) |
| 12 | Realistic portable candidates and dependency boundaries | [mobile architecture](mobile-architecture.md) |
| 13 | ARM, mobile UI/window/GL/plugin/network blockers | [mobile architecture](mobile-architecture.md), [build matrix](build-matrix.md) |
| 14 | Existing structured events, snapshot/query and deterministic edit seams | [AI architecture](ai-architecture.md), [World API](world-api.md) |
| 15 | Upstream preservation and additive Finalverse modules | [architecture](architecture.md#proposed-additive-boundaries), [migration plan](migration-plan.md) |
| 16 | Source provenance, libraries, artwork, trademarks and service risks | [licensing audit](licensing-audit.md) |
| 17 | Existing test coverage and actual verification gaps | [test matrix](test-matrix.md), [baseline](upstream-baseline.md) |
| 18 | Contracts/core/adapter/UI/brain/journal boundaries | [architecture](architecture.md#proposed-additive-boundaries), [semantic model](semantic-world-model.md) |
| 19 | Phases 0–13 with evidence gates | [migration plan](migration-plan.md), [roadmap](roadmap.md) |
| 20 | First implementation recommendation and acceptance criteria | [roadmap](roadmap.md), [decision log](decision-log.md) |

## Architectural conclusions

The fork is a mature simulator-backed desktop runtime, not a portable world kernel. Its scene is a streamed local replica; the simulator retains persistent authority. Networking, assets, avatars, inventory, UI and rendering should be preserved. Existing `LLAgentListener` exposes nearby object/agent queries, own-avatar autopilot and other structured operations. Editing already routes through permission/selection-aware viewer tools and simulator messages. Wrap those paths after validating explicit UUID targets; do not mutate local scene objects or treat arbitrary event APIs as an AI sandbox.

Add value-only contracts, a semantic sidecar and one main-thread viewer adapter. AI emits bounded proposals; trusted policy/capability code validates them; the executor uses mature protocol paths; observed server updates determine outcomes; an operation journal records provenance. A viewer packet send is not a server transaction commit. Undo requires a new compensating action and concurrency checks.

## Verified baseline

The clean macOS open build passed: 1,193 C++ test cases passed and four known skips were reported across 1,197 cases. The produced Intel app ran via Rosetta, entered the local MutSea world, moved the avatar, created/moved an owned cube and restored it with existing undo; simulator persistence matched the changes. A standalone Python packaging test fails at import, the local avatar remains a cloud, and the viewer later became unresponsive and required forced cleanup. These are recorded gaps, not an all-functionality-pass claim. See [the baseline](upstream-baseline.md) and [machine evidence](verification.json).

## Critical risks

The current build is an Intel target on Apple Silicon; SSE2 math and dependency archives block an assumed native ARM/mobile build. Runtime reliability remains open: the viewer stopped responding after the object tests and did not exit on SIGTERM. A process sample taken after that signal cannot establish the original cause. The fork's source labels and dependency age do not establish binary security or distribution clearance. The hourly upstream-sync workflow can merge/push `main` and conflicts with controlled product releases. A clean detached checkout is required because the original checkout has 9,330 mode changes, an edited `.gitignore` and untracked build variables. Build tests do not prove all world behavior; see the measured baseline for remaining gates.

## Recommended Phase 1

First fix the packaging test, investigate avatar appearance/grid compatibility and reproduce/resolve the runtime unresponsiveness. Lock the measured baseline, then add a distinct Finalverse product channel/profile identity with inherited attribution preserved. Inventory branding/endpoints rather than replacing strings broadly. Add a read-only world-inspection adapter and AI floater as the next architectural seam. The first mutation milestone is one explicitly authorized bounded move of an owned non-physical root object, verified against the simulator and journaled with a guarded compensating undo. Delete, script execution, purchases, multi-object transactions and renderer/mobile rewrites are later gates.

Rebranding alone is not the AI-native MVP. Its acceptance test remains live world entry, structured nearby-object information, typed action, policy, visible authoritative change, inspectable operation record and undo where feasible.

## Verification and workspace organization

[upstream-baseline.md](upstream-baseline.md) records commands, final results, runtime checks and limitations. [baseline-macos.sh](baseline-macos.sh) reproduces the open configuration with an isolated clean source tree and pinned tooling/flags. [workspace-organization.md](workspace-organization.md) records the user's additional move to `~/Finalverse`; original paths remain compatibility links and projects retain separate histories.
