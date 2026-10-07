#!/usr/bin/env python3
"""Check packaged identity and installer isolation without installing anything.

Copyright 2026 Finalverse contributors. SPDX-License-Identifier: LGPL-2.1-only
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import plistlib
import re
import shlex
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[2]
VIEWER = ROOT / "indra/newview"


def require(condition, description):
    if not condition:
        raise RuntimeError(description)


def verify(bundle):
    checks = []
    # Every translated product name must resolve to the same independent client.
    locales = []
    for file in (VIEWER / "skins/default/xui").glob("*/strings.xml"):
        tree = ET.parse(file)
        names = {e.get("name"): (e.text or "").strip() for e in tree.getroot()}
        if "APP_NAME" in names:
            require(names["APP_NAME"] == "Finalverse", f"Wrong product name in {file}")
            locales.append(file.parent.name)
    for file in subprocess.check_output(
        ["git", "diff", "--name-only", "aec8eaf022e0b67b415727cf512d62c396132811"],
        cwd=ROOT, text=True,
    ).splitlines():
        if file.endswith(".xml"):
            ET.parse(ROOT / file)
    checks.append("product strings and modified XML")
    assets = json.loads((VIEWER / "branding/asset-manifest.json").read_text())
    for record in assets["assets"]:
        require(hashlib.sha256((VIEWER / record["path"]).read_bytes()).hexdigest() == record["sha256"],
                "Supplied branding resource differs from its provenance record")
    checks.append("supplied artwork provenance hashes")

    # Actual shell output includes paths with spaces; no HOME override is used.
    with tempfile.TemporaryDirectory(prefix="finalverse-branding-") as temporary:
        fixture = Path(temporary) / "install with spaces"
        etc = fixture / "etc"
        etc.mkdir(parents=True)
        source = VIEWER / "linux_tools/refresh_desktop_app_entry.sh"
        script = etc / source.name
        shutil.copy2(source, script)
        destination = Path(temporary) / "desktop entries"
        subprocess.run(["bash", str(script), "--output-dir", str(destination)], check=True, capture_output=True)
        desktop = (destination / "finalverse-viewer.desktop").read_text()
        fields = dict(line.split("=", 1) for line in desktop.splitlines() if "=" in line)
        require(fields["Name"] == "Finalverse", "Wrong desktop application name")
        launch = shlex.split(fields["Exec"])
        require(len(launch) == 2 and launch[1] == "%u" and
                Path(launch[0]).resolve() == (fixture / "finalverse").resolve(),
                "Desktop launcher loses path quoting or no-URL launch")
        require(fields["MimeType"] == "x-scheme-handler/finalverse;", "Desktop scheme namespace")
        # Exercise the actual wrapper with a harmless fake viewer. The desktop
        # helper is stubbed so this test cannot write the user's menu entries.
        launcher = fixture / "finalverse"
        shutil.copy2(VIEWER / "linux_tools/wrapper.sh", launcher)
        script.write_text("#!/bin/bash\nexit 0\n")
        script.chmod(0o755)
        binary = fixture / "bin/do-not-directly-run-finalverse-bin"
        binary.parent.mkdir()
        binary.write_text('#!/bin/bash\n: > "$FV_TEST_ARGS"\nfor arg in "$@"; do printf "%s\\n" "$arg" >> "$FV_TEST_ARGS"; done\n')
        binary.chmod(0o755)
        arguments_file = Path(temporary) / "arguments"
        environment = dict(os.environ, FV_TEST_ARGS=str(arguments_file), LL_WRAPPER="")
        for arguments, expected in (([], []), (["finalverse://Example/128/128/30"],
                                             ["--url", "finalverse://Example/128/128/30"])):
            subprocess.run(["bash", str(launcher), *arguments], env=environment,
                           check=True, capture_output=True)
            require(arguments_file.read_text().splitlines() == expected,
                    "Linux ordinary/protocol launch arguments changed")
    checks.append("Linux desktop entry with spaces")
    checks.append("Linux wrapper ordinary and protocol argument dispatch")

    # These are source contract checks, not an executed Windows installer claim.
    nsis = (VIEWER / "installers/windows/installer_template.nsi").read_bytes().decode("utf-8")
    forbidden = (r"\Roaming\SecondLife", r"\Local\SecondLife", r"\SecondLifeViewer2", 'FindWindow $0 "Second Life"')
    active = "\n".join(line for line in nsis.splitlines() if not line.lstrip().startswith((";", "#")))
    require(not any(value in active for value in forbidden), "Installer touches another viewer's data or process")
    require('"SOFTWARE\\Finalverse"' in nsis, "Installer vendor namespace")
    require('ReadRegStr $0 HKEY_CLASSES_ROOT "${URLNAME}\\shell\\open\\command"' in nsis, "Uninstall must check current protocol owner")
    require('"x-grid-location-info"' not in active, "Installer takes over a shared scheme")
    require("precheck" not in active and
            "Exec '\"$INSTDIR\\$VIEWER_EXE\" $SHORTCUT_LANG_PARAM'" in active,
            "Post-install launch still invokes the upstream updater")
    for file in (VIEWER / "installers/windows").glob("lang_*.nsi"):
        raw = file.read_bytes()
        encoding = "utf-16" if raw.startswith((b"\xff\xfe", b"\xfe\xff")) else "utf-8"
        old = subprocess.check_output(["git", "show", "aec8eaf022e0b67b415727cf512d62c396132811:" + str(file.relative_to(ROOT))], cwd=ROOT)
        keys = lambda value: re.findall(r"(?m)^LangString\s+(\S+)", value.decode(encoding))
        require(keys(raw) == keys(old), "Installer language identifiers changed")
    manifest = (VIEWER / "viewer_manifest.py").read_text()
    require('!define SHORTCUT "%(app_name)s"' in
            manifest.split("version_vars =", 1)[1].split("inst_vars_template", 1)[0],
            "Installer product macros must precede language includes")
    checks.append("Windows installer ownership constraints (source only)")

    baseline_license = subprocess.check_output(["git", "show", "218de297a4a2cc286aba54e5df40e5e9e69caf43:LICENSE"], cwd=ROOT)
    require((ROOT / "LICENSE").read_bytes() == baseline_license, "Inherited source license changed")
    checks.append("inherited license preserved")

    result = {"checks": checks, "locales": locales}
    if bundle:
        resources = bundle / "Contents/Resources"
        info = plistlib.loads((bundle / "Contents/Info.plist").read_bytes())
        require(info["CFBundleIdentifier"] == "com.finalverse.viewer", "Bundle identifier collides with upstream")
        require(info["CFBundleName"] == "Finalverse", "Bundle display name")
        localized = (resources / "English.lproj/InfoPlist.strings").read_text()
        require('CFBundleName = "Finalverse";' in localized,
                "Localized bundle metadata overrides the product name")
        require("Finalverse" in info["NSMicrophoneUsageDescription"], "Microphone consent identity")
        require((resources / info["NSMainNibFile"]).exists(), "Branded native menu absent")
        schemes = info["CFBundleURLTypes"][0]
        require("finalverse" in schemes["CFBundleURLSchemes"], "Finalverse URL scheme absent")
        require(not schemes.get("LSIsAppleDefaultForScheme", False), "Bundle forces an existing scheme default")
        data = json.loads((resources / "build_data.json").read_text())
        require(data["Channel Base"] == "Finalverse" and data["Channel"].startswith("Finalverse"), "Package channel identity")
        require(data["Update Service"] == "" and data["Update Mode"] == "manual", "Wrong update service")
        require(not (resources / "updater").exists(), "Inherited updater shipped")
        require((resources / "skins/default/html/en-us/welcome/index.html").exists(), "Offline welcome page missing")
        for name in ("login.png", "login-small.png", "startup.png", "notification.png", "mark.png", "lumi.png"):
            relative = Path("skins/default/textures/finalverse") / name
            require((resources / relative).read_bytes() == (VIEWER / relative).read_bytes(), f"Wrong packaged artwork: {name}")
        require((resources / "secondlife.icns").read_bytes() == (VIEWER / "icons/finalverse/secondlife.icns").read_bytes(), "Wrong app icon")
        require((resources / "licenses.txt").is_file(), "Third-party notices missing")
        require((resources / "finalverse-branding.json").read_bytes() == (VIEWER / "branding/asset-manifest.json").read_bytes(), "Artwork provenance absent from package")
        result["checks"].append("actual macOS bundle identity, resources, notices and update isolation")
        result["bundle"] = str(bundle)
        result["channel"] = data["Channel"]
        result["executable_sha256"] = hashlib.sha256((bundle / "Contents/MacOS" / info["CFBundleExecutable"]).read_bytes()).hexdigest()
    return result


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--bundle", type=Path)
    args = parser.parse_args()
    print(json.dumps(verify(args.bundle), indent=2))
