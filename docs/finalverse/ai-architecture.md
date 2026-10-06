# AI architecture and integration seams

There is no OpenAI/Anthropic/Ollama provider router, Finalverse action validator or WorldLine journal in the audited C++ source. The relevant existing asset is structured event dispatch, not AI orchestration.

## Reuse points

- `LLEventAPI`, `LLEventPumps`, `LLSD` provide schema-like structured dispatch and responses.
- `LLAgentListener` already exposes nearby object/avatar inspection, own-avatar position, autopilot, touch/sit/stand and animation.
- `LLLeap`/`LLLeapListener` expose subprocess event bridges and API introspection. They are powerful developer/plugin mechanisms, not a policy sandbox.
- `LLAppViewer::doFrame` obtains the mainloop pump; `LL::WorkQueue` and main-thread task utilities provide scheduling seams.
- `LLCore::HttpRequest`, response handlers and coroutine utilities provide existing asynchronous network machinery.
- `LLFloaterReg` and XUI can host an additive AI panel without replacing chat, movement or inventory UI.

Evidence: [agent operations](../../indra/newview/llagentlistener.cpp), [events](../../indra/llcommon/lleventapi.h), [LEAP](../../indra/llcommon/llleap.cpp), [LEAP introspection](../../indra/llcommon/llleaplistener.cpp), [frame loop](../../indra/newview/llappviewer.cpp), [HTTP](../../indra/llcorehttp/httprequest.h), [UI registry](../../indra/newview/llviewerfloaterreg.cpp).

## Proposed request flow

1. Human text/spatial selection supplies explicit intent and context scope.
2. Viewer adapter produces a bounded, immutable snapshot of loaded entities.
3. Brain receives user intent plus data; world labels/chat are quoted untrusted content.
4. Provider adapter normalizes streaming/cancellation/errors and proposes typed actions.
5. Trusted kernel checks schema, identity, capability, permissions, expiry and stale state.
6. Desktop executor queues an allowed action on the main thread and observes existing server updates.
7. Journal records proposed/denied/dispatched/observed/unknown states and exposes the result in the panel.

Never route model output to arbitrary LEAP APIs, `LLViewerControlListener::set`, shell commands, scripts or arbitrary viewer methods. Existing events that skip confirmations are not authorization for an AI caller. Internal model tool names are mapped through an explicit allowlist. Selected object text must not expand capability scope.

## Provider boundary

A neutral request carries messages, snapshot references, supported action schemas, cancellation and output limits. A neutral response carries assistant text and action proposals with provider/model/request provenance. Providers declare tool/stream/vision capabilities; unsupported features are surfaced rather than guessed.

Server adapters can support OpenAI-compatible APIs, Anthropic, Gemini, xAI and local model protocols over time. Codex integration is a separate execution/product interface to evaluate, not a claim that every provider uses the same chat API. Do not embed privileged cloud keys in source, app bundles, settings defaults or CI artifacts. Desktop authenticates to the Finalverse service with a user session; cloud credentials live there. User-configured local endpoints can be supported with explicit connection controls and data scope.

Use existing HTTP/cancellation primitives for the first client connection; no vendor SDK is required in the portable contract library. Minimize what leaves the client: exclude login credentials, capability URLs, private IM/voice, complete inventory and raw asset contents by default. Model inference cannot itself grant world permissions.

## Agent scope

The first client controls the logged-in human avatar and user-owned objects. A persistent Lumi NPC needs a server agent identity/binding, presence lifecycle, simulator capabilities and memory authority. The viewer is not a substitute for an always-on agent runtime. Speech recognition/synthesis and voice channel injection require separate policy, device and latency work after text proves the loop.
