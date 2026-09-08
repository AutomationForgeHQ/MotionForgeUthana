# MotionForge Uthana

Adds Uthana to [MotionForge](https://github.com/AutomationForgeHQ/MotionForge) as a text-to-motion provider. Add-on only:
removing it changes nothing about MotionForge except which providers are registered.

Until 2026-09-01 this code was a folder inside the MotionForge core, and the core's settings
defaulted to a vendor by name — including a model id, `text-to-motion-3.0`, which is Uthana's
private vocabulary and had no business being a core default. It is the same provider, extracted -
the same shape as MotionForgeKimodo, FaceForgeACE and MeshForgeCloud.

**Everything measured about the account, the API and the rig mapping was measured through this
code while it lived in the core** - the verification history is in MotionForge's README and git
log, and it still applies verbatim: the extraction moved files and changed registration, not
behaviour.

---

## What this provider is

Talks to Uthana's GraphQL API. Two model families behave differently and the difference matters:

| | |
|---|---|
| `text-to-motion-2.0` | synchronous, 0.25–10 s, exposes seed / cfg_scale / steps |
| `text-to-motion-3.0` | asynchronous, 4–10 s integer, no seed, higher quality |

Only the async shape is implemented, because v3.0 is the one worth generating with. The absence of
a seed on v3.0 is the reason MotionForge never discards candidates anywhere: a take that is not
kept can never be produced again, only re-rolled into something different.

Native frame rate is 60, asked through `GetCaps` rather than assumed - Kimodo answers 30 to the
same question, which is why it is a capability and not a setting.

## The key

Declared in `Config/ForgeMachine.json`, stored in the OS credential vault under
`MotionForge/Uthana`, environment fallback `MOTIONFORGE_UTHANA_KEY`. The entry names predate the
extraction and keep their old spelling on purpose: they are looked up rather than displayed, and
renaming them would be a migration for no gain - a key set before the extraction is still found
after it.

Set it in **Tools ▸ Automation Forge ▸ Keys**, the hub, or MotionForge's own settings page - all
three address the same vault row.

## Related

- **[MotionForge](https://github.com/AutomationForgeHQ/MotionForge)** — the pipeline this registers with: definitions,
  takes, retargeting, provenance.
- **[MotionForgeKimodo](https://kovati.dev/plugins/motionforge/)** — the free local provider, one shape over.
