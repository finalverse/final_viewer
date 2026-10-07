![Finalverse](indra/newview/branding/finalverse-mark.svg)

# Finalverse Viewer

Finalverse is evolving the mature open-source Second Life Viewer into an AI-native persistent 3D world client. This is an incremental transformation: world streaming, avatars, communication, inventory, editing and rendering remain the bootstrap runtime.

The current milestone establishes Finalverse product identity, artwork, native menus, independent settings/cache directories and desktop packaging. AI world actions and the World Kernel are planned; they are not implemented by rebranding.

Start with [the architecture and measured baseline](docs/finalverse/README.md), [branding and verification](docs/finalverse/rebranding.md), and [building on macOS](docs/finalverse/upstream-baseline.md). Windows and Linux source/packaging paths are retained; current runtime verification is on macOS through Rosetta.

Use an account on the grid you select. Finalverse does not replace grid identity, permissions, inventory or server authority. The default welcome page is bundled locally; grid-provided login pages can be enabled with `UseGridLoginPage`. Updates are manual until a trusted Finalverse update service exists.

## Contributing and provenance

Use [the project issue tracker](https://github.com/finalverse/final_viewer/issues) for Finalverse feedback. Keep changes small, tested and compatible with upstream. Preserve source copyrights, the [LGPL license](LICENSE), [third-party notices](indra/newview/licenses-mac.txt), and [artwork attribution](doc/LICENSE-logos.txt).

Inherited code is authored by Linden Lab and upstream contributors. Finalverse branding modifications and newly created artwork are recorded in [the branding provenance ledger](indra/newview/branding/README.md). This is an independent fork, not the official Second Life client.

<details><summary>Verbatim upstream README retained for provenance</summary>

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="doc/sl-logo-dark.png">
  <source media="(prefers-color-scheme: light)" srcset="doc/sl-logo.png">
  <img alt="Second Life Logo" src="doc/sl-logo.png">
</picture>

**[Second Life][] is a free 3D virtual world where users can create, connect and chat with others from around the
world.** This repository contains the source code for the official client.

## Open Source

Second Life provides a huge variety of tools for expression, content creation, socialization and play. Its vibrancy is
only possible because of input and contributions from its residents. The client codebase has been open source since
2007 and is available under the LGPL license. The [Open Source Portal][] contains additional information about Linden
Lab's open source history and projects.

## Download

Most people use a pre-built viewer release to access Second Life. Windows and macOS builds are
[published on the official website][download]. More experimental viewers, such as release candidates and
project viewers, are detailed on the [Alternate Viewers page](https://releasenotes.secondlife.com/viewer.html).

### Third Party Viewers

Third party maintained forks, which include Linux compatible builds, are indexed in the [Third Party Viewer Directory][tpv].

## Build Instructions

[Windows](https://wiki.secondlife.com/wiki/Build_the_Viewer_on_Windows)

[Mac](https://wiki.secondlife.com/wiki/Build_the_Viewer_on_macOS)

[Linux](https://wiki.secondlife.com/wiki/Build_the_Viewer_on_Linux)

## Contribute

Help make Second Life better! You can get involved with improvements by filing bugs, suggesting enhancements, submitting
pull requests and more. See the [CONTRIBUTING][] and the [open source portal][] for details.

[Second Life]: https://secondlife.com/
[download]: https://secondlife.com/support/downloads/
[tpv]: http://wiki.secondlife.com/wiki/Third_Party_Viewer_Directory
[open source portal]: http://wiki.secondlife.com/wiki/Open_Source_Portal
[contributing]: https://github.com/secondlife/viewer/blob/main/CONTRIBUTING.md

</details>
