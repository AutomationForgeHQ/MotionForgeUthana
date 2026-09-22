# MotionForgeUthana

Every released version of MotionForgeUthana, newest first. A release publishes **one** section of this
file — the one whose heading matches its tag — as its release notes; for an `open` plugin those
notes are posted to Discord `#releases` automatically. Write for someone who installs the plugin,
not for the commit log.

Headings are `## <x.y.z> — <date>`. Use `Added` / `Changed` / `Fixed` / `Compatibility` /
`Known issues`, only the ones that apply.

## 0.2.0 — 2026-09-22

### Added
- A settings page for the plan - pay as you go or subscription - its rates and currency, carried over from MotionForge's old billing settings. Every price in the editor comes from it. A pay-as-you-go rate starts at Uthana's published price, $0.10 a generated second, and every price says it is that until you enter your own - including on a project whose old rate was zero, which meant seconds only.
- Uthana's own settings on every definition: the model and prompt rewriting.
- Setup steps for Get started - the key, the plan, a character on Uthana, the connection - and the character's actions: *Upload*, with its options, and *Import Uthana's rig*.
- On pay as you go, takes play in the editor before choosing, because fetching one is free. On a subscription the viewer link, and an import that says what quota it will use and asks.

### Compatibility
- Needs MotionForge 0.4.0 or later.

## 0.1.1 — 2026-09-08
- Packaging fix: the release now carries everything the register allows. `BuildPlugin`'s filter excludes `Config/` and every `public_extra` path, so earlier zips shipped without them.
- `Config/ForgeMachine.json` reaches an installed copy for the first time, so the hub's Keys and Runners pages are no longer empty for it.

## 0.1.0 — 2026-09-07
- Extract ElevenLabs and Uthana; the cores now name no vendor
- Two open plugins get their licence, and DeepL joins the register
- Every plugin now points at kovati.dev
