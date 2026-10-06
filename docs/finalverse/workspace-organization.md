# Local project organization

The user authorized moving all Finalverse-related projects to `~/Finalverse` during Phase 0. Existing projects were inventoried before movement; filesystem renames retained directory identity, file modes, untracked data and Git state. No repositories or histories were combined.

| New physical location | Purpose |
|---|---|
| `/Users/wenyan/Finalverse/viewer` | Direct Second Life fork being audited here |
| `/Users/wenyan/Finalverse/classic` | Existing FinalProjects workspace: MutSea, OpenSim, SOS/product services, shared core, platform shells and legacy variants |
| `/Users/wenyan/Finalverse/tetra` | Independent TetraMesh Finalverse/FinalStorm/TetraUI workspaces and backups |
| `/Users/wenyan/Finalverse/design` | Existing Studio design and prompts |
| `/Users/wenyan/Finalverse/web` | Existing website and local deployment script |
| `/Users/wenyan/Finalverse/tools` | Waver and its existing backup |
| `/Users/wenyan/Finalverse/dev` | New private Phase 0 sandbox/account, retained build artifact/tooling and raw verification evidence |

`~/FinalProjects` points to `~/Finalverse/classic`; its old `legacy/final_viewer` entry points to `~/Finalverse/viewer`. Moved ClaudeProjects entry paths are compatibility links. Existing relative sibling dependencies inside each family therefore retain their layouts. The root directory is a workspace index, not a Git repository. Parent local AGENTS instructions and independent banking/mail/music products were not moved or copied into repositories.

The root-local `reorganization-manifest.json` records 13 moves, 30 nested repository snapshots, sanitized remote URLs and successful before/after Git-status and inode checks. Ten embedded credential portions in old FinalProjects remotes were removed under the existing credential-handling rules; values were neither logged nor retained. This credential cleanup is the only intentional existing Git-config edit. Subsequent audit documentation and its local commit are scoped to this viewer alone.

The separate Rust and mobile projects are inventory context, not evidence that this viewer already exposes their protocols, native mobile surfaces or renderer. Reuse decisions require their own integration audit and baseline.
