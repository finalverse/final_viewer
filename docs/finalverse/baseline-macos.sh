#!/bin/bash
# Phase 0 reproduction tool. Does not edit tracked source or publish artifacts.
set -euo pipefail
if [ "$#" -ne 4 ]; then
  echo "Usage: $0 CLEAN_SOURCE BUILD_VARIABLES OUTPUT_DIRECTORY PYTHON_VENV" >&2
  exit 2
fi
fv_source=$(cd "$1" && pwd)
fv_variables=$(cd "$(dirname "$2")" && pwd)/$(basename "$2")
fv_venv=$(cd "$4" && pwd)
fv_commit=218de297a4a2cc286aba54e5df40e5e9e69caf43
if [ "$(git -C "$fv_source" rev-parse HEAD)" != "$fv_commit" ]; then
  echo "Source must be the recorded Phase 0 commit." >&2
  exit 2
fi
git -C "$fv_source" diff --quiet
git -C "$fv_source" diff --cached --quiet
if [ -n "$(git -C "$fv_source" ls-files --others --exclude-standard)" ]; then
  echo "Use an isolated clean checkout; preserve existing user files." >&2
  exit 2
fi
[ -f "$fv_variables" ]
fv_flags_hash=$(shasum -a 256 "$fv_variables" | awk '{print $1}')
if [ "$fv_flags_hash" != "8a0ff9fa0e204e19a3f1ba2f66076f262bdefb87aafe31154ea60692522cca96" ]; then
  echo "Build variables differ from the recorded Phase 0 input." >&2
  exit 2
fi
[ -x "$fv_venv/bin/autobuild" ]
mkdir -p "$3"
fv_output=$(cd "$3" && pwd)
case "$fv_output/" in
  "$fv_source/"*)
    echo "Keep output logs/cache outside the clean source tree." >&2
    exit 2
    ;;
esac
export PATH="$fv_venv/bin:/usr/bin:/bin:/opt/homebrew/bin:$PATH"
export AUTOBUILD_VARIABLES_FILE="$fv_variables"
export AUTOBUILD_INSTALLABLE_CACHE="$fv_output/installable-cache"
export AUTOBUILD_CPU_COUNT=4 AUTOBUILD_ADDRSIZE=64 AUTOBUILD_BUILD_ID=262791648
export PYTHON="$fv_venv/bin/python" CC=/usr/bin/clang CXX=/usr/bin/clang++
# Public open-source baseline: privileged cloud credentials are unnecessary.
unset AUTOBUILD_GITHUB_TOKEN GITHUB_TOKEN
cd "$fv_source"
autobuild configure -c RelWithDebInfoOS --config-file "$fv_source/autobuild.xml" -- \
  -DCMAKE_OSX_ARCHITECTURES=x86_64 -DLL_TESTS=ON -DUSE_OPENAL=ON \
  -DLL_SKIP_REQUIRE_SYSROOT=ON -DCMAKE_OSX_SYSROOT="$(xcrun --show-sdk-path)" \
  > "$fv_output/configure.log" 2>&1
autobuild build -c RelWithDebInfoOS --no-configure --config-file "$fv_source/autobuild.xml" \
  > "$fv_output/build.log" 2>&1
