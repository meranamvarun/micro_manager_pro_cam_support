# Project guidance for Claude

## Purpose and scope

This repository develops clean-room Micro-Manager camera support and an
experimental Toupcam compatibility scaffold. It must not be presented as a
replacement for the vendor's `toupcam.dll` or `mmgr_dal_Toupcam.dll` until
binary and hardware compatibility have been demonstrated.

The authoritative project state and next steps are in:

- `docs/next-agent-handoff.md`
- `docs/reverse-engineering-report.md`
- `docs/compatibility-build.md`
- `docs/clean-room-replacement.md`

For structured implementation work, also read `factory/guardrails.md` and use
`scripts/factory.ps1 next` / `scripts/factory.ps1 verify`. The factory tracks
evidence and required checks; it does not establish unsupported hardware or ABI
claims.

Read those before changing Toupcam-related code or claims.

## Clean-room rules

- Do not copy vendor source code, headers, decompiled function bodies, or
  proprietary constants into implementation files.
- Use reverse-engineering observations only to guide independently written,
  documented behavior.
- Keep `TOUPCAM_COMPAT_VENDOR_FILENAMES` disabled by default. Vendor-like
  filenames are only for controlled, isolated experiments.
- Do not claim support for `USB 0547:114C` / `E3ISPM05000KPA`, a complete SDK
  ABI, or a drop-in DLL replacement without reproducible hardware evidence.

## Current compatibility components

- `compat/ToupcamCompat.*` provides an intentionally limited LoupeCam-backed
  façade.
- `adapter/ToupcamCompatAdapter.*` registers `ToupcamCompat` as a
  Micro-Manager device.
- `analysis/ghidra/` is static-analysis evidence, not source material to copy
  into implementation.
- `scripts/ReverseEngineerToupcam.java` regenerates analysis evidence from the
  original binaries with Ghidra.

The observed private SDK names and ABI facts are recorded in the reverse-
engineering report. Do not infer missing parameter layouts, return values, or
calling conventions from names alone.

## Build and test expectations

The project requires these external dependencies:

- CMake and a Windows C++ toolchain
- Micro-Manager Device API matching `MM_API_VERSION=75`
- LoupeCam SDK and its C library

Follow `docs/compatibility-build.md` for configuration. Build artifacts have
not yet been compiled or hardware-tested in this repository's current setup.

Before committing code changes, run the applicable checks:

```powershell
git diff --check
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\audit-clean-room-source.ps1
```

When a supported build environment is available, also configure, build, and
test the relevant CMake targets. Report any unavailable prerequisite instead
of claiming a successful build.

## Suggested development order

1. Establish a reproducible Windows build with pinned dependency versions.
2. Add DLL load/smoke tests and validate against a known LoupeCam-supported
   camera or simulation path.
3. Gather target-camera behavior with real hardware, preserving evidence.
4. Implement and test discovery, properties, ROI, binning, and sequencing one
   behavior at a time.
5. Verify exported ABI details before any isolated vendor-compatibility test.

## Repository hygiene

- Keep generated analysis and source changes scoped to the task.
- Do not commit downloaded SDKs, vendor DLLs, Ghidra projects, tool caches, or
  build directories.
- Preserve unrelated user changes in a dirty working tree.
