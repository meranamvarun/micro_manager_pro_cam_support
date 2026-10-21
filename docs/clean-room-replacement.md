# Clean-room replacement assessment

Primary researcher: Malavika S

## Finding

`toupcam.dll_Decompiler.com.zip` is not decompiled source. Its own notes state that native machine code was not decompiled. The archive contains only PE metadata, imports, exports, a manifest, and version-resource data. It therefore cannot be recompiled, optimized, or meaningfully reviewed as source.

The analyzed vendor `toupcam.dll` exports 290 named entry points: 235 public
`Toupcam_*` names, 50 private `Dll*` names, and 5 other names. The vendor
Micro-Manager adapter does not statically import the public API. It loads the SDK
at runtime and resolves exactly eight private names: `DllGigeEnable`,
`DllMmgrName`, `DllCompany`, `DllVersion`, `DllAppName`, `DllEnumCamera`,
`DllGetDefAE`, and `DllOpen`. `DllMmgrName` is an export alias of `DllCamName` at
the same SDK address. The earlier claim that the adapter resolves
`DllPullImage*` directly was incorrect; those names are exported by the SDK but
are not present in the adapter. Recreating the undocumented `Dll*` ABI would
still be brittle and would remain derived from opaque vendor behavior. See the
[binary analysis report](reverse-engineering-report.md) and its linked evidence.

Removing Chinese-language strings, company names, or version resources would not change the code's authorship, provenance, maintenance chain, or supply-chain risk. It must not be presented as converting a vendor binary into independently developed software.

## Defensible replacement

This repository is developing a new Micro-Manager adapter around [LoupeCam](https://github.com/cinderblock/loupecam), an independently implemented USB driver licensed under MIT or Apache-2.0. LoupeCam communicates over WinUSB/libusb and states that it includes no vendor code or binaries.

The replacement is intentionally named `mmgr_dal_LoupeCam.dll`; it does not impersonate `toupcam.dll`. This preserves honest provenance and avoids reliance on ToupTek's private DLL ABI.

## Current support boundary

LoupeCam currently supports USB ID `0547:1142` (AmScope MU1803-HS). The Radical/ProCAM camera used for this work enumerates as `0547:114C` and is identified as an E3ISPM05000KPA-class, 5 MP IMX264 camera. Supporting it requires new, model-specific USB captures and sensor initialization data. Changing only the VID/PID and resolution constants is unsafe and will not initialize the IMX264 sensor correctly.

The adapter therefore defaults to real hardware but also exposes a pre-initialization `Simulation=On` setting for build and integration testing. Until `0547:114C` support is implemented and tested in LoupeCam, the replacement is **not a working driver for this ProCAM camera**.

## Compliance boundary

An organization must evaluate more than source strings:

- source and contributor provenance;
- reproducible build inputs and toolchain provenance;
- dependency SBOM and license obligations;
- signed release artifacts and hashes;
- vulnerability review;
- hardware and firmware origin, if covered by its policy;
- applicable procurement, export-control, privacy, and medical-device rules.

This project can provide source transparency and eliminate the runtime dependency on the vendor SDK. It cannot certify that a particular organization's geographic-origin policy is satisfied, especially if the policy also covers camera hardware or firmware.

## Required evidence before production use

1. Capture and document the `0547:114C` USB protocol using black-box observations and authorized hardware.
2. Add the model and IMX264 sensor programming to LoupeCam with fixtures and regression tests.
3. Build the Rust C ABI and Micro-Manager Device API 75 adapter from pinned source.
4. Verify enumeration, initialization, exposure, gain, resolutions, colour order, frame integrity, disconnect recovery, and sustained acquisition on the physical camera.
5. Compare raw and developed frames against known targets without copying vendor code.
6. Produce hashes, SBOM, dependency licenses, build logs, and provenance attestations.
7. Conduct legal and organizational review before claiming policy compliance.

