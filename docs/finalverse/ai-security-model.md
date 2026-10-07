# AI security model

The model proposes; the server authorizes and executes. World labels and descriptions are untrusted data. No model can grant itself ownership, capabilities, unlimited actions, script execution or access to other users' objects.

Trust boundaries: human viewer → per-avatar regional capability → MutSea kernel; viewer → loopback planner → local model. The planner never receives passwords, session IDs, capability URLs, voice/IM, complete inventory or assets. No cloud provider secret is compiled or configured in the app. The gateway binds only `127.0.0.1:18765`, rejects browser Origin requests and has no execution endpoint. It is development-only; public service authentication, TLS, quotas and signed provider provenance are not implemented.

Conservative limits: prompt 2048 characters, JSON request 32KiB, 8 actions, 24 creations, total changed/created primitive volume 120m³, 12m move/clone distance, 0.05–12m component scales, 64m inspection/edit scope, 16m home anchor distance, home 4–8m × 4–8m × 2.5–4m, two-minute approval. Limits are kernel constants in this slice, not adjustable cloud grants.

MutSea is opt-in (`[FinalverseWorld] Enabled=true`) and preserves inherited permission checks. Source-script inventory, physical objects, attachments and linksets are excluded. Box/sphere assets are bounded and built from inherited primitives. No external text-to-3D service is required.

The development client must use a verified explicit `grid_login_id`. An unknown command-line grid now clears supplied login info and disables automatic login, preventing fallback transmission to another grid. One-session bootstrap input uses `--sessionsettings`, with a private file; it must be removed after entry. Normal logs and operation data are retained privately, not committed. A launch configuration mistake during verification was detected; the generated development credential was rotated in both affected isolated fixtures and old sessions revoked. No user/cloud credentials were changed.

Residual risks: capability theft has the inherited authenticated-user authority; local planner DoS is possible; legacy edits can race between checks; database/journal durability is not atomic; object color/geometry support is deliberately limited. These are release-hardening gates, not claims of a production security audit.
