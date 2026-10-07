# World Tool Protocol v1

Canonical JSON Schemas live in the separate local repository `~/Finalverse/services/ai-gateway/schemas`: `world-action.schema.json`, `world-plan.schema.json`, and `planner-response.schema.json`. The MutSea kernel independently rejects unknown fields/types; schema validation is not authorization.

A proposal contains `schema_version: 1`, the request, provider/model provenance and 1–8 actions. Each action contains an allowlisted `type`, explicit `target_id` (null for creation) and exact per-type parameters. Actor/region identity, expiry, generated target IDs, transaction IDs and timestamps are assigned by the authenticated server, never accepted as model authority. Capability URLs are bearer credentials and are never model context.

| Action | Parameters / supported scope |
|---|---|
| move_entity | absolute `position`, or `toward_id` + bounded `distance`; owned safe root only |
| rotate_entity | normalized quaternion `rotation` |
| scale_entity | `scale` in meters |
| set_name / set_description | bounded text, no control characters |
| set_color | normalized RGBA; per-face color unsupported |
| create_primitive | box/sphere, position, scale, color, name |
| clone_entity | position; own AI-created plain full-permission primitive only |
| delete_entity | empty parameters; own AI-created plain primitive with exact generated permissions/material form |
| create_structure | anchor, width, depth, height, yaw, Lumi owner ID |

The `FinalverseWorld` JSON POST operations are `inspect`, `prepare`, `commit`, `undo`, `journal`, `semantic`, and `lumi`. Inspect returns authoritative region name/bounds, avatar transform, selected entity, distance-sorted nearby entities, terrain, semantic references and undo candidate. Radius is at most 64m, count at most 64; missing selection or out-of-scope selection is rejected. Model context is smaller than inspection output.

`prepare` returns an immutable plan ID, expiry, exact component steps and a summary. `commit` accepts only that ID and revalidates state/permissions. Repeated successful commit returns the original operation. Undo is a new guarded compensation; retry after undo does not recreate geometry.

No arbitrary scripts, shell commands, properties, purchases, inventory transfer, material import or environment mutation are exposed. Environment/terrain editing and richer assets remain future actions.

Inspection entities include `clearance_radius`, derived from inherited group bounding boxes for placement protection. It is server metadata and is omitted from model context. Clone/delete/undo guards compare the supported shape and effective eligibility as well as identity, ownership, names, descriptions, transforms and color. Recovery may require manual review when unrepresented legacy attributes change; this is a deliberately bounded development contract.
