# Test coverage and product verification matrix

## Existing test architecture

`LL_TESTS` defaults OFF. CMake creates TUT unit targets and test-success stamp commands; integration tests run after linking. CTest registration is commented out. Relevant suites include LLSD/events/UUID/work queues/serialization, vector/quaternion/volume math, message/permissions/inventory, primitive/media/glTF material, HTTP local-peer integration, login/security/grid/settings and viewer helper/map/asset statistics. Some entries are commented out; test source file presence is not an enabled test count.

`llinventory/CMakeLists.txt` has no real project unit-source entries but supplies inventory/parcel integration tests. Viewer unit tests use stubs for complex renderer/world objects. `doc/testplans` supplies manual PBR/terrain/alpha/media plans. None of this proves full world-entry, semantic actions, provider routing or mobile behavior.

## Layered verification

| Level | Required evidence | Phase 0 status |
|---|---|---|
| 0 Compilation/static | Configure/compiler/dependency closure; docs/manifest validation | Configure/full build passed; no static analyzer run |
| 1 Unit | LL_TESTS generated TUT targets | 1,197 reported TUT cases: 1,193 passed, four known skips |
| 2 Subsystem | HTTP, inventory, login/security/grid/math/material tests | Enabled where existing CMake registers them; no blanket coverage claim |
| 3 World protocol | Fixtures/packet/capability tests and compatibility | Existing message/LLSD tests cover portions; no new Finalverse protocol exists |
| 4 Integration | Local-peer suites and real simulator session | Build-driven suites plus new local MutSea account/session, persisted cube move/undo |
| 5 Viewer launch | Actual generated app, UI and log inspection | Produced Intel app launched; login UI/live world observed with CUA |
| 6 Login/world entry | Configured sandbox, account, stream/region/connectivity | Passed local login, Seed caps, inventory startup and STATE_STARTED; terrain/water/sky visible |
| 7 Avatar/object interaction | Movement/chat/inventory/edit/teleport/permissions | Movement + own cube create/move/legacy undo verified; avatar cloud appearance, chat/voice/teleport/full inventory remain gaps |
| 8 AI world operation | Typed proposal -> policy -> legacy write -> observed result -> journal/undo | Not implemented; future MVP gate |
| 9 Multi-platform smoke | Mac/Windows/Linux then native iOS/Android | Only this Mac is available in Phase 0; no multi-platform completion claim |

Runtime stability is an additional unresolved gate. After the successful object tests the viewer became unavailable to window automation, remained alive and did not exit on SIGTERM. The post-SIGTERM process sample is retained privately; it cannot establish the original cause. Next verification needs a reproduced session, evidence collected before termination and a clean shutdown, alongside complete avatar appearance. No chat message or voice exchange was verified.

## Separately executed packaging test

`PYTHONPATH=<clean-source>/indra/lib/python <venv>/bin/python -m unittest discover -s <clean-source>/indra/test -p test_llmanifest.py -v` fails at import: the Windows path literal on line 60 contains a malformed `\N` escape. No packaging test cases executed. This existing standalone file is not the CMake TUT suite. The failure is retained as a baseline test gap, not silently patched to describe an unchanged upstream suite as passing.

## First-action test plan (proposal)

Test real permission denial, stale/deleted entity, region change, expired/wrong-target grant, malformed/NaN/out-of-bound transforms, duplicate action ID, cancellation, timeout/unknown outcome and unauthorized provider response. Integration uses an owned non-physical root object in a sandbox and verifies server-observed transform, visible scene change, readable journal and compensating undo. A concurrent third-party edit must prevent automatic undo. Selection/linkset/UI state and normal manual editing must still work.

Future CI should pin toolchains/dependency hashes and run public open configurations on Mac/Windows/Linux. Mobile CI starts with dependency-isolated contract/policy libraries on arm64, then actual native client smoke tests. Do not enable inherited publishing/signing jobs as a side effect of testing.

Evidence: [LL_TESTS defaults](../../indra/cmake/Variables.cmake), [test commands](../../indra/cmake/LLAddBuildTest.cmake), [legacy suite](../../indra/test/CMakeLists.txt), [viewer suites](../../indra/newview/CMakeLists.txt), [inventory tests](../../indra/llinventory/CMakeLists.txt), [manual plans](../../doc/testplans), [packaging test](../../indra/test/test_llmanifest.py).
