# Upstream synchronization

Latest report: [2026-10-08 synchronization and validation](upstream-sync-20261008.md).

The product branch is `main`; `contribute` tracks the direct parent `qwy16/secondlife_viewer`. Preserve both histories and fetch the parent before integrating local changes. Review original `secondlife/viewer` ancestry through the parent. Do not enable automatic upstream merging.

## Future merge watch list

- `indra/newview/llstartup.cpp`, `llviewerwindow.cpp`, `llpanelLogin.cpp`, `llfloateravatarwelcomepack.cpp`: current-grid routing, safe local welcome, new login controls and listener ownership.
- `indra/newview/llappviewer.*`, macOS XIB and platform window implementations: independent window identity, marker/settings/log/cache namespaces and startup behavior.
- `indra/newview/llviewerregion.cpp`, `llfloaterfinalverseai.*`, AI XUI/menu: preserve `FinalverseWorld` beside inherited capabilities, approved structured operations, stale guards and UI resources.
- `indra/llprimitive/llprimtexturelist.cpp`, `llgltfmaterial.cpp` and real fixtures: copy-before-delete self-alias safety, upstream null behavior, initialized padding/hash semantics.
- `indra/llcorehttp/_httplibcurl.cpp`: guarded Linux forced-unwind rethrow; validate on Linux separately.
- `indra/cmake/Python.cmake`, `autobuild.xml`, `viewer_manifest.py`, workflows and installers: locked Python, architecture/dependency floors, updater/profile/protocol ownership and legal attribution.
- `scripts/finalverse/verify_branding.py` and supplied-asset provenance: recheck both source and freshly packaged app after upstream changes.

Dated reports are evidence for their recorded revisions. A later compile does not inherit their runtime acceptance results.
