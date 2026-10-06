# Desktop architecture

## Existing platform boundary

The same C++ viewer runtime is built for desktop platforms. `LLAppViewer` subclasses and `LLWindow` supply platform application/window/input behavior. macOS uses Cocoa and Objective-C++ OpenGL view code; Windows uses Win32, drag/drop and hardware probing; Linux uses SDL/X11 and platform integration. `llui` widgets and XUI XML skins are shared desktop UI. `LLPipeline` and `llrender` remain OpenGL.

Sources: [window build](../../indra/llwindow/CMakeLists.txt), [Mac view](../../indra/llwindow/llopenglview-objc.mm), [Windows window](../../indra/llwindow/llwindowwin32.cpp), [Linux window](../../indra/llwindow/llwindowsdl.cpp), [application](../../indra/newview/llappviewer.cpp), [UI](../../indra/llui/CMakeLists.txt).

## Existing support versus intent

The Autobuild manifest has `darwin64`, `windows64` dependencies and `linux64`; CMake architecture selection maps address size 64 to x86_64. Its SIMD math includes x86 intrinsic headers and requires SSE2. Native Apple Silicon, Windows ARM64 and Linux ARM64 are not established targets of this commit. Running an Intel executable under Rosetta is a compatibility baseline, not native ARM support.

Preserve Windows/macOS build pathways and Linux sources. Add desktop features inside existing frame/main-thread rules. Network/provider workers receive copied values; object edits, selection changes and UI callbacks are serialized onto the viewer thread. Shutdown/disconnect cancels async tasks and invalidates session-bound snapshots/capabilities.

## Minimal product identity seam (Phase 1 proposal)

Use existing `VIEWER_CHANNEL`, bundle version/Info.plist machinery and a small set of product-owned assets/settings. Review About/provenance text and grid/login/update/telemetry/help endpoints separately. Retain license headers and wire-protocol names. No mass string replacement or binary-class rename is justified.

Before distribution, isolate user settings/cache/log paths and updater identity so a development Finalverse build does not unexpectedly overwrite another viewer's profile. Do not redirect authentication endpoints simply to change product branding. Keep the native renderer and mature input/editor behaviors.

See [build-matrix.md](build-matrix.md) for evidence-based verification status and [licensing-audit.md](licensing-audit.md) for provenance/endpoint risks.
