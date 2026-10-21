# Camera code-factory guardrails

This factory is a small, evidence-driven delivery harness for the clean-room
adapter.  It tracks work, makes prerequisites visible, and ensures that every
change receives the checks that are actually possible in its environment.
It does not certify hardware support or vendor ABI compatibility.

## Work boundaries

- Work on one claimed story or tightly-scoped defect at a time.  Claim only a
  story shown by `scripts/factory.ps1 next -Role dev`.
- Treat `compat/`, the adapter API, CMake targets, and the evidence reports as
  shared interfaces.  Write a handoff in `factory/handoffs/` before another
  story consumes a changed interface.
- Start with a focused regression test where practical.  A hardware-only
  observation belongs in an evidence record with the device, toolchain, exact
  command, and result; it is not a reason to invent a fixture or ABI layout.

## Non-negotiable safety rules

- Preserve the clean-room rules in `CLAUDE.md`: never copy vendor source,
  headers, proprietary constants, or decompiled implementation into source.
- Unknown ABI fields, return values, and camera behaviour remain unsupported.
  Do not fill them in with guesses to make a test or demo pass.
- Keep vendor-like filenames disabled unless a separately documented isolated
  experiment has proven safe.  Passing a simulation test is not hardware
  compatibility evidence.
- Record a confirmed process-caused defect in `factory/incidents.jsonl` and
  add its prevention check before closing it.

## Verification and review

- Run `scripts/factory.ps1 verify` before review.  It always checks the story
  graph, incident records, whitespace, and the clean-room source audit.
- When the pinned native prerequisites are available, run
  `scripts/factory.ps1 verify -RequireNative`; it configures, builds, and runs
  CTest in a disposable build directory.
- QA independently inspects the diff and runs the affected check.  A `done`
  story requires a QA reviewer, date, and reproducible acceptance evidence.
