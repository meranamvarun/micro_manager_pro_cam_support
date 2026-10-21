# ToupCam binary analysis report

Date: 2026-10-21
Target: the x64 Device API 75 adapter and SDK from `mmgr.20260818`

## Result

The earlier work made the right architectural decision—build a separately
named adapter on an independently implemented driver—but overstated what was
known about the vendor ABI. The replacement is a useful Micro-Manager and
simulation scaffold; it is not a working replacement for the observed
`0547:114C` / E3ISPM05000KPA-class camera.

The vendor adapter dynamically loads the SDK and resolves eight private entry
points. It does **not** resolve `DllPullImage*` itself. Enumeration and opening
go through `DllEnumCamera` and `DllOpen`; the returned opaque camera object is
then used by the adapter's C++ object graph. This means cloning a few exports or
redirecting the DLL name would not reproduce the driver.

## Exact artifacts

| Artifact | Size | SHA-256 |
|---|---:|---|
| `win/x64/75/mmgr_dal_Toupcam.dll` | 434,176 bytes | `d195b16939067d3169e66e0595d71f98b86920881f705692bc41e9a0b1a0ffd1` |
| `win/x64/toupcam.dll` | 31,815,168 bytes | `40dd6dfc409a790d0be1675ddae53ee4bdfcfc48eea3dedf4730bc34c1f62c27` |

Both files are x86-64 PE DLLs, both have a PE timestamp of 2026-08-18, and
neither has an Authenticode signature. The SDK reports file/product version
`60.32327.20260818`; the adapter has no populated version resource.

## Method and boundary

The supplied REA fork was checked out at commit
`1c123493a21cc579ba7532c40e7ef30df45db3e8` (package version 6.2.0). REA's
scoped doctor accepted portable Node 24.18.0, Ghidra 12.1.4, Temurin JDK 21,
and its verified Windows Job Object/DACL/path controls.

REA 6.2.0's experimental Windows Ghidra provider intentionally admits PE
applications but rejects PE shared libraries. The unchanged targets therefore
could not be opened through the REA provider. The analysis did not bypass that
guard or alter the PE headers. Instead, it followed REA's evidence workflow and
used the same verified Ghidra installation directly in read-only/headless mode.
This distinction matters: the linked Markdown files are direct-Ghidra static
evidence, not REA-provider Evidence objects. Static analysis does not establish
runtime USB behavior.

The reproducible extraction script is
[`scripts/ReverseEngineerToupcam.java`](../scripts/ReverseEngineerToupcam.java).

## Observations

### Adapter boundary

The adapter exports the normal Micro-Manager module entry points, including
`InitializeModuleData`, `CreateDevice`, and `DeleteDevice`. It imports
`LoadLibraryW` and `GetProcAddress` but has no static import from `toupcam.dll`.

`InitializeModuleData` searches beside the adapter for the SDK, loads it, and
resolves these exact strings:

- `DllGigeEnable`
- `DllMmgrName`
- `DllCompany`
- `DllVersion`
- `DllAppName`
- `DllEnumCamera`
- `DllGetDefAE`
- `DllOpen`

It calls `DllEnumCamera` with a stack buffer. The recovered loop advances by
`0x108` bytes per enumerated record, so 264 bytes is an observed record stride;
the field layout is not yet recovered. If no cameras are initially returned,
the adapter optionally calls `DllGigeEnable`, waits two seconds, and enumerates
again.

The camera-construction path re-enumerates, matches the requested wide-string
identifier, appends `;registry=`, and passes that string to `DllOpen`. A nonzero
return is stored in a reference-counted wrapper. Later acquisition behavior is
therefore reached through the opaque returned object and internal C++ calls,
not by adapter-side `GetProcAddress("DllPullImage...")` calls.

### SDK boundary

The SDK contains 290 named exports:

- 235 `Toupcam_*` public API exports;
- 50 private `Dll*` exports;
- 5 other exports.

All eight names requested by the adapter exist. `DllMmgrName` and `DllCamName`
are aliases at RVA `0xD55470`, which is why Ghidra displays `DllCamName` as the
primary symbol. The SDK also exports private acquisition helpers such as
`DllPullImageV3`, `DllPullImageV4`, and `DllStartPullModeWithCallback`, but the
adapter does not resolve those names directly.

Focused decompilation shows:

- `DllAppName(char*)` stores an application-name string in SDK global state.
- `DllEnumCamera(void*)` performs one-time initialization and delegates to an
  internal enumeration routine that fills the caller's buffer.
- `DllGetDefAE(uint32_t*, uint16_t*)` writes observed defaults `350000` and
  `500` when the corresponding pointers are non-null, then returns zero.
- `DllOpen(wchar_t*)` performs one-time initialization, parses the supplied
  identifier, and dispatches to internal open routines. Its true return type is
  pointer-sized even though type recovery on the unanalyzed SDK body rendered
  it as `void`.

The SDK contains the model string `E3ISPM05000KPA`, along with many other
E3ISPM models. A model string proves inclusion in a model table, not that the
connected device was exercised during this static analysis.

## Assessment of the existing replacement

What is good:

- honest, separately named `mmgr_dal_LoupeCam.dll` provenance;
- no attempt to disguise or patch the vendor binary;
- a small, readable adapter boundary around LoupeCam's C ABI;
- pre-initialization simulation mode and a basic simulated snap smoke test;
- clear documentation that `0547:114C` is not supported yet.

What is incomplete or weak:

- LoupeCam's documented hardware support does not include the target
  `0547:114C` device, so the real path is unverified by construction;
- the smoke test exercises simulation only and proves neither USB enumeration
  nor acquisition;
- resolution is hard-coded (`size = 0` on hardware), binning is fixed at 1,
  and ROI/sequence acquisition are unsupported;
- exposure, gain, colour order, frame integrity, disconnect recovery, and
  sustained streaming have no physical-camera tests;
- `OpenCamera()` does not free `camera_` if `loupecam_camera_info()` fails,
  leaving cleanup until shutdown/destruction and risking a leak on an
  initialization retry;
- exposure state is written before acquiring `cameraLock_`, and SDK set-exposure
  failures are silently ignored because the Micro-Manager override returns
  `void`;
- the exposed Micro-Manager exposure property has no property action, so its
  UI/property synchronization should be verified against Device API 75 rather
  than assumed from the virtual `SetExposure` method;
- no source build, hardware test, or ABI comparison in this pass demonstrates
  functional equivalence to the vendor adapter.

Overall: the work is a sound clean-room starting point, not a completed driver.
Its strongest part is provenance and separation; its weakest part is the gap
between a simulated generic camera adapter and model-specific USB/sensor
initialization.

## Evidence files

- [Adapter inventory](../analysis/ghidra/adapter-inventory.md)
- [Adapter focused pseudocode](../analysis/ghidra/adapter-focused.md)
- [SDK export inventory](../analysis/ghidra/sdk-inventory.md)
- [SDK focused pseudocode](../analysis/ghidra/sdk-focused-preanalysis.md)

Addresses are Ghidra image addresses for these exact hashes. Decompiled text is
reconstructed pseudocode, not original source.

## Next work needed for a real replacement

1. Capture authorized USB traffic for enumeration, open, initialization,
   exposure changes, streaming, and shutdown on the physical `0547:114C`
   camera.
2. Correlate those captures with public LoupeCam abstractions without copying
   vendor code or private constants from decompiled implementation.
3. Add an explicit `0547:114C` model implementation and IMX264 initialization
   to LoupeCam, with a hardware fixture and regression captures.
4. Fix the adapter cleanup/threading/property issues above and add tests for
   error paths, exposure/gain changes, colour layout, and reconnect behavior.
5. Validate Device API 75 loading and sustained acquisition in Micro-Manager,
   then publish hashes, build logs, SBOM, licenses, and hardware-test results.
