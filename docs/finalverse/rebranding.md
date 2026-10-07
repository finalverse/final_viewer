# Phase 1: Finalverse product identity

Implementation date: 2026-10-07. Source base: Phase 0 audit commit `aec8eaf022e0b67b415727cf512d62c396132811`, inherited viewer baseline `218de297a4a2cc286aba54e5df40e5e9e69caf43`. Development used the isolated `codex/finalverse-branding` worktree because the original `contribute` checkout contains thousands of unrelated mode changes and local build flags. Verified local commits are integrated by fast-forward, preserving those edits and original file modes. No push or upstream merge is part of this milestone.

## Implemented behavior

The product channel defaults to **Finalverse Test**. The app uses the user's existing silver-and-purple Finalverse mark and platform icons, purple/mint accents, Finalverse login/startup textures and Lumi welcome artwork, a bundled offline welcome page, Finalverse native macOS menu titles, product/version strings and explicit inherited attribution in About. Product `APP_NAME` strings use Finalverse in every locale that defines them; other locales retain normal fallback behavior. Newly added welcome, first-run and attribution text is English; translation review remains a release gate.

`llcommon/finalversebrand.h` provides the profile identity and update policy. Existing directory code uses **Finalverse** for application support, logs, cache and temporary directories; no migration or deletion of Second Life profiles is attempted. macOS uses `com.finalverse.viewer`; Windows uses `FinalverseViewer.exe`, a Finalverse window class/mutex/vendor registry namespace and owned protocol uninstall checks. Linux uses a quoted Finalverse launcher, desktop entry and `finalverse` scheme. The parser accepts `finalverse:` as an alias of the existing SLURL grammar; legacy URL interpretation remains compatible.

The default login browser displays packaged content rather than the upstream promotion page. Local-page navigation supplies CEF with an escaped `file:` URI, including app paths with spaces. `UseGridLoginPage` is an explicit compatibility setting, and the existing `ForceLoginURL` override still works. First-run guidance tells users to obtain an account from their selected grid; it does not open an upstream registration page. Account creation/password recovery controls are hidden for custom grids at initial display and grid changes. Custom grids use the regular login layout with a grid selector even on first launch; the inherited system-grid first-login layout uses square Lumi artwork. Protected Second Life grid definitions, authentication and simulator networking are retained.

Bug reports and manual release discovery point to `finalverse/final_viewer` on GitHub. The inherited updater is disabled and its launcher is omitted from the Finalverse macOS/Windows package. Automatic updating requires a separately designed, trusted Finalverse distribution service. The CI workflow no longer selects the upstream product/crash database by default. Upstream synchronization is a manually invoked, read-only inspection workflow; it cannot merge or push changes.

## Preservation and scope

The renderer, world protocols, avatars, inventory, editing, permissions, voice and asset code remain the bootstrap runtime. Existing source license, copyright headers, contributor lists and third-party notices remain. Supplied artwork provenance, byte hashes and packaging commands are documented in [the artwork guide](../../indra/newview/branding/README.md). No new artwork license is asserted for the user's existing materials.

The server-side product remains **MutSea**, as explicitly requested by the user. No server repository, service name, database, original asset collection or server endpoint was rebranded by this client milestone. The local Finalverse-named region is a test world label, not a server product rename.

Internal `SecondLife` CMake targets, protocol structures, compatibility schemes and some asset filenames remain intentionally stable. Service-specific Second Life links, grid names, abuse/help functions and legally required notices are not relabeled as Finalverse services. Existing dependency archive URLs still identify their actual upstream suppliers. Rebranding does not imply redistribution clearance; [licensing-audit.md](licensing-audit.md) remains the distribution gate.

The new URL scheme is an input alias for the existing grammar. Generated world links and specialized in-chat link patterns retain the compatible legacy scheme; a new cross-grid World Protocol is a later architectural phase.

This phase implements product identity, not the AI-native acceptance test. There is no AI Shell, semantic entity service, action executor or WorldLine journal yet. Their boundaries remain the proposal in [architecture.md](architecture.md).

## Verification

The Python packaging suite was repaired in local commit `5ed0cc0427` and all eight cases pass. Repairs align the tests with the current manifest registry/prefix/command APIs and remove malformed Windows path escapes; library behavior was not changed to satisfy stale tests.

`scripts/finalverse/verify_branding.py` checks locale product names, changed XML, actual Linux desktop-entry generation with spaces, Windows installer ownership constraints and the byte-identical inherited source license. With `--bundle`, it additionally checks the actual macOS plist, native menu resource, URL schemes, manual-update metadata, absent updater, packaged welcome/artwork and retained notices. Windows checks are source checks; they do not prove an executed installer.

The macOS Intel `RelWithDebInfoOS` build runs through Rosetta on this Apple Silicon host. Build ID is `262800001`, version `7.1.14`. The full Phase 1 native TUT/integration run reports **1,198 cases: 1,194 passed, four inherited known skips, zero failures**, across 132 group executions. The added Finalverse URL-alias case passes alongside the three inherited SLURL cases. The final incremental build after the local-page/login fixes also passed: 681 cases executed, 677 passed and the same four known skips, with zero failures. All eight Python packaging tests pass. Linux desktop generation and wrapper dispatch were executed with isolated fixtures; Windows installer checks remain source-only.

Actual runtime verification entered the private local **MutSea** region, rendered terrain/water/sky and moved the avatar. The final packaged build repeated this smoke check: simulator-observed position changed from `(128, 128, 27.1)` to `(128, 131.05309, 27.01)`. Normal user quit exited the process, and the simulator persisted the final position with `Online=False`. The simulator then shut down normally. The avatar still renders as a cloud, matching the Phase 0 appearance gap. Voice was disabled for this smoke session; chat, voice, teleport and full inventory behavior were not retested.

Native macOS About was observed with the supplied Finalverse icon, Finalverse product/version identity and inherited Linden attribution. An initial visual pass caught the localized bundle-name override, a blank local welcome page and initial custom-grid account links; these were corrected in source before final packaging. The final app was observed displaying the supplied logo and Lumi, the MutSea Local grid selector, hidden custom-grid account links, scrollable connection/exploration/build guidance, and explicit inherited attribution. The viewer About Info, Credits and Licenses tabs were also observed, retaining contributor lists and third-party notices. Final verification includes normal quit from the login screen.

Baseline avatar appearance, native ARM, other platform builds, automated updates/signing and long-duration stability remain explicit gates. A successful clean quit in this session does not resolve the earlier Phase 0 unresponsiveness report. This build is a development artifact, not a signed or redistribution-cleared release.

The temporary build uses the Phase 0 pinned archive cache. Missing staged dependencies were restored from those exact archives; all 44 cached archive hashes match the recorded Autobuild pins. The cause of the missing staging inputs is not established. No dependency version was upgraded to obtain a passing build.

## Reproduction

Use the pinned tooling and open Intel configuration in [baseline-macos.sh](baseline-macos.sh), with `AUTOBUILD_BUILD_ID=262800001` and explicit `-DVIEWER_CHANNEL=Finalverse Test` so an older CMake cache cannot retain the upstream channel. Keep the external build variables and macOS SDK override from [upstream-baseline.md](upstream-baseline.md). The new native menu is compiled from `SecondLife.xib` with Xcode `ibtool` during configure.

```sh
PYTHONPATH=indra/lib/python python3 -m unittest discover -s indra/test -p test_llmanifest.py
python3 scripts/finalverse/verify_branding.py --bundle '/path/to/Finalverse Test.app'
```

Use a separate `CFFIXED_USER_HOME` and a private local simulator for runtime verification. Never put account passwords in command-line arguments, source, screenshots or committed logs. Raw build/runtime evidence is retained privately under `~/Finalverse/dev/evidence/phase1`; distributable documentation contains only sanitized outcomes.
