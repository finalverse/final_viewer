# Build and desktop platform matrix

Source commit: `218de297a4a2cc286aba54e5df40e5e9e69caf43`. Evidence distinguishes manifest/source support from verified builds.

| Platform | Existing source/manifest path | Phase 0 result / blocker |
|---|---|---|
| macOS Apple Silicon host, Intel binary | darwin64 / Xcode / SSE2, Rosetta runtime | Clean configure/full build passed; launch and local world entry passed; avatar appearance and later unresponsiveness remain gaps |
| macOS Apple Silicon native arm64 | No native architecture selection or ARM SIMD path at this commit | Not built; SSE2 intrinsics and dependency architecture closure block native path |
| macOS Intel x86_64 | Existing darwin64 target | Intel binary built/run on ARM host via Rosetta; actual Intel-hardware runtime not tested |
| Windows x86_64 | Windows64 packages / VS/MSBuild / Win32 | Source path present; not built on this Mac |
| Windows ARM64 | No manifest closure/architecture path | Not implemented or tested |
| Linux x86_64 | linux64 / Ninja / SDL-X11 source | Source path present; inherited default disables tests; not built here |
| Linux ARM64 | No dependency closure; SSE2 math | Not implemented or tested |
| iOS/iPadOS | No viewer target or mobile prebuilts | Not implemented; desktop Cocoa/GL/UI/plugins and ARM blockers |
| Android | No viewer target or mobile prebuilts | Not implemented; desktop GL/SDL/UI/plugins and ARM blockers |
| Browser/XR | OpenXR dependency alone is insufficient | No production browser/XR client proved by this audit |

Host observed: macOS 26.6.2, arm64, 16 GiB RAM, 8 CPUs; Xcode 26.3 (17C529), AppleClang 17.0.0, SDK 26.2, CMake 4.3.1, Ninja 1.13.2, Python 3.13.6. Shell PATH initially preferred Homebrew Clang 22.1.8; baseline explicitly selects Apple compiler. Autobuild 3.10.2/llsd 1.2.4/llbase 1.3.1 are installed only in an isolated temporary virtual environment.

The external flag file declares macOS minimum version 11 and `-std=c++17` while root CMake selects C++20; actual compiler arguments/toolchain are part of the baseline evidence. Native ARM support must be an explicit build-enablement effort, not an architecture claim inferred from the host.

Source: [Variables](../../indra/cmake/Variables.cmake), [SIMD math](../../indra/llmath/llsimdmath.h), [common flags](../../indra/cmake/00-Common.cmake), [Autobuild configurations](dependency-manifest.json), [workflow](../../.github/workflows/build.yaml).
