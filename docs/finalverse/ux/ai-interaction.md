# AI interaction

The command surface opens the same Finalverse AI flow used by the Phase 2/3 viewer. It routes explicit `Lumi` messages or a selected canonical Lumi embodiment to citizen conversation; other requests go to world planning. No mutation is attached to a HUD keypress without the existing prepare/approval step. Go home is a user-requested bounded navigation goal, consistent with Phase 3.

World planning: inspect the actual capability context → model proposes structured actions → server prepares and validates → display preview → human applies → deterministic executor → WorldLine result → guarded Undo. Generation guards discard late results, region changes invalidate proposals, and a changed selected object invalidates Apply locally. Server ownership, placement, stale-state and transaction checks remain authoritative.

The visible context line shows the current region and selected object. The existing model projection sends avatar, region, terrain, selected entity and bounded nearby entities. Camera data, arbitrary scene graphs, universal generation, and conversational world-edit history are not added to this protocol in Phase 4A. Citizen identity, active goal and memory context continue through the existing runtime.

The normal preview displays actual names, change counts, dimensions, movement deltas, color changes, renaming and deletions. A long recipe abbreviates non-destructive rows after six; every deletion remains visible. Technical details expands the exact step states and identifiers in the same scrollable area. It is off by default. This is presentation of validated data, never hidden model reasoning.

Apply is enabled only while a world preview is displayed; Create garden is enabled only while a citizen garden preview is displayed. Reading Nearby/History or submitting another request clears local approval. Cancel plan is available during non-mutating reasoning; citizen Pause/Cancel goal can interrupt an outstanding model request. Mutation controls remain disabled while a commit/Undo is in flight. Service failures are not reported as successful changes.

The capability and citizen runtime are existing single-controller MVP services. Production multi-session citizen approvals need atomic binding to the displayed proposal across concurrent controllers/sessions; this UI does not expand authority or claim to implement a distributed approval system.

Both command editors disable LLUI commit-on-focus-loss. Only Enter or an explicit Plan/Send/Ask AI action submits a draft. Changing focus or expanding details does not invoke a model. For actionable world plans, the normal approval text comes only from validated steps; the provider answer is available in Technical details. Unsupported recoloring requests in the current local planner returned no actionable plan during validation. Bounded movement and the existing home/garden recipes are the proven operations.
