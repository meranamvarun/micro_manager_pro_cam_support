# Building the clean-room compatibility pair

This project now has two clean-room targets:

- `toupcam_compat.dll`: a small documented SDK façade backed by LoupeCam;
- `mmgr_dal_Toupcam_compat.dll`: a Micro-Manager Device API 75 adapter using
  that façade.

They are source-built replacements, not recovered vendor source and not a
verified drop-in replacement for the vendor DLLs. In particular, the observed
vendor `DllEnumCamera` record layout and the `0547:114C` camera's USB/sensor
protocol remain unrecovered.

## Prerequisites

- a Device API 75 `mmCoreAndDevices` checkout;
- a pinned LoupeCam checkout that provides `bindings/c/include/loupecam.h`;
- the matching `loupecam_c.dll` and `loupecam_c.dll.lib` for Windows;
- CMake 3.24+ and a 64-bit MSVC build environment.

## Configure and build

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 `
  -DMMCOREANDDEVICES_ROOT=C:\src\mmCoreAndDevices `
  -DLOUPECAM_ROOT=C:\src\loupecam `
  -DLOUPECAM_LIBRARY_DIR=C:\src\loupecam\target\release
cmake --build build --config Release --target toupcam_compat mmgr_dal_Toupcam_compat
```

The adapter output directory also receives `toupcam_compat.dll` and
`loupecam_c.dll`.

## Factory verification

The repository includes a small dependency-aware harness for clean-room work.
It validates the work graph, incident-prevention records, whitespace, and the
source audit every time:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\factory.ps1 verify
```

Set the three build inputs below to include a fresh native configure, Release
build, and simulated adapter smoke test. `-RequireNative` makes missing inputs
an error, which is the appropriate CI gate:

```powershell
$env:MMCOREANDDEVICES_ROOT = 'C:\src\mmCoreAndDevices'
$env:LOUPECAM_ROOT = 'C:\src\loupecam'
$env:LOUPECAM_LIBRARY_DIR = 'C:\src\loupecam\target\release'
powershell -NoProfile -ExecutionPolicy Bypass -File scripts\factory.ps1 verify -RequireNative
```

Use `scripts\factory.ps1 next -Role dev` to see work whose dependencies are
complete. Record hardware observations as reproducible evidence; a successful
source-only run does not demonstrate camera or ABI compatibility.

## Safety boundary

`TOUPCAM_COMPAT_VENDOR_FILENAMES` defaults to `OFF`. Leave it off for normal
development so the artifacts cannot overwrite or shadow the vendor pair. Only
enable it in a separate test directory after hardware validation:

```powershell
cmake -S . -B build-vendor-names -DTOUPCAM_COMPAT_VENDOR_FILENAMES=ON ...
```

Even with matching filenames, this build does not claim the vendor's private
ABI is complete. Its adapter registers the distinct Micro-Manager device name
`ToupcamCompat`; configure that device explicitly.

## Current support

Simulation and hardware paths are delegated to LoupeCam. The target
`0547:114C` / E3ISPM05000KPA-class device will remain unavailable until its
model-specific USB initialization and streaming support are added to LoupeCam
and verified on physical hardware.
