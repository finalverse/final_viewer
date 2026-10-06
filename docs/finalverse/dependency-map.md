# Dependency map

Manifest evidence: [autobuild.xml](../../autobuild.xml) at `218de297a4a2cc286aba54e5df40e5e9e69caf43`. [dependency-manifest.json](dependency-manifest.json) preserves all installables/platform configurations and archive metadata. This is a declared manifest, not an actual shipped binary SBOM.

## Major dependency paths

`llcorehttp -> curl/OpenSSL/nghttp2 -> async HTTP`; `llmessage -> llcommon/llmath/llcorehttp -> UDP/capabilities`; `llprimitive -> llrender/llcharacter/physics/Collada/glTF -> scene geometry`; `llappearance -> llrender/llimage/inventory/character -> avatars`; `llwindow/llui -> rendering/fonts/platform UI`; `newview -> all of those + plugin/media/voice`. CMake contains cycles and conditional imports; this is a responsibility map, not a fully resolved linker graph.

## Declared installables

| Package | Version | License label | Platforms |
|---|---|---|---|
| SDL | 1.2.15 | lgpl | linux64 |
| apr_suite | 1.7.4-10338381102 | apache | darwin64, linux64, windows64 |
| boost | 1.86 | boost 1.0 | darwin64, linux64, windows64 |
| bugsplat | 5.0.1.0-71fc41e | Proprietary | darwin64, windows64 |
| colladadom | 2.3.0-r8 | SCEA | darwin64, linux64, windows64 |
| cubemaptoequirectangular | 1.1.0 | MIT | darwin64, linux64, windows64 |
| curl | 7.54.1-10342910827 | curl | darwin64, linux64, windows64 |
| dbus_glib | 0.76 | Academic Free License v. 2.1 | linux64 |
| dictionaries | 1.a01bb6c | various open source | common |
| dullahan | 1.14.0.202408091639_118.4.1_g3dd6078_chromium-118.0.5993.54 | MPL | darwin64, linux64, windows64 |
| emoji_shortcodes | 15.3.2.10207138275 | MIT | common |
| expat | 2.6.2-r5 | expat | darwin64, linux64, windows64 |
| fontconfig | 2.11.0 | bsd | linux64 |
| freetype | 2.13.3-cb2e120 | FreeType | darwin64, linux64, windows64 |
| glext | 68 | Copyright (c) 2007-2010 The Khronos Group Inc. | common |
| glh_linear | 1.0.1-dev4 | BSD | common |
| glm | v1.0.1 | MIT | common |
| gstreamer | 0.10.6.314267 | LGPL | linux64 |
| gtk-atk-pango-glib | 0.1 | lgpl | linux64 |
| havok-source | 2012.1-2 | havok | darwin64, linux64, windows64 |
| jpegencoderbasic | 1.0 | NONE | darwin64, linux64, windows64 |
| libjpeg-turbo | 3.0.3-r2 | libjpeg-turbo | windows64, linux64, darwin64 |
| kdu | 7.10.4.4b9ec5f | Kakadu | darwin64, linux64, windows64, linux |
| libhunspell | 1.7.2.10207243663 | LGPL | darwin64, linux64, windows64 |
| libndofdev | 0.1.8e9edc7 | BSD | darwin64, windows64 |
| libpng | 1.6.43-r2 | libpng | darwin64, linux64, windows64 |
| libuuid | 1.6.2 | UUID | linux64 |
| libxml2 | 2.13.3-r1 | mit | darwin64, linux64, windows64 |
| llappearance_utility | 0.0.1 | Proprietary | linux |
| llca | 202407221423.0 | mit | common |
| llphysicsextensions_source | 1.0.66e6919 | internal | darwin64, linux64, windows64 |
| llphysicsextensions_stub | 1.0.542456 | internal | darwin64, linux64, windows64 |
| llphysicsextensions_tpv | 1.0.561752 | internal | darwin64, linux64, windows64 |
| mesa | 7.11.1.297294 | mesa | linux |
| meshoptimizer | 210.0.0-r2 | meshoptimizer | darwin64, windows64, linux64 |
| mikktspace | 1 | Apache 2.0 | darwin64, linux64, windows64 |
| minizip-ng | 4.0.7-r1 | minizip-ng | darwin64, linux64, windows64 |
| nanosvg | 2022.09.27 | Zlib | darwin64, linux, windows64 |
| nghttp2 | 1.62.1 | MIT | darwin64, linux64, windows64 |
| nvapi | 560.0.0-r1 | MIT | windows64 |
| ogg_vorbis | 1.3.5-1.3.7.10341271136 | ogg-vorbis | darwin64, linux64, windows64 |
| open-libndofdev | 0.3 | BSD |  |
| openal | 1.23.1 | LGPL2 | darwin64, linux64, windows64 |
| openjpeg | 2.5.0.ea12248 | BSD | darwin64, linux64, windows64 |
| openssl | 1.1.1w | openssl | darwin64, linux64, windows64 |
| openxr | 1.1.40-r1 | Apache 2.0 | windows64, linux64, darwin64 |
| slvoice | 4.10.0000.32327.5fc3fe7c.5942f08 | Mixed | darwin64, linux64, windows64 |
| threejs | 0.132.2 | MIT | darwin64, linux64, windows64 |
| tinygltf | 2.9.3-r1 | MIT | common |
| tracy | v0.11.0.10376230034 | bsd | darwin64, windows64, linux64 |
| tut | 2008.11.30 | bsd | common |
| viewer-fonts | 1.0.0.10204976553 | Various open source | common |
| viewer-manager | 3.0-f14b5ec | viewerlgpl | darwin64, linux64, windows64 |
| vlc-bin | 3.0.21.10218721728 | GPL2 | darwin64, windows64 |
| vulkan_gltf | 1.0.0 | Copyright (c) 2018 Sascha Willems | common |
| webrtc | m114.5735.08.72-test.10444682919 | MIT | darwin64, linux64, windows64 |
| xxhash | 0.8.2-10285735820 | xxhash | common |
| zlib-ng | 2.2.1-r2 | zlib-ng | darwin64, linux64, windows64 |
| tinyexr | 1.0.9-5e8947c | 3-clause BSD | common |

## Constraints

The manifest has no iOS/Android or Windows/Linux ARM closure. CEF/Dullahan, SLVoice/Vivox, WebRTC, codecs, platform fonts and physics extensions are significant platform/distribution boundaries. Common-only entries provide headers/assets, not proof of architecture-independent execution. Dependencies with old version labels (for example curl, OpenSSL and Chromium) require an artifact/security audit before distribution; no vulnerability scan or safety claim is made here.

Optional KDU/Havok/BugSplat are not necessary for an open source baseline. `llphysicsextensions_stub` is still a separately fetched dependency. Fetched archive availability and hash verification are part of reproducibility, not a reason to silently replace mature systems.
