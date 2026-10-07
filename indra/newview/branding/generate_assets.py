#!/usr/bin/env python3
"""Package supplied Finalverse artwork without changing its pixels.

Copyright 2026 Finalverse contributors. SPDX-License-Identifier: LGPL-2.1-only
Artwork ownership/licensing is separate; see README.md and asset-manifest.json.
Requires macOS iconutil to unpack the supplied platform icon container.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent.parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--materials", type=Path, required=True)
    args = parser.parse_args()
    materials = args.materials.resolve()
    records = []

    def copy(source, relative, origin):
        target = ROOT / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
        records.append({"path": relative, "source": origin,
                        "sha256": hashlib.sha256(target.read_bytes()).hexdigest()})

    icons = "icons/finalverse"
    textures = "skins/default/textures/finalverse"
    copy(materials / "finalviewer_icon.icns", icons + "/secondlife.icns", "finalviewer_icon.icns")
    copy(materials / "Finalverse_logo_07.svg", "branding/source/Finalverse_logo_07.svg", "Finalverse_logo_07.svg")
    for name in ("login.png", "login-small.png", "mark.png"):
        copy(materials / "Finalverse_logo_07@3x.png", textures + "/" + name, "Finalverse_logo_07@3x.png")
    copy(materials / "Lumi-logo.png", textures + "/lumi.png", "Lumi-logo.png")
    copy(materials / "finalviewer_256.bmp", icons + "/secondlife_256.BMP", "finalviewer_256.bmp")
    for name in ("install_icon", "uninstall_icon"):
        copy(materials / "finalviewer_256.bmp", f"installers/windows/{name}.BMP", "finalviewer_256.bmp")

    with tempfile.TemporaryDirectory(prefix="finalverse-icons-") as temporary:
        iconset = Path(temporary) / "finalverse.iconset"
        subprocess.run(["iconutil", "-c", "iconset", str(materials / "finalviewer_icon.icns"),
                        "-o", str(iconset)], check=True)
        for file in sorted(iconset.glob("*.png")):
            copy(file, icons + "/finalverse.iconset/" + file.name,
                 "finalviewer_icon.icns:" + file.name)
        for size in (16, 32, 128, 256, 512):
            copy(iconset / f"icon_{size}x{size}.png", icons + f"/secondlife_{size}.png",
                 f"finalviewer_icon.icns:icon_{size}x{size}.png")
        copy(iconset / "icon_512x512@2x.png", icons + "/secondlife_1024.png", "finalviewer_icon.icns:icon_512x512@2x.png")
        copy(materials / "finalviewer_48.png", icons + "/secondlife_48.png", "finalviewer_48.png")
        copy(iconset / "icon_512x512@2x.png", textures + "/startup.png", "finalviewer_icon.icns:icon_512x512@2x.png")
        copy(iconset / "icon_32x32.png", textures + "/notification.png", "finalviewer_icon.icns:icon_32x32.png")

        # ICO container encoding only: the supplied PNG payloads are unchanged.
        payloads = [(size, (iconset / f"icon_{size}x{size}.png").read_bytes())
                    for size in (16, 32, 128, 256)]
        offset = 6 + 16 * len(payloads)
        directory, data = [], []
        for size, payload in payloads:
            directory.append(struct.pack("<BBBBHHII", size % 256, size % 256, 0, 0,
                                         1, 32, len(payload), offset))
            data.append(payload)
            offset += len(payload)
        ico = struct.pack("<HHH", 0, 1, len(payloads)) + b"".join(directory + data)
        target = ROOT / icons / "secondlife.ico"
        target.write_bytes(ico)
        records.append({"path": icons + "/secondlife.ico", "source": "finalviewer_icon.icns:PNG payloads in ICO container",
                        "sha256": hashlib.sha256(ico).hexdigest()})
        for name in ("install_icon", "uninstall_icon"):
            copy(target, f"installers/windows/{name}.ico", "finalviewer_icon.icns:PNG payloads in ICO container")

    manifest = {"origin": "User-supplied Finalverse/Images brand materials",
                "pixel_changes": False, "license": "Existing asset rights; no new artwork license asserted",
                "assets": records}
    (ROOT / "branding/asset-manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")


if __name__ == "__main__":
    main()
