# Finalverse brand materials

The client uses the user-supplied silver-and-purple Finalverse mark, existing platform icon container and Lumi artwork from the Finalverse `Images` collection. Original files remain unchanged. The server-side product continues to be **MutSea**; this artwork change applies to the viewer client.

`source/Finalverse_logo_07.svg` preserves the supplied vector source. `asset-manifest.json` records each selected output's source filename/container entry and SHA-256. Platform icons are unpacked from the supplied `finalviewer_icon.icns`; the macOS package uses that original file byte-for-byte. Windows ICO encoding wraps its existing PNG payloads without modifying pixels. The bitmap and login/startup/welcome images retain the supplied data. Normal UI layout scales images while preserving aspect ratio.

The source artwork retains its existing rights and provenance. No new authorship or artwork license is asserted for these supplied materials. Distribution still requires confirmation of their applicable rights. Inherited Linden artwork/notices elsewhere retain their original attribution. The packaging script is copyright 2026 Finalverse contributors, **LGPL-2.1-only**.

To reproduce the selected resources on macOS, from the repository root:

```sh
python3 indra/newview/branding/generate_assets.py --materials '/path/to/Finalverse/Images'
```

`iconutil` is the macOS system format-conversion tool. Generated resources are tracked, so ordinary client builds do not require access to the user's original materials folder. Some output filenames and XUI texture aliases retain `secondlife` or `SL_Logo` to preserve mature packaging interfaces; their selected artwork is Finalverse's.

The bundled welcome page uses these local resources and contains no remote assets or model credentials. It names the inherited viewer separately and does not claim that AI world tools are implemented. This guide and the asset manifest are included in the app's Resources directory alongside inherited notices.
