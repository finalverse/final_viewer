# Mobile architecture and portable-core candidates

No iOS/iPadOS or Android build target, native mobile UI or mobile dependency set exists in the audited viewer commit. Desktop compilation is not a mobile implementation.

## Candidate extraction ranking

| Component | Candidate reuse | Required isolation |
|---|---|---|
| New action/snapshot/schema values | Best first shared core | No legacy pointers, GL, window or vendor SDK |
| LLSD, UUID, typed serialization | Strong existing code candidates | Select files/interfaces; llcommon also includes APR, Boost, process/platform code |
| Permission/asset/inventory value types | Useful adapters / values | llinventory links llmath, llmessage, llxml; avoid importing entire cache/session stack |
| Scalar vectors/quaternions/math algorithms | Useful | llmath requires SSE2 and links other libraries; SIMD abstraction or scalar subset first |
| Message templates and wire serialization | Valuable compatibility reuse | APR, socket/circuit lifecycle, platform and callback dependencies |
| llcorehttp | Potential network reuse | curl/TLS dependencies and llmessage include cycle; mobile lifecycle/transport audit |
| Asset metadata/geometry readers | Potential | llprimitive links llrender, physics packages, Collada and codecs |
| Avatar motions/skeleton | Later useful | llcharacter/llappearance carry network, image, render and filesystem dependencies |
| LLViewerObject/LLWorld | Projection only | Graphics/UI/global state: retain behind desktop adapter |
| llui/llwindow/CEF/media processes | Desktop boundary | Native mobile UI, touch input, lifecycle, sandbox and embedded media evaluation |

These rankings are extraction proposals, not portable builds proved by tests. Evidence: the corresponding [CMake targets](../../indra/CMakeLists.txt), [math/SSE](../../indra/llmath/llsimdmath.h), [primitive dependencies](../../indra/llprimitive/CMakeLists.txt), [HTTP dependencies](../../indra/llcorehttp/CMakeLists.txt), [window platforms](../../indra/llwindow/CMakeLists.txt).

## Critical blockers

Desktop OpenGL context/UI/font rendering assumptions reach into scene/object and appearance code. iOS cannot inherit the Cocoa desktop window/plugin process arrangement; Android cannot inherit X11/SDL1 platform dependencies. Both require ARM-safe math, mobile asset/voice/codec distributions, battery/memory budgets, app lifecycle reconnect, secure credential storage, touch camera/movement and app packaging. The manifest has no mobile prebuilt dependency closure. macOS ARM portability is not evidence of iOS support.

## Proposed sequence

Desktop WorldAction and snapshot contracts become the first shared portable artifacts. Build a dependency-isolated C++ library on arm64 with meaningful schema/policy tests before extracting legacy runtime code. Use C++ initially; compare Rust only when ownership/concurrency/service needs justify an FFI cost. A narrow C ABI with opaque handles or a world protocol is appropriate for Swift/Kotlin; do not expose STL types across boundaries.

For iOS/iPadOS, evaluate Swift/SwiftUI shell and Metal presentation independently. For Android, evaluate Kotlin/Compose shell and Vulkan independently. Start a mobile read-only world/session prototype only after the desktop mutation MVP and protocol contracts stabilize. Budget rendering, avatar appearance, streaming, voice and session continuity as real product work; a standalone static 3D scene is not a persistent-world client.

No Blender runtime dependency is introduced. A future Blender add-on talks to the authenticated World API and imports/exports permitted assets through glTF first; OpenUSD requires a demonstrated interchange need.
