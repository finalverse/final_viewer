# Rendering architecture and modernization gates

## Current source

`LLPipeline` in `indra/newview/pipeline.*` orchestrates scene culling, draw pools, deferred geometry/lighting, shadows, reflection maps and post-deferred work. `LLDrawable`, spatial partitions/groups and volume/avatar geometry bind viewer objects to rendering. `LLViewerShaderMgr` and GLSL files under `app_settings/shaders` provide shader management. `llrender` supplies GL state, buffers, textures, FBO/render targets, fonts and shader programs. `llwindow` owns platform GL contexts; `llui` drawing is also coupled to these facilities.

PBR material and glTF paths exist (`LLGLTFMaterial`, `LLDrawPoolGLTFPBR`, `newview/gltf`). A `VulkanGltf.cmake` dependency name or OpenXR package is not evidence that this desktop world renderer is Vulkan, Metal or XR-ready. `FindOpenGL` and the GL context sources are the actual backend evidence.

Sources: [pipeline](../../indra/newview/pipeline.h), [renderer](../../indra/llrender/CMakeLists.txt), [shader manager](../../indra/newview/llviewershadermgr.cpp), [OpenGL import](../../indra/cmake/OpenGL.cmake), [Mac context](../../indra/llwindow/llopenglview-objc.mm), [glTF scope](../../indra/newview/gltf/README.md).

## Preserve now

Phase 0 and the AI MVP make no renderer changes. Wrap world operations and take snapshots without changing drawable ownership. Use the existing performance instrumentation and manual test plans as baseline evidence; do not equate a successful shader compile with visually correct materials or animation.

## Later gates

1. Preserve OpenGL and measure real scenes: streaming stalls, CPU frame work, GPU time, upload budgets, memory and UI.
2. Isolate renderer handles from semantic snapshots/action schemas; document GL context/thread assumptions at touched boundaries.
3. Introduce narrowly justified presentation interfaces, then resource/upload/frame boundaries with explicit lifetimes.
4. Evaluate Metal/Vulkan/D3D12/WebGPU only with representative avatars, alpha, terrain, PBR, media, UI and shader regression fixtures.
5. Roll out a backend only with visual parity, measurable benefit, platform dependency closure and rollback.

Do not mandate one desktop backend for mobile. OpenGL deprecation on macOS creates a long-term product risk, but it does not justify blocking AI integration on a renderer rewrite. No schedule estimate is credible until representative scene measurements and resource lifetime audit exist.

Existing manual regression assets: [PBR materials](../../doc/testplans/pbr_materials.md), [terrain appearance](../../doc/testplans/pbr_terrain_appearance.md), [terrain loading](../../doc/testplans/terrain_loading.md), [alpha optimization](../../doc/testplans/optimize_away_alpha.md).
