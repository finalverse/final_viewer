# Upstream baseline and reproducible build

Audit date: 2026-10-07 (Asia/Shanghai). Starting commit: `218de297a4a2cc286aba54e5df40e5e9e69caf43`, viewer version 7.1.14. Original branch: `contribute`; canonical working repository: `/Users/wenyan/Finalverse/viewer` (formerly `/Users/wenyan/FinalProjects/legacy/final_viewer`). Origin is `https://github.com/finalverse/final_viewer.git`; configured upstream is `https://github.com/qwy16/secondlife_viewer.git`. Anonymous live origin HEAD and contribute refs both matched the starting commit. No fetch/merge/rebase/push was performed.

## Preserved starting state

The original checkout had 9,330 tracked file-mode differences, `.gitignore` with five added lines (four patterns and a blank line), no staged changes and untracked `build-variables/`. No submodules were configured in this direct viewer fork. The build uses a clean detached worktree of the exact starting commit; existing changes were not stashed or normalized. A SHA-256 and mode comparison of all 9,416 original tracked/untracked files found zero changes after the audit and move. Machine details are in [git-state.json](git-state.json).

The baseline worktree, isolated Python environment and dependency cache remain under `/var/folders/56/v6pzzwvd449dsz9wxqrt_sp40000gn/T/finalverse-phase0-a2zi86u6`; this is temporary storage. The built app is also retained at `/Users/wenyan/Finalverse/dev/baseline-macos/Second Life Test.app`, with a complete byte/mode/symlink inventory comparison against the original bundle. Runtime tests used the original build location; the retained copy was not separately launched. Build variables and the Python dependency lock are retained in `dev/baseline-macos/tooling/`, and raw evidence is retained in `dev/evidence/phase0/`. Source stays unmodified. The external flag input was copied from the existing untracked variables file; SHA-256 `8a0ff9fa0e204e19a3f1ba2f66076f262bdefb87aafe31154ea60692522cca96`.

## Toolchain and configuration

macOS 26.6.2 (25G83), Apple Silicon arm64 host, 16 GiB, 8 CPUs. Xcode 26.3 (17C529), AppleClang 17.0.0, SDK 26.2, CMake 4.3.1, Python 3.13.6, Autobuild 3.10.2, llbase 1.3.1, llsd 1.2.4. [toolchain-python.lock](toolchain-python.lock) captures all installed Python build dependencies. No system Python environment was changed.

Use `/usr/bin/clang` and `clang++`, not the initially preferred Homebrew Clang 22.1.8. The source and dependency selection produce **x86_64**, not native ARM. `/usr/bin/arch -x86_64 /usr/bin/true` passed the Rosetta availability probe. `RelWithDebInfoOS` is the public open configuration: proprietary package installation is disabled, OpenAL enabled, tests explicitly enabled. C++20 is selected by root CMake. Build ID is fixed to 262791648 for reproduction.

```sh
# In a scratch directory with a clean detached source worktree and Python venv:
export PATH="$PWD/venv/bin:/usr/bin:/bin:/opt/homebrew/bin:$PATH"
export AUTOBUILD_VARIABLES_FILE="$PWD/tooling/variables"
export AUTOBUILD_INSTALLABLE_CACHE="$PWD/installable-cache"
export AUTOBUILD_CPU_COUNT=4 AUTOBUILD_ADDRSIZE=64 AUTOBUILD_BUILD_ID=262791648
export PYTHON="$PWD/venv/bin/python" CC=/usr/bin/clang CXX=/usr/bin/clang++
unset AUTOBUILD_GITHUB_TOKEN GITHUB_TOKEN
./venv/bin/autobuild configure -c RelWithDebInfoOS --config-file "$PWD/source/autobuild.xml" -- \
  -DCMAKE_OSX_ARCHITECTURES=x86_64 -DLL_TESTS=ON -DUSE_OPENAL=ON \
  -DLL_SKIP_REQUIRE_SYSROOT=ON -DCMAKE_OSX_SYSROOT="$(xcrun --show-sdk-path)"
./venv/bin/autobuild build -c RelWithDebInfoOS --no-configure --config-file "$PWD/source/autobuild.xml"
```

[baseline-macos.sh](baseline-macos.sh) implements these commands with exact source/flag guards and output logs. To recreate the venv, install [the pinned Python dependencies](toolchain-python.lock). Create a detached worktree at the starting commit rather than building over the user's modified files. `LL_SKIP_REQUIRE_SYSROOT=ON` deliberately bypasses the inherited exact SDK requirement while providing the installed SDK explicitly; it is a recorded host adaptation, not a tracked source patch.

For example, with a new clean worktree and virtual environment at the chosen absolute paths:

```sh
/Users/wenyan/Finalverse/viewer/docs/finalverse/baseline-macos.sh \
  /absolute/path/to/clean-source \
  /Users/wenyan/Finalverse/dev/baseline-macos/tooling/variables \
  /absolute/path/to/build-evidence \
  /absolute/path/to/python-venv
```

Configuration fetched public archive dependencies and passed (about 192 seconds plus generation). CMake emitted compatibility warnings for CMP0175/CMP0210, optional Doxygen was missing and the image dynamic-codec integration helper was skipped. These do not establish coverage of omitted proprietary codecs, Havok or alternate platforms. [dependency-manifest.json](dependency-manifest.json) records source archive versions/platform declarations.

## Verification results

The complete Autobuild/Xcode build exited 0 with `BUILD SUCCEEDED`. It reported 1,197 TUT case executions across 132 group executions: 1,193 passed and four upstream known-failure skips. There were no failed-test summaries. [verification.json](verification.json) records the final counts and evidence hashes. CTest registration is commented out in the inherited test CMake; build-driven TUT outputs are the relevant evidence. A no-tests CTest run would not substitute for them.

The separately invoked Python `test_llmanifest.py` suite failed before test-case execution: line 60 contains an unescaped Windows path with malformed `\N`. This is an existing test-source defect. No tracked source repair was applied, and no all-tests-pass claim is made.

## Local simulator and development account

The user authorized a new development account and use of a local OpenSim/MutSea stack. An isolated fixture was created from existing MutSea binaries with fresh SQLite state, default assets and a new account; no original simulator database or existing accounts were reused. The original server checkout remains clean. Grid URL: `http://127.0.0.1:18090/`; account: `Finalverse Developer`; private credentials: `/Users/wenyan/Finalverse/dev/credentials/phase0-account.json` (0600 in a 0700 directory). No password appears in process arguments or committed documents.

The fixture is `/Users/wenyan/Finalverse/dev/phase0-sandbox`; its local `run-sandbox.sh` starts an attached console, and `shutdown` stops it. Registration was verified from the fixture account table. Grid-info HTTP returned 200; an XML-RPC authentication probe returned login=true, local simulator port 18090 and 21 inventory folders. This protocol probe alone is not actual viewer-world entry.

Server checkout is `13c546ce413e49e957e387547d97e79800cf33ca`; reused binaries were not rebuilt or proved to match it. Installed .NET 10.0.3 runs the net8.0 binary with major roll-forward; Homebrew libgdiplus is supplied via library path. `basicphysics`/`ZeroMesher` avoid unavailable native ubODE. HTTP binds all interfaces, UDP is loopback, and no public DNS or port forwarding was configured. The fixture is stopped after verification. These are development limitations, not a production server baseline.

The viewer profile is isolated using `CFFIXED_USER_HOME`; a compiled Foundation probe verified Cocoa home, application-support and cache paths resolve inside scratch storage before launching. Its custom `grids.xml` supplies only the local grid. Existing Second Life protected grid definitions remain intact. Launch/login were verified from the produced executable, runtime logs and CUA observations as recorded below.

## Measured desktop/world results

| Check | Result |
|---|---|
| Configure + full open build | Passed on this Mac, clean starting source, Xcode exit 0 |
| C++ TUT/subsystem executions | 1,193 passed; four known skips; 1,197 reported cases, 132 group executions |
| Known skips | LLSphere case 2; m3math_h case 12; llmainthreadtask cross-thread case 2; LLHost case 9 |
| Python manifest unit suite | Failed at import; no real cases executed |
| Executable | Mach-O x86_64, 88,128,704 bytes before runtime; Rosetta on Apple M1 |
| Actual launch / login UI | Passed, observed through Computer Use |
| GL runtime | Apple / Apple M1 / OpenGL 4.1; driver version includes `Metal - 90.5`, not a native Metal viewer implementation |
| World entry | Authentication, seed capabilities, inventory callbacks, `STATE_STARTED`, visible terrain/water/sky and account name |
| Avatar movement | 12 Up key taps; simulator position changed from (127.05495,125.72421,27) to (131.17856,124.4398,26.85) |
| Owned primitive | Created 0.5 m cube, renamed `Finalverse Phase0 Cube`, owner/creator displayed correctly |
| One-metre move | X 134.750 -> 135.750, unchanged Y/Z, visible and persisted in fresh simulator SQLite state |
| Existing undo | Returned X to 134.750 in viewer and persisted simulator state |
| Avatar appearance | Remained a cloud in this local fixture; full appearance validation is open |
| Chat/voice/teleport/full inventory | Not verified; chat observation hit a Computer Use window-binding timeout after object tests |
| Runtime reliability / shutdown | Viewer later became unresponsive, did not exit on SIGTERM and required SIGKILL of the verified test process; original cause remains open |
| AI action/kernel/provider | Not implemented in Phase 0 |
| Other platforms | Not built/run here |

The fixture initially lacked the shipped `data/avataranimations.xml`, producing a simulator static-initializer failure after UseCircuitCode. Copying existing default runtime data and restarting resolved avatar-presence creation without any viewer or server source edits. Appearance remains unresolved; no cause is asserted without a separate compatibility investigation.

A temporary 0600 session-settings file provides `UserLoginInfo` and disables remembered user/password while `AutoLogin` is false. The inherited Session group is explicitly non-persistent. The final viewer launch uses `/usr/bin/open -W -n --env CFFIXED_USER_HOME=<scratch-profile> -a <built-app> --args ... --sessionsettings <private-file>` so macOS registers the measured process for window access. No password appears in argv. The transient file is removed after verification; the development account remains in the private fixture and credential JSON.

Computer Use initially waited for OS permissions, then observed the live scene, object transforms and undo. During a restart its window helper launched an additional bare viewer instance; that instance was identified and stopped. The measured sandbox sessions used the isolated profile. No pre-launch snapshot of personal viewer preferences was taken, so this audit does not certify those preferences unchanged. All original repository files and working changes are separately hash/mode verified unchanged.

After the object tests, window automation reported noWindowsAvailable/timeouts while the viewer process remained alive. SIGTERM did not stop it. A process sample captured **after SIGTERM** shows `renderFinalize -> generateGlow -> glDrawArrays -> AppleMetalOpenGLRenderer` and allocator frames, followed by nested UNIX signal-handler/logging/allocator frames. This records an unresponsive process and termination issue; the post-signal sample does not establish the original trigger or prove a pre-signal GPU crash. Only the verified isolated viewer process and its verified children were stopped during cleanup; the simulator shut down normally. Reproduce the issue and collect evidence before sending a signal in the next baseline-repair phase.

Raw logs, original-file hash snapshot and transform evidence are retained privately at `/Users/wenyan/Finalverse/dev/evidence/phase0`. Runtime logs contain local session/capability data and are not committed. [verification.json](verification.json) contains selected non-secret observations and hashes. The test cube and account remain in the stopped fixture for further development. The original simulator checkout/data remain unchanged.

## Scope of completion

Phase 0 repository audit, reproducible open build and bounded live-world smoke verification are complete with explicit gaps above. This is not a production release or the AI-native MVP. Before broadening Phase 1, fix the packaging test, establish fully rendered avatar appearance on the chosen supported development grid and reproduce/resolve the viewer unresponsiveness with a clean shutdown check. Preserve renderer/network/avatar/inventory systems and add controlled identity plus a read-only adapter next.
