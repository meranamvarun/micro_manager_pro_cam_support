# Micro-Manager support for ToupTek and rebranded ProCAM cameras

A practical Windows guide for using ToupTek USB microscope cameras—and compatible rebranded cameras such as some Radical ProCAM models—with [Micro-Manager](https://micro-manager.org/).

This is an independent community guide. It is not affiliated with or endorsed by ToupTek, Radical Scientific, or the Micro-Manager project. Do not redistribute proprietary camera drivers in this repository; download them from the manufacturer.

## Known working setup

The workflow was verified with:

- Windows 11, 64-bit
- Micro-Manager 2.0.3 (`20261014`)
- MMCore 12.5.1
- Device API version 75
- A Radical-branded USB3 camera identified by Windows as `USB\VID_0547&PID_114C`
- Underlying ToupTek model: `E3ISPM05000KPA`

The same approach should work for other ToupTek USB cameras supported by ToupTek's `Toupcam` Micro-Manager adapter. Always match the adapter to your installed Micro-Manager **Device API version**.

## Symptoms this guide fixes

- Micro-Manager shows a striped test image: `MMConfig_demo.cfg` and `DemoCamera` are loaded.
- OpenCVgrabber lists only the laptop webcam.
- Windows detects the microscope camera merely as `USB3.0 Camera` or a `WinUSB Device`.
- Selecting different OpenCV camera numbers never finds the microscope camera.
- A copied camera adapter is shown as unavailable or crashes Micro-Manager.

For these cameras, the native `Toupcam` adapter is preferable to OpenCVgrabber because it exposes camera-specific controls without depending on DirectShow enumeration.

## 1. Confirm that Windows detects the camera

Connect the camera directly to a USB 3 port, avoiding an unpowered hub. In PowerShell, run:

```powershell
Get-PnpDevice -PresentOnly |
    Where-Object { $_.FriendlyName -match 'Toup|ProCAM|USB3.0 Camera' } |
    Format-Table Status, Class, FriendlyName, InstanceId -AutoSize
```

An `OK` device with an instance ID beginning `USB\VID_0547` is commonly a ToupTek-family camera. The exact product ID identifies the model or OEM variant.

You can also run the repository's read-only diagnostic:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\inspect-touptek.ps1
```

## 2. Find the Micro-Manager Device API version

In Micro-Manager, open **Help → About Micro-Manager** and note:

- whether the program is 32-bit or 64-bit;
- the **Device API version**.

Do not infer the Device API from the Micro-Manager release number. ToupTek supplies different adapter builds for API versions such as 68–75. A mismatched adapter may fail to load or crash Micro-Manager.

On Windows, the version is also recorded near the beginning of the latest file under:

```text
C:\Program Files\Micro-Manager-2.0\CoreLogs
```

Example:

```text
MM Studio version: 2.0.3 20261014
MMCore version 12.5.1
Device API version 75, Module API version 10
```

## 3. Download the official adapter

1. Open the [ToupTek Software & Driver Download Center](https://touptekphotonics.com/download/).
2. Select **Third-Party Drivers → Micro-Manager Drivers**.
3. Download **mmgr — Micro-Manager library for USB cameras**.
4. Extract the ZIP to a temporary folder.

Avoid driver-aggregator websites. The adapter package contains native executable code and should come directly from ToupTek.

## 4. Install the correct files

Close Micro-Manager completely before copying the files.

For 64-bit Windows and Device API 75, copy:

```text
win\x64\75\mmgr_dal_Toupcam.dll
win\x64\toupcam.dll
```

into the Micro-Manager installation directory, normally:

```text
C:\Program Files\Micro-Manager-2.0
```

Administrator permission may be required. For a different Device API, replace `75` with the number shown in **Help → About Micro-Manager**. For 32-bit Micro-Manager, use the package's `win\x86` files instead.

If upgrading an existing adapter, first keep a copy of the old `mmgr_dal_Toupcam.dll` and `toupcam.dll` outside the Micro-Manager directory.

## 5. Configure Micro-Manager

1. Start Micro-Manager.
2. Choose **None** at the initial configuration prompt, or open **Devices/Config → Hardware Configuration Wizard**.
3. Create a new configuration.
4. Under **Available Devices**, expand **Toupcam**.
5. Add the detected camera.
6. Set it as the Core camera if prompted.
7. Finish the wizard and save the configuration, for example as `MMConfig_Toupcam.cfg`.
8. Click **Live** to test the image.

Do not select `DemoCamera`, and do not select `Undefined` in OpenCVgrabber.

## 6. Recommended acquisition settings

Open **Device Property Browser** and begin with:

- full sensor resolution;
- colour/RGB output for brightfield pathology;
- binning `1 × 1`;
- manual exposure and gain;
- one-time white balance on a clear part of the slide, followed by manual/fixed white balance;
- minimal digital sharpening, denoising, gamma manipulation, or saturation enhancement.

For scientific records, retain an untouched TIFF/OME-TIFF master and create separate annotated derivatives. Calibrate pixel size separately for every objective, resolution, binning mode, and camera adapter combination.

## Alternative: ToupView

ToupTek's [ToupView](https://touptekphotonics.com/download/) can confirm that the camera and USB connection work independently of Micro-Manager. If ToupView sees the camera but Micro-Manager does not, the usual causes are:

- the `Toupcam` adapter files were copied to the wrong folder;
- 32-bit/64-bit architecture mismatch;
- Device API version mismatch;
- Micro-Manager was open while its DLLs were copied;
- the camera is already in use by ToupView, Windows Camera, Teams, Zoom, or another program.

Close ToupView before opening the same camera in Micro-Manager.

## Troubleshooting

| Problem | Likely cause | What to do |
|---|---|---|
| Striped gradient image | Demo configuration is loaded | Create a new configuration and add `Toupcam` |
| Laptop webcam opens | OpenCVgrabber selected the DirectShow webcam | Use the native `Toupcam` adapter |
| `Toupcam` is absent | Adapter files are missing or in the wrong directory | Copy both required DLLs into the Micro-Manager installation directory |
| Adapter is unavailable | Missing dependency or architecture/API mismatch | Recheck x64/x86 and Device API version |
| Micro-Manager crashes during loading | Usually an API-version mismatch | Remove the newly copied adapter and install the matching build |
| Black image | Camera port is not receiving light, cap/beam selector issue, or exposure too low | Check the microscope light path and increase exposure |
| Camera busy or initialization error | Another application owns the camera | Close all other camera software, then reconnect the camera |
| Poor frame rate | USB 2 connection, hub, or excessive resolution | Use a direct USB 3 port and test a smaller ROI/resolution |

## Useful official links

- [ToupTek download center](https://touptekphotonics.com/download/)
- [ToupTek E3ISPM05000KPA product page](https://www.touptekphotonics.com/Microscopy/E3ISPM/E3ISPM05000KPA)
- [Micro-Manager configuration guide](https://micro-manager.org/Micro-Manager_Configuration_Guide)
- [Micro-Manager device support](https://micro-manager.org/Device_Support)
- [Radical ProCAM software information](https://radicalindia.com/procam-software.php)

## Research attribution

Primary researcher: Malavika S

AI assistance was used as a drafting and technical-verification aid. Device selection, image acquisition, scientific interpretation, and the final research record remain the responsibility of the researcher.
