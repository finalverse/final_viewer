# Semantic world model

## Current state

Persistent object identity comes from `LLUUID`; network updates also use region-scoped local IDs. `LLViewerObject` derives from `LLPrimitive`, `LLRefCount` and `LLGLUpdate`, holding region, drawable, materials, children, attachment and inventory relationships. `LLVOVolume` integrates volume/mesh geometry; avatar objects are separate specializations. `LLWorld` owns the nearby region set, and `gObjectList` receives full, compressed, terse and cached updates.

[LLViewerObject](../../indra/newview/llviewerobject.h), [registry](../../indra/newview/llviewerobjectlist.h), [region collection](../../indra/newview/llworld.h), [volume](../../indra/newview/llvovolume.h), [selection properties](../../indra/newview/llselectmgr.h) are the evidence. The existing class is coupled to graphics and cannot be promoted to a portable entity model unchanged.

## Proposed sidecar, not a legacy structure expansion

Key sidecars by `(world_id, entity_uuid)`, with observed `region_id` separate. `world_id` is a stable configured world/grid namespace, not an assumption that one UUID is globally unique across all grids or that a region name identifies a world. Store object/local IDs only in the adapter and refresh them on region transitions.

| Projection | Source / interpretation |
|---|---|
| UUID, region, transform, root/parent | Observed viewer values; include coordinate frame and meters |
| Geometry/material/physics summary | Existing primitive/volume/material/physics metadata; asset references subject to visibility/rights |
| Name, description, ownership, permissions | Property cache/request; null and `unknown` if missing/stale |
| Semantic type, tags, affordances | Explicit annotations or derived claims with author/model/version/confidence |
| Relationships | Separate observed link/attachment relation from inferred near/inside/purpose relation |
| Agent binding, memory, narrative | New namespaced sidecar references; never assume simulator understands them |
| Provenance/version | Author, timestamp, origin (observed/user/inferred), local observation revision and schema version |

Initially project identity, transform, basic name/description, permissions and provenance only. A missing semantic type stays unknown. Do not classify a structure as a hospital or an avatar as an agent solely because a model says so. Store model claims separately from authoritative facts.

Snapshots are immutable values, bounded by radius/count/byte budget and tied to session/world/region/time. The adapter takes them on the viewer thread. Worker/provider code receives copies, never a pointer into the object registry. Removal/death/region unload invalidates observed entries; no sidecar creates an object by itself.

A loaded-region query reports its coverage, truncation and unavailable property fields. Persistent collaborative metadata and world memory require a service with authenticated writes and permission checks; a local sidecar alone is not shared persistent state. Do not put hidden metadata in SL assets or change inherited creator/owner provenance.


## Implemented Phase 2 sidecar

MutSea now derives persistent semantic entities from committed WorldLine records without extending inherited object layouts. A home uses its floor UUID as the semantic structure ID; all 17 independent primitive IDs share that structure relationship. The floor has semantic type `home`, other components `home_component`, and every component binds `owner_agent_id` to Lumi. Physical ownership remains the authenticated human's simulator account.

Lumi has a stable semantic ID, display name, profile, nullable avatar binding, home UUID and creation-request memory. Querying `lumi` and `semantic` after restart reconstructs both directions from durable records; semantic queries also check target existence. Undo removes the active relationship while retaining operation history. Legacy manual deletion is not yet reconciled into Lumi's home/memory projection. This is a semantic citizen identity, not an animated/autonomous NPC. The sidecar namespace is the configured MutSea region/deployment; copied test worlds intentionally retain UUIDs in isolated stores. Globally federated namespace/versioning and independent citizen memory are later work.
