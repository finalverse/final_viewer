# Migration from inherited UI

| Surface | Facade | Existing implementation retained |
|---|---|---|
| World chrome | Compact identity/command/navigation cards | Status/nav/menu containers, keyboard mappings |
| AI | Compact context, readable preview, technical expansion | Capability + gateway + world/runtime executors |
| Movement/camera | Explore | `move` / `view` LLCommandManager commands |
| Creation | Create / selection Edit / Tools | WorldAction recipes and build tools |
| Social | Friends | People friends/nearby tabs; `chat` command |
| Personal | Me | `profile`, `appearance`, `inventory`, `preferences` |
| World discovery/sharing | Explore | `map`, `places`, `snapshot` |
| Diagnostics | Tools → Full interface | Inherited menus, performance and other advanced panels |

New facade source is confined to LLPanelFinalverse, LLFloaterFinalverseExperience, presentation helper, XUI, a few registration/build/init seams, and the existing Finalverse AI floater. New settings/tokens use Finalverse names. Inherited classes are not mass-renamed. MutSea remains MutSea. Renderer, protocol and bundled-font implementations remain unchanged.

Full interface is a reversible saved preference. Reverting the Phase 4A viewer commit removes the facade; preserved Phase 3 branches and packaged Citizens app remain available. This branch does not rewrite upstream history, published main, or unrelated dirty worktrees.

Remaining inherited surfaces include login/account registration, build/editor panels, inventory internals, friends/people lists, profiles/outfits, movement/camera/map, settings, diagnostics, context menus and floater chrome. They are deliberately reached through clearer product entry points, not falsely described as redesigned.

Two small grid compatibility seams were added after live verification. A non-system grid that does not advertise account-level benefits keeps existing default limits without showing a false vendor-service failure notice; system-grid and advertised-but-invalid benefits still report failures. Non-system grids use neutral empty-friends guidance instead of advertising the vendor destination service. Vendor-grid behavior and copyright notices remain intact.
