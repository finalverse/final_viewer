# Roadmap and recommended next milestone

The product direction is an AI-native persistent world client using the mature viewer as bootstrap runtime. The audited source supplies streaming, avatars, camera, chat/voice, editing, inventory, materials and networking; it does not supply a Finalverse brain, policy kernel, semantic sidecar or action journal.

## Critical path

1. Close the verified baseline build/test/run gate described in `upstream-baseline.md`.
2. Phase 1: controlled product identity, artwork, packaging, attribution and separate settings/cache identity; preserve world compatibility. See [the implemented branding milestone](rebranding.md).
3. Phase 2: C++ value/action contracts, permission and capability policy, immutable snapshot adapter; keep all renderer/UI pointers out.
4. Phase 3: one LLUI AI floater and neutral server/local provider bridge, read-only nearby objects first.
5. Phase 4: one bounded owned-object move with observed result, readable operation record and conflict-aware undo.

The recommended first implementation milestone is **baseline repair followed by controlled identity plus a read-only WorldSnapshot adapter**. The measured repair backlog is the Python packaging-test import failure, local avatar cloud appearance and later viewer unresponsiveness/failed SIGTERM shutdown. Reproduce the runtime issue with evidence gathered before termination; its original cause is not established. Do not implement mutations until world-entry behavior and query freshness/coverage are verified. The first mutation then proves the full model-proposal/policy/executor/WorldLine path.

The subsequent Phase 1 work repairs the eight-case packaging suite and implements product identity. The next architectural milestone remains a read-only snapshot adapter and neutral AI shell after the runtime reliability/appearance gaps are addressed. Branding does not satisfy the AI-native MVP.

## Work to defer

Renderer replacement, general world scripting, multi-object atomic claims, destructive deletion, NPC autonomy, Blender runtime embedding and desktop UI compilation for phones. Studio and native mobile UI remain separate surfaces connected by contracts.

## Evidence for choosing this path

`LLAgentListener::getNearbyObjectsList` already supplies structured UUID/region/position data; `LLSelectMgr::sendMultipleUpdate` supplies existing persistent object transform messaging. Wrapping those paths avoids duplicating the world runtime. They need scope/freshness/permission/result handling, not a new renderer. Mobile extraction begins with these new values/policies, since existing llmath, primitive, appearance and window dependencies are not already a mobile shared core.

No calendar estimates are assigned until dependency availability and runtime baseline are verified. [migration-plan.md](migration-plan.md) contains the phase gates; [test-matrix.md](test-matrix.md) defines evidence required for product claims.
