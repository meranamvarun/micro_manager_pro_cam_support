# Next-agent handoff

## What is now in `main`

This change set adds a **clean-room, source-only compatibility scaffold** for
the Toupcam/Micro-Manager integration. It does not contain vendor source code
or a drop-in replacement for either vendor DLL.

- `compat/ToupcamCompat.*` is a LoupeCam-backed façade with a small documented
  API and limited observed-name exports.
- `adapter/ToupcamCompatAdapter.*` is a Micro-Manager adapter named
  `ToupcamCompat` that uses that façade.
- `analysis/ghidra/` contains reproducible static-analysis evidence.
- `scripts/ReverseEngineerToupcam.java` regenerates the Ghidra evidence from
  the original, unmodified binaries.
- `docs/reverse-engineering-report.md` is the authoritative technical report.

The default build artifact names are deliberately `toupcam_compat.dll` and
`mmgr_dal_Toupcam_compat.dll`. Do not deploy them over the vendor DLLs.

## Verified facts to preserve

The examined binaries were:

| File | SHA-256 |
| --- | --- |
| `mmgr_dal_Toupcam.dll` | `d195b16939067d3169e66e0595d71f98b86920881f705692bc41e9a0b1a0ffd1` |
| `toupcam.dll` | `40dd6dfc409a790d0be1675ddae53ee4bdfcfc48eea3dedf4730bc34c1f62c27` |

The adapter dynamically resolves exactly these private SDK names:

`DllGigeEnable`, `DllMmgrName`, `DllCompany`, `DllVersion`, `DllAppName`,
`DllEnumCamera`, `DllGetDefAE`, and `DllOpen`.

Important corrections to earlier work:

- The adapter does **not** dynamically resolve `DllPullImage*` functions.
- `DllMmgrName` and `DllCamName` alias the same SDK export (RVA `0xD55470`).
- Enumeration uses observed 264-byte records, but their full ABI/layout is not
  yet known.
- The adapter appends `;registry=` to its wide identifier before calling
  `DllOpen`.
- `DllGetDefAE` was observed writing `350000` and `500`, then returning zero.

Read the linked evidence before changing these statements:

- [Reverse-engineering report](reverse-engineering-report.md)
- [Adapter focused evidence](../analysis/ghidra/adapter-focused.md)
- [SDK focused evidence](../analysis/ghidra/sdk-focused-preanalysis.md)
- [Compatibility build guide](compatibility-build.md)

## Current limitations

- No compiler, CMake installation, Micro-Manager Device API checkout, or
  LoupeCam SDK checkout was available in the implementation environment.
  Therefore neither compatibility DLL has been compiled or hardware-tested.
- The target physical camera (`USB 0547:114C`, model `E3ISPM05000KPA`) is not
  established as supported by LoupeCam. Static strings alone do not prove that
  it can be opened or streamed.
- `DllEnumCamera` intentionally reports unsupported rather than fabricating an
  undocumented 264-byte record.
- The observed vendor export names/signatures and ABI are not fully recreated;
  this is not yet a binary-compatible SDK replacement.
- The new adapter currently supports open/start/snap/exposure/gain only. It
  does not reproduce the vendor adapter's complete property, ROI, binning,
  sequencing, error, or discovery behavior.

## Suggested next steps

1. On a Windows build machine, install Visual Studio Build Tools (C++), CMake,
   the Micro-Manager Device API matching `MM_API_VERSION=75`, and the LoupeCam
   SDK. Follow the configuration command in
   [compatibility-build.md](compatibility-build.md).
2. Build the project unchanged, add a CI or local smoke test that loads both
   compatibility DLLs, and record the exact toolchain/dependency versions.
3. Test `ToupcamCompat` against a known LoupeCam-supported camera (or its
   simulation path) before attempting target-camera support.
4. Use the real `0547:114C` camera to capture load/enumerate/open/stream traces
   and compare observable behavior. Add evidence and tests for each behavior;
   do not guess undocumented record layouts or function signatures.
5. Expand the adapter one feature at a time (discovery, properties, ROI,
   binning, sequencing), with hardware regression tests.
6. Verify all proposed vendor-facing exports, calling conventions, parameter
   sizes, return codes, and ordinal requirements before considering an
   isolated compatibility experiment.

## Guardrails

- Keep the work clean-room: do not copy vendor source, headers, proprietary
  constants, or decompiled bodies into the project.
- Keep vendor-like filenames disabled by default. The CMake option
  `TOUPCAM_COMPAT_VENDOR_FILENAMES` exists only for controlled, isolated tests.
- Do not claim support for the target camera or binary compatibility until it
  is demonstrated by reproducible hardware tests.
- Regenerate analysis with the provided script against the preserved hashes if
  a conclusion needs re-checking.
