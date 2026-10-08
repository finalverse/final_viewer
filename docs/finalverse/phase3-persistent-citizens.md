# Phase 3: Persistent AI Citizens

The candidate extends the published Phase 2 viewer (`a285e04d5de0515034286e7bc7e2c5cba6676abf`) and MutSea (`0c9e8756d05c2820deeda4c0c0d5bb8c30bb4a22`). It retains authentication, scene streaming, ordinary editing, approved world operations, semantic home metadata and renderer.

## Boundaries

The new independent service is `~/Finalverse/services/agent-runtime`, intended repository name `finalverse/agent-runtime`. Its `docs/finalverse/agents/` is the source-matched reference for architecture, identity, memory, perception, goals, navigation, capabilities, autonomy, WorldLine and validation. The sibling `services/ai-gateway` supplies read-only citizen conversation alongside its existing planner.

The viewer sends human commands through the authenticated MutSea `FinalverseWorld` capability. It contains no service token, model credentials, autonomous loop or persistent citizen database. MutSea derives the human actor and relays to the private runtime; runtime policy and deterministic simulator tools enforce authority. Viewer logout leaves Lumi alive.

Viewer changes are confined to the existing AI floater plus an inherited nearby-chat sorting defect exposed by real citizen speech. The sorting comparator previously sampled changing countdown timers; the fix snapshots timer values before stable sorting, with three regression cases. Copyright/licensing notices remain intact.

## Use

Follow [local-development.md](local-development.md) for actual source paths, Python 3.13 tooling, universal build, package validation, current sandbox/account location and isolated profile. The Phase 3 grid is **MutSeaCitizens**, loopback port 18094; account credentials remain in the private configuration file.

Open Finalverse AI and select **Talk to Lumi**. Ask **Lumi, who are you?**, **Where is your home?**, **What do you remember about your home?**, then **Lumi, go home.** Lumi resolves the real Phase 2 semantic home, moves its linked embodiment through existing streaming and records arrival/memory.

**Lumi status** is the developer inspector. **Pause**, **Resume**, **Cancel goal** remain available during pending reasoning. Ordinary conversation displays concise replies; it does not dump tool coordinates. World creation has a separate preview and explicit **Approve garden** control; **Undo garden** reverses the recorded citizen operation.

## Verification and limits

Native ARM64 execution and universal ARM64/Intel packaging are verified on this Mac. Real identity/home/memory replies and navigation were observed in the viewer. Rust/gateway/world tests, actual capability/UDP smoke tests and separate runtime/simulator/combined restarts supply backend evidence. Exact results and all 24 gate statuses are in the runtime's `phase3-validation.md`.

Lumi is a seven-part primitive citizen entity, not a completed articulated avatar. The navigator is bounded/conservative, not a navmesh. Model conversation is local Ollama `llama3.2:3b`; identity/home/memory/navigation do not require inference. Creation currently uses one approved garden recipe. Future meetings, many agents, voice, public hosting and mobile are unimplemented.

No cloud fallback, service secret in a distributable client, unrestricted simulator API or renderer rewrite is introduced. Current private development services are not claimed to be production deployments.
