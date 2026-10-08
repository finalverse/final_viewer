# Upstream synchronization — 2026-10-08

`main` is the product integration branch. All mandatory pre-publication gates pass; the next step is a normal `main` push and setting GitHub's default branch to `main`. `contribute` remains available as the direct-parent tracking branch. Normal clones receive the Finalverse AI integration. Previous Phase 0/1/2 reports remain evidence for their recorded commits, rather than test results for this much larger upstream update.

## Source and history

| Reference | Commit |
|---|---|
| Previous upstream baseline | `218de297a4a2cc286aba54e5df40e5e9e69caf43` |
| Starting local AI viewer | `a87158a73df8b718539215b3657ed8fa213489fa` |
| `finalverse/final_viewer`, `contribute` | `02d76c98b3f909d065b591b336b652d5121c989f` |
| Direct parent `qwy16/secondlife_viewer`, `contribute` | `02d76c98b3f909d065b591b336b652d5121c989f` |
| Original `secondlife/viewer`, `develop` | `7dd6de6120ce7a80b0a290d34a701e8717d03962` |
| Reviewed integration merge | `6f11650a35` |

GitHub fork metadata identifies the direct parent. Its merge head contains the current original Second Life head. Fetching revealed 2,104 upstream commits absent from local `main` and eight local commits absent from upstream. Relative to the Phase 0 source, the update changes 2,174 files, with 92,421 insertions and 88,508 deletions. Histories were merged without squashing or replacing upstream commits. All eight completed local milestones remain ancestors of `main`.

## Integration decisions

Twenty conflicted files were reviewed against both implementations. Finalverse identity, artwork, local welcome content, independent profile/protocol ownership, manual-update policy, supplied asset provenance and the AI floater were retained. The new Seed capability list contains both `FinalverseWorld` and upstream's `SpatialVoiceModerationRequest`.

Upstream's new login layout and `unique_ptr` listener ownership were retained. Finalverse grid-specific links now use the new `sign_btn` control; the removed first-login panel was not resurrected. The smaller current macOS XIB is compiled with the Finalverse native menu and window identity. New watchdog/startup/crash markers use the independent Finalverse namespace. Newly introduced SDL metadata and D-Bus service/path/interface identifiers also use Finalverse's namespace. The window-class constant moved into `llappviewer.h`; it now matches the independent installer checks. The explicit Linux protocol-registration helper registers only Finalverse's scheme and preserves the inherited helper filenames for packaging compatibility.

The runtime now includes upstream's universal `arm64;x86_64` macOS configuration, updated dependencies, SDL3 Linux windowing, additional glTF/Lua functionality and voice changes. These are inherited changes, not a Finalverse renderer rewrite. The AI interface continues to propose structured plans for the server-side policy/executor; the new Lua surface is not exposed as an AI mutation bypass.

The deferred Linux worktree was preserved. Its SDL 1.2/GLX and old dependency work is superseded by upstream's SDL3 architecture and was not copied into that replacement. Two independent, still-applicable fixes were ported with their original regression tests: initialize GLTF material-copy padding before hashing, and copy a potentially self-referenced texture entry before deleting the owned original. The updated upstream's null-entry semantics were retained. The independent Linux/glibc HTTP-thread cancellation fix was also retained: forced unwinding is rethrown before the application catch-all, guarded for libstdc++/glibc. Its Linux runtime verification remains deferred to that platform.

The build workflow retains new upstream platform/test jobs, uses open-source configurations, defaults to the established installer path, and uses Finalverse release tags. Automatic upstream merging remains disabled; the read-only inspection workflow now names the actual parent's `contribute` branch. Windows/Velopack and Linux execution still require platform-specific verification.

## Verification and reproduction

Python manifest tests: eight passed. Source branding checks passed for translated product identity, modified XML, artwork provenance, Linux launcher/protocol argument handling, Windows installer ownership constraints and inherited license preservation. These source checks are not an executed Windows or Linux build.

The macOS candidate is configured from the current manifest into `build-darwin-universal`, using Xcode 26.3, the preserved Python tooling/build variables, `RelWithDebInfoOS`, `LL_TESTS=ON`, OpenAL and `USE_VELOPACK=OFF`. Its separate development bundle identifier is `com.finalverse.viewer.sync`; the prior `com.finalverse.viewer.phase2` app and profile are preserved. The full rebuild and package generation succeeded. Binary inspection confirms `x86_64 arm64`; a sample of the running fresh app identifies `Code Type: ARM64`. The executable SHA-256 is `ca7b144f947ec043b6d523e4b5be06c3d9e0750718da017b3c2997d512e16172`. Package verification passed all seven groups; eight Python manifest tests passed. No prior executable is substituted for this rebuild. The first post-link packaging attempt exposed an upstream Python-hint/cache problem: the executable path was used as a root directory, and the legacy alias kept a different interpreter. `Python.cmake` now honors the executable hint and refreshes the alias; the build explicitly selects the existing locked environment, which already contains `llsd`. Dependency archives also emit minimum-macOS-version warnings (12.0 and 15.5) against the requested 11.0 target. Only the current development OS is tested here; older-macOS support requires a dependency/deployment-target audit.

See [local-development.md](local-development.md) for updated commands and [MutSea's sync report](https://github.com/finalverse/mutsea-o/blob/main/docs/mutsea/upstream-sync-20261008.md) for the matching server integration. The AI planner remains a separate local repository; this two-repository synchronization does not publish or deploy that service.

## Preserved work and limits

The original viewer checkout retains its existing `.gitignore`, private build-variable files and file-mode changes. The deferred Linux checkout retains its original changes. Neither was reset or broadly staged. The clean AI candidate checkout was used for integration. No credential files, private worlds, runtime profiles, model traces or raw logs are included in Git; a bounded credential-pattern scan found no matching secret across 1,829 changed text files, including an exact check for the private test password and its login hash. Original viewer and deferred Linux status snapshots match their pre-integration hashes.

Older repository/component/dependency maps are dated baseline snapshots. The current `autobuild.xml`, CMake targets and this report supersede them for build choices. Mobile builds, the new Lua surface, upstream voice changes, full-world compatibility and production packaging/signing remain separate gates. This merge preserves provenance and notices; it does not provide distribution licensing clearance.


## Repaired validation failures

Neither failed group was disabled or marked expected-failure.

| Group | Classification and evidence | Resolution |
|---|---|---|
| `INTEGRATION_TEST_RUNNER_lldatapacker` | Inherited undefined behavior, also present at the previous baseline. Native ARM64 case 11 aborted in hardened libc++: the ASCII indentation builder indexed an empty `std::string` after `reserve()`, which allocates capacity but does not create characters. LLDB identified `writeIndentedName()` as the source. | Construct the tab string with its actual size. All 14 existing cases pass, including the stream indentation assertion. |
| `PROJECT_llprimitive_TEST_llprimitive` | Finalverse regression-test integration error. The ported self-copy test executed primitive fixture stubs that return no texture entries, rather than production `LLPrimTextureList`. | Move the regression to a real texture-list fixture linked with the existing glTF target. Two production tests cover self-copy and the upstream null sentinel; original primitive tests stay intact. |

Related seven CTest groups passed. The complete rebuilt suite passed **114/114 groups**, **0 failures**, **0 CTest-level skips**, in 52.84 seconds. Underlying TUT totals are **1,274 cases: 1,265 passed, 0 failed, 9 inherited known-failure skips**. Those inherited skips are `HttpRequest Tests` 5/6 (pthread cancellation), 19/20/21 (known assertions/functionality), `LLHost` 9 (flaky), `LLTectureInfo` 10 (LLTrace statistics), `m3math_h` 12 (architecture-dependent comparison), and `llmainthreadtask` 2 (cross-thread build hangs). No new exclusions were introduced.

## Grid identity and live acceptance

Non-system grids no longer preload or automatically show the Second Life avatar welcome panel at first login, including the low-memory path. Manually opening that panel resolves the bundled Finalverse welcome page on non-system grids. Explicit system-grid behavior is retained. Existing login-page opt-in and grid selection remain separate; no assumption that every custom grid is MutSea was introduced. The forced `FirstLoginThisInstall=true` MutSea login displayed the Harbor scene without the Second Life welcome panel. Manually opening Avatar Welcome Pack loaded the actual bundled Finalverse logo and welcome page. The live About dialog reports Finalverse Test 26.4.0.262810008, identifies the server as MutSea 0.9.3.1 FinalverseDev (Arm64/Unix/DotNet), and retains Linden Research and dependency attribution on its Licenses tab. The independent bundle, profile, cache and log namespaces passed source/package and live checks.

The rebuilt app used an independent private profile and the previously verified server image in a separate loopback sandbox on port 18093. Prior Harbor and Phase 2 fixtures were preserved. Live Ollama `llama3.2:3b` inspection named actual practice objects. A selected owned Practice Cube was proposed from `(64,128,26.3)` to `(66,128,26.3)`, applied visibly, recorded as intent/committed in History, then restored with Undo. A fresh preview followed by a manual inherited-editor edit to X=64.5 was rejected with “The target changed since this plan was prepared”; the manual edit remained intact and was restored explicitly. An expired two-minute preview was also refused. A destination outside the inspected scene was refused rather than invented. A generic “east” request returned no actionable plan on one attempt; the grounded named-target request completed successfully.

The existing Lumi workflow produced a 17-part home preview at `(172,156)`, committed through the same policy/executor, and rendered its walls, roof, doorway and interior from an exterior viewpoint. Closing and reopening the AI floater, then selecting Nearby, recovered the authoritative undoable plan; History alone does not refresh that plan. Composite Undo removed the rendered home and restored empty ground. The matching server report contains the already completed restart, semantic metadata, memory and composite Undo persistence checks. Walking changed the avatar map location from Y=120 to Y=127; teleport and camera follow also worked. Normal Quit sent the logout request, received LogoutReply, exited the main loop and logged Goodbye. The owned `open -W` process returned zero and the viewer process exited; server logs also recorded logout. No forced termination was needed.

Avatar appearance remains a cloud, as before this sync; it did not prevent walking, world entry, AI interaction or object edits. An inherited account-benefits notice also appears because MutSea does not supply Second Life account benefits; `FailedToGetBenefits` exists at the starting local commit. No fabricated benefits or bypass was added. Inherited grid-specific commerce/help menu entries remain (including L$ and Second Life blogs); broad menu cleanup is outside this regression-focused sync. Intel hardware/Rosetta execution, older macOS deployment floors, Windows/Linux runtime, mobile, full voice/Lua compatibility and production signing/notarization remain unverified.

## Release gates

| Gate | Outcome | Evidence |
|---|---|---|
| 1 Upstream merge | PASS | Both current parent and starting Finalverse commits are ancestors of product main. |
| 2 Universal build | PASS | Fresh full build succeeds; both slices present. |
| 3 Packaging | PASS | Full package succeeds; seven package checks and eight manifest tests pass. |
| 4 CTest | PASS | 114 groups passed; inherited case skips enumerated above. |
| 5 Product identity | PASS | Source/package checks and live About, server identity and license notices verified. |
| 6 Welcome routing | PASS | Forced first login avoids the upstream panel; manual panel loads bundled Finalverse content. |
| 7 Launch | PASS | Fresh rebuilt app launches natively as ARM64. |
| 8 MutSea login | PASS | Isolated authenticated sandbox session succeeds. |
| 9 Harbor render | PASS | Starter scene and newly built home render. |
| 10 Movement | PASS | Walking, map coordinates and teleport verified. |
| 11 AI panel | PASS | Ask / Create opens and submits requests. |
| 12 Grounded inspection | PASS | Actual scene names returned through world capability. |
| 13 AI move | PASS | Selected cube changes X=64 to X=66. |
| 14 History | PASS | Intent and committed move visible in WorldLine. |
| 15 Undo | PASS | Original cube transform restored; composite home Undo visibly removes all components. |
| 16 Stale preview | PASS | Manual edit preserved; stale Apply rejected. |
| 17 Destination guard | PASS | Out-of-scope pavilion refused. |
| 18 Secret scan | PASS | Final changed-file scan has no credential-pattern or private-password matches; private artifacts excluded. |
| 19 Documentation | PASS | Current build, merge decisions, test totals, live/logout results and future merge watch list recorded. |
| 20 Publication | PENDING | Normal main push follows completion of mandatory gates. |

Next focused milestone after publication: **Phase 3 — Persistent AI Citizens**, building on the validated semantic/action/history boundary. Do not begin it as part of this sync.
