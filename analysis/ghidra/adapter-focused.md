# Direct Ghidra static-analysis evidence

- Executable: `/C:/Users/malai/Downloads/mmgr.20260818/win/x64/75/mmgr_dal_Toupcam.dll`
- SHA-256: `d195b16939067d3169e66e0595d71f98b86920881f705692bc41e9a0b1a0ffd1`
- Format: `Portable Executable (PE)`
- Language: `x86:LE:64:default`
- Image base: `180000000`

## Imported symbols

- `KERNEL32.DLL::InitializeCriticalSection`
- `KERNEL32.DLL::FindClose`
- `KERNEL32.DLL::WaitForSingleObject`
- `KERNEL32.DLL::GetModuleHandleExA`
- `KERNEL32.DLL::DisableThreadLibraryCalls`
- `KERNEL32.DLL::LoadLibraryW`
- `KERNEL32.DLL::LeaveCriticalSection`
- `KERNEL32.DLL::GetProcAddress`
- `KERNEL32.DLL::DeleteCriticalSection`
- `KERNEL32.DLL::WriteConsoleW`
- `KERNEL32.DLL::CreateFileW`
- `KERNEL32.DLL::GetModuleFileNameW`
- `KERNEL32.DLL::FindNextFileW`
- `KERNEL32.DLL::OutputDebugStringA`
- `KERNEL32.DLL::EnterCriticalSection`
- `KERNEL32.DLL::FindFirstFileW`
- `KERNEL32.DLL::CreateThread`
- `KERNEL32.DLL::Sleep`
- `KERNEL32.DLL::WideCharToMultiByte`
- `KERNEL32.DLL::QueryPerformanceCounter`
- `KERNEL32.DLL::QueryPerformanceFrequency`
- `KERNEL32.DLL::ReleaseSRWLockExclusive`
- `KERNEL32.DLL::AcquireSRWLockExclusive`
- `KERNEL32.DLL::TryAcquireSRWLockExclusive`
- `KERNEL32.DLL::GetCurrentThreadId`
- `KERNEL32.DLL::SleepConditionVariableSRW`
- `KERNEL32.DLL::GetTickCount64`
- `KERNEL32.DLL::WakeAllConditionVariable`
- `KERNEL32.DLL::CloseHandle`
- `KERNEL32.DLL::WaitForSingleObjectEx`
- `KERNEL32.DLL::GetExitCodeThread`
- `KERNEL32.DLL::InitializeCriticalSectionEx`
- `KERNEL32.DLL::EncodePointer`
- `KERNEL32.DLL::DecodePointer`
- `KERNEL32.DLL::MultiByteToWideChar`
- `KERNEL32.DLL::LCMapStringEx`
- `KERNEL32.DLL::GetSystemTimeAsFileTime`
- `KERNEL32.DLL::GetModuleHandleW`
- `KERNEL32.DLL::GetStringTypeW`
- `KERNEL32.DLL::GetCPInfo`
- `KERNEL32.DLL::RtlCaptureContext`
- `KERNEL32.DLL::RtlLookupFunctionEntry`
- `KERNEL32.DLL::RtlVirtualUnwind`
- `KERNEL32.DLL::UnhandledExceptionFilter`
- `KERNEL32.DLL::SetUnhandledExceptionFilter`
- `KERNEL32.DLL::GetCurrentProcess`
- `KERNEL32.DLL::TerminateProcess`
- `KERNEL32.DLL::IsProcessorFeaturePresent`
- `KERNEL32.DLL::IsDebuggerPresent`
- `KERNEL32.DLL::GetStartupInfoW`
- `KERNEL32.DLL::GetCurrentProcessId`
- `KERNEL32.DLL::InitializeSListHead`
- `KERNEL32.DLL::RtlUnwindEx`
- `KERNEL32.DLL::InterlockedFlushSList`
- `KERNEL32.DLL::RtlPcToFileHeader`
- `KERNEL32.DLL::RaiseException`
- `KERNEL32.DLL::GetLastError`
- `KERNEL32.DLL::SetLastError`
- `KERNEL32.DLL::InitializeCriticalSectionAndSpinCount`
- `KERNEL32.DLL::TlsAlloc`
- `KERNEL32.DLL::TlsGetValue`
- `KERNEL32.DLL::TlsSetValue`
- `KERNEL32.DLL::TlsFree`
- `KERNEL32.DLL::FreeLibrary`
- `KERNEL32.DLL::LoadLibraryExW`
- `KERNEL32.DLL::ExitThread`
- `KERNEL32.DLL::FreeLibraryAndExitThread`
- `KERNEL32.DLL::GetModuleHandleExW`
- `KERNEL32.DLL::ExitProcess`
- `KERNEL32.DLL::HeapAlloc`
- `KERNEL32.DLL::HeapFree`
- `KERNEL32.DLL::GetStdHandle`
- `KERNEL32.DLL::GetFileType`
- `KERNEL32.DLL::FlsAlloc`
- `KERNEL32.DLL::FlsGetValue`
- `KERNEL32.DLL::FlsSetValue`
- `KERNEL32.DLL::FlsFree`
- `KERNEL32.DLL::VirtualProtect`
- `KERNEL32.DLL::LCMapStringW`
- `KERNEL32.DLL::GetLocaleInfoW`
- `KERNEL32.DLL::IsValidLocale`
- `KERNEL32.DLL::GetUserDefaultLCID`
- `KERNEL32.DLL::EnumSystemLocalesW`
- `KERNEL32.DLL::SetFilePointerEx`
- `KERNEL32.DLL::HeapReAlloc`
- `KERNEL32.DLL::FindFirstFileExW`
- `KERNEL32.DLL::IsValidCodePage`
- `KERNEL32.DLL::GetACP`
- `KERNEL32.DLL::GetOEMCP`
- `KERNEL32.DLL::GetCommandLineA`
- `KERNEL32.DLL::GetCommandLineW`
- `KERNEL32.DLL::GetEnvironmentStringsW`
- `KERNEL32.DLL::FreeEnvironmentStringsW`
- `KERNEL32.DLL::GetProcessHeap`
- `KERNEL32.DLL::SetStdHandle`
- `KERNEL32.DLL::FlushFileBuffers`
- `KERNEL32.DLL::WriteFile`
- `KERNEL32.DLL::GetConsoleOutputCP`
- `KERNEL32.DLL::GetConsoleMode`
- `KERNEL32.DLL::HeapSize`
- `USER32.DLL::GetForegroundWindow`
- `USER32.DLL::GetActiveWindow`
- `COMDLG32.DLL::GetOpenFileNameA`
- `COMDLG32.DLL::GetSaveFileNameA`
- `ADVAPI32.DLL::RegOpenKeyExW`
- `ADVAPI32.DLL::RegQueryValueExW`
- `ADVAPI32.DLL::RegCloseKey`

## Exported entry points

- `18000a3e0` `CreateDevice`
- `180009b60` `DeleteDevice`
- `180006d50` `GetDeviceDescription`
- `180006be0` `GetDeviceInterfaceVersion`
- `180006c20` `GetDeviceName`
- `180006cb0` `GetDeviceType`
- `180006bd0` `GetModuleVersion`
- `180006bf0` `GetNumberOfDevices`
- `180009e80` `InitializeModuleData`
- `180026164` `entry`

## Relevant strings and static references

- `180059ad8 DllGigeEnable`
  - referenced at `18000a0f4 in InitializeModuleData`
- `180059b18 DllMmgrName`
  - referenced at `18000a19c in InitializeModuleData`
- `180059b28 DllCompany`
  - referenced at `18000a1af in InitializeModuleData`
- `180059b38 DllVersion`
  - referenced at `18000a1c6 in InitializeModuleData`
- `180059b48 DllAppName`
  - referenced at `18000a1dd in InitializeModuleData`
- `180059b58 DllEnumCamera`
  - referenced at `18000a1f4 in InitializeModuleData`
- `180059b68 DllGetDefAE`
  - referenced at `18000a20f in InitializeModuleData`
- `180059d20 DllOpen`
  - referenced at `18000b660 in FUN_18000b610`

## Selected function pseudocode

Decompiler output is reconstructed pseudocode, not original source.

### `InitializeModuleData` at `180009e80`

```c

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void InitializeModuleData
               (undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  bool bVar1;
  char *pcVar2;
  short *psVar3;
  int iVar4;
  uint uVar5;
  undefined1 (*pauVar6) [16];
  HANDLE hFindFile;
  INT_PTR IVar7;
  FARPROC pFVar8;
  FARPROC pFVar9;
  FARPROC pFVar10;
  FARPROC pFVar11;
  longlong *plVar12;
  longlong lVar13;
  short *psVar14;
  ulonglong uVar15;
  undefined1 *puVar16;
  char *pcVar17;
  undefined1 *puVar18;
  char *pcVar19;
  WCHAR *pWVar20;
  undefined1 auStack_8df8 [32];
  undefined *local_8dd8;
  _WIN32_FIND_DATAW local_8dc8;
  undefined1 local_8b78 [33792];
  undefined1 local_778 [16];
  undefined4 local_768;
  undefined2 local_764;
  undefined8 local_758;
  undefined8 uStack_750;
  undefined8 local_748;
  undefined8 uStack_740;
  undefined8 local_738;
  undefined8 uStack_730;
  undefined8 local_728;
  undefined8 uStack_720;
  undefined8 local_718;
  undefined8 uStack_710;
  undefined8 local_708;
  undefined8 uStack_700;
  undefined8 local_6f8;
  undefined8 uStack_6f0;
  undefined8 local_6e8;
  undefined8 uStack_6e0;
  WCHAR local_658 [264];
  WCHAR local_448 [263];
  short sStack_23a;
  WCHAR local_238 [6];
  short local_22c [258];
  ulonglong local_28;
  undefined8 local_20;

                    /* 0x9e80  9  InitializeModuleData */
  local_20 = 0x180009ea6;
  local_28 = DAT_1800650c0 ^ (ulonglong)auStack_8df8;
  if ((DAT_180068448 & 0x8000) != 0) {
    FUN_180009c70(0x180058b58,"InitializeModuleData",param_3,param_4);
  }
  if (DAT_180068440 != (HMODULE)0x0) {
    return;
  }
  FUN_180048e10((undefined1 (*) [32])local_448,0,0x208);
  FUN_180048e10((undefined1 (*) [32])local_658,0,0x208);
  GetModuleFileNameW(DAT_180068428,local_658,0x104);
  lVar13 = 0;
  if (local_658[0] != L'\0') {
    local_778._0_8_ = _DAT_180059aa0;
    local_778._8_8_ = _UNK_180059aa8;
    local_768 = DAT_180059ab0;
    local_764 = DAT_180059ab4;
    pauVar6 = FUN_180027a84((undefined1 (*) [16])local_658,&local_778);
    if (pauVar6 != (undefined1 (*) [16])0x0) {
      *(undefined2 *)*pauVar6 = 0;
      do {
        psVar3 = (short *)((longlong)local_658 + lVar13);
        *(short *)((longlong)local_238 + lVar13) = *psVar3;
        lVar13 = lVar13 + 2;
      } while (*psVar3 != 0);
      psVar3 = &sStack_23a;
      do {
        psVar14 = psVar3;
        psVar3 = psVar14 + 1;
      } while (*psVar3 != 0);
      *(undefined8 *)psVar3 = _DAT_180059ab8;
      *(undefined4 *)(psVar14 + 5) = DAT_180059ac0;
      psVar14[7] = DAT_180059ac4;
      FUN_180048e10((undefined1 (*) [32])&local_8dc8,0,0x250);
      hFindFile = FindFirstFileW(local_238,&local_8dc8);
      if (hFindFile != (HANDLE)0x0) {
        if (((local_8dc8.dwFileAttributes & 0x10) == 0) &&
           (iVar4 = FUN_180037b80(local_8dc8.cFileName,(WCHAR *)(pauVar6[1] + 4)), iVar4 == 0)) {
LAB_18000a131:
          pWVar20 = local_8dc8.cFileName;
          FUN_180008ed0(local_448,0x180059ac8,local_658,pWVar20);
          FindClose(hFindFile);
          if ((DAT_180068448 & 0x8000) != 0) {
            FUN_180009c70(0x180059b00,"InitializeModuleData",local_448,pWVar20);
          }
          DAT_180068440 = LoadLibraryW(local_448);
          if (DAT_180068440 == (HMODULE)0x0) {
            return;
          }
          pFVar8 = GetProcAddress(DAT_180068440,"DllMmgrName");
          pFVar9 = GetProcAddress(DAT_180068440,"DllCompany");
          pFVar10 = GetProcAddress(DAT_180068440,"DllVersion");
          pFVar11 = GetProcAddress(DAT_180068440,"DllAppName");
          DAT_180068420 = GetProcAddress(DAT_180068440,"DllEnumCamera");
          DAT_180068438 = GetProcAddress(DAT_180068440,"DllGetDefAE");
          if (pFVar11 != (FARPROC)0x0) {
            (*pFVar11)(&DAT_180059b74);
          }
          if (pFVar8 != (FARPROC)0x0) {
            IVar7 = (*pFVar8)();
            FUN_180008f30((undefined1 *)&local_758,0x180059b7c,IVar7,pWVar20);
            DAT_180068458 = FUN_180037a60((longlong)&local_758);
          }
          if (pFVar9 != (FARPROC)0x0) {
            IVar7 = (*pFVar9)();
            FUN_180008f30((undefined1 *)&local_758,0x180059b7c,IVar7,pWVar20);
            DAT_180068430 = FUN_180037a60((longlong)&local_758);
          }
          if (pFVar10 != (FARPROC)0x0) {
            DAT_180068450 = (undefined *)(*pFVar10)();
          }
        }
        else {
          iVar4 = FindNextFileW(hFindFile,&local_8dc8);
          while (iVar4 != 0) {
            if (((local_8dc8.dwFileAttributes & 0x10) == 0) &&
               (iVar4 = FUN_180037b80(local_8dc8.cFileName,(WCHAR *)(pauVar6[1] + 4)), iVar4 == 0))
            goto LAB_18000a131;
            iVar4 = FindNextFileW(hFindFile,&local_8dc8);
          }
          FindClose(hFindFile);
        }
      }
    }
  }
  if (DAT_180068440 == (HMODULE)0x0) {
    return;
  }
  if (DAT_180068458 == (char *)0x0) {
    return;
  }
  if (DAT_180068430 == (char *)0x0) {
    return;
  }
  pcVar17 = DAT_180068458;
  pcVar19 = DAT_180068430;
  if ((DAT_180068448 & 0x8000) != 0) {
    local_8dd8 = &DAT_180059b80;
    if (DAT_180068450 != (undefined *)0x0) {
      local_8dd8 = DAT_180068450;
    }
    FUN_180009c70(0x180059b88,"InitializeModuleData",DAT_180068458,DAT_180068430);
  }
  IVar7 = (*DAT_180068420)(local_8b78);
  uVar5 = (uint)IVar7;
  if (uVar5 == 0) {
    pFVar8 = GetProcAddress(DAT_180068440,"DllGigeEnable");
    if ((pFVar8 == (FARPROC)0x0) || (IVar7 = (*pFVar8)(0,0), (int)IVar7 != 0)) {
      bVar1 = false;
    }
    else {
      Sleep(2000);
      bVar1 = true;
    }
    if (bVar1) {
      IVar7 = (*DAT_180068420)(local_8b78);
      uVar5 = (uint)IVar7;
      if (uVar5 != 0) goto LAB_18000a2ed;
    }
    pcVar2 = DAT_180068458;
    pcVar19 = DAT_180068430;
    plVar12 = (longlong *)FUN_180006b60();
    FUN_180006920(plVar12,(undefined8 *)pcVar2,pcVar17,pcVar19);
  }
  else {
LAB_18000a2ed:
    puVar16 = local_8b78;
    uVar15 = (ulonglong)uVar5;
    do {
      local_758 = 0;
      uStack_750 = 0;
      local_748 = 0;
      uStack_740 = 0;
      local_738 = 0;
      uStack_730 = 0;
      local_728 = 0;
      uStack_720 = 0;
      local_718 = 0;
      uStack_710 = 0;
      local_708 = 0;
      uStack_700 = 0;
      local_6f8 = 0;
      uStack_6f0 = 0;
      local_6e8 = 0;
      uStack_6e0 = 0;
      puVar18 = puVar16;
      FUN_180008f30((undefined1 *)&local_758,0x180059b7c,puVar16,pcVar19);
      pcVar19 = DAT_180068430;
      plVar12 = (longlong *)FUN_180006b60();
      FUN_180006920(plVar12,&local_758,puVar18,pcVar19);
      puVar16 = puVar16 + 0x108;
      uVar15 = uVar15 - 1;
    } while (uVar15 != 0);
  }
  plVar12 = (longlong *)FUN_180025980(0x10);
  *plVar12 = 0;
  plVar12[1] = 0;
  lVar13 = FUN_180025980(0x50);
  *(longlong *)lVar13 = lVar13;
  *(longlong *)(lVar13 + 8) = lVar13;
  *(longlong *)(lVar13 + 0x10) = lVar13;
  *(undefined2 *)(lVar13 + 0x18) = 0x101;
  *plVar12 = lVar13;
  DAT_180068418 = plVar12;
  return;
}


```
### `FUN_18000b610` at `18000b610`

```c

/* WARNING: Function: __chkstk replaced with injection: alloca_probe */
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */

longlong *
FUN_18000b610(undefined8 param_1,longlong *param_2,undefined8 *param_3,undefined8 *param_4)

{
  longlong *plVar1;
  longlong *plVar2;
  int *piVar3;
  longlong lVar4;
  int iVar5;
  char cVar6;
  short sVar7;
  short sVar8;
  undefined1 (*pauVar9) [32];
  longlong *plVar10;
  uint uVar11;
  undefined8 uVar12;
  INT_PTR IVar13;
  longlong lVar14;
  short *psVar15;
  longlong *plVar16;
  undefined1 (*pauVar17) [32];
  undefined8 *******pppppppuVar18;
  undefined8 *puVar19;
  undefined8 *puVar20;
  undefined8 *puVar21;
  ulonglong uVar23;
  uint uVar25;
  ulonglong uVar26;
  undefined1 (*pauVar27) [32];
  undefined1 auStackY_85f8 [32];
  FARPROC local_85b0 [2];
  undefined8 *******local_85a0 [2];
  longlong local_8590;
  ulonglong local_8588;
  longlong local_8580 [4];
  longlong *local_8560;
  longlong *local_8558;
  short local_8548 [64];
  undefined1 local_84c8 [2];
  short asStack_84c6 [16831];
  short local_148 [128];
  ulonglong local_48;
  undefined8 uStack_40;
  undefined8 *puVar22;
  ulonglong uVar24;

  pauVar9 = DAT_180068418;
  uStack_40 = 0x18000b632;
  local_48 = DAT_1800650c0 ^ (ulonglong)auStackY_85f8;
  puVar19 = param_4;
  local_85b0[0] = GetProcAddress(DAT_180068440,"DllOpen");
  uVar11 = (*DAT_180068420)(local_8548);
  if (uVar11 == 0) {
    *param_2 = 0;
    param_2[1] = 0;
    return param_2;
  }
  FUN_180048e10((undefined1 (*) [32])local_148,0,0x100);
  if (0xf < (ulonglong)param_3[3]) {
    param_3 = (undefined8 *)*param_3;
  }
  FUN_180008ed0(local_148,0x180059d18,param_3,puVar19);
  uVar26 = 0;
  if (uVar11 != 0) {
    do {
      psVar15 = local_148;
      param_3 = (undefined8 *)((longlong)local_8548 + (uVar26 * 0x108 - (longlong)psVar15));
      do {
        sVar7 = *psVar15;
        sVar8 = *(short *)((longlong)psVar15 + (longlong)param_3);
        if (sVar7 != sVar8) break;
        psVar15 = psVar15 + 1;
      } while (sVar8 != 0);
      if (sVar7 == sVar8) {
        if (-1 < (int)uVar26) goto LAB_18000b72a;
        break;
      }
      uVar25 = (int)uVar26 + 1;
      uVar26 = (ulonglong)uVar25;
    } while (uVar25 < uVar11);
  }
  uVar26 = 0;
LAB_18000b72a:
  pauVar27 = (undefined1 (*) [32])(local_84c8 + uVar26 * 0x108);
  puVar19 = *(undefined8 **)*pauVar9;
  cVar6 = *(char *)((longlong)puVar19[1] + 0x19);
  puVar22 = (undefined8 *)puVar19[1];
  while (cVar6 == '\0') {
    puVar21 = (undefined8 *)0xffffffffffffffff;
    do {
      puVar20 = (undefined8 *)((longlong)puVar21 + 1);
      lVar14 = (longlong)puVar21 * 2;
      puVar21 = puVar20;
    } while (*(short *)(*pauVar27 + lVar14 + 2) != 0);
    pauVar17 = (undefined1 (*) [32])(puVar22 + 4);
    if (7 < (ulonglong)puVar22[7]) {
      pauVar17 = (undefined1 (*) [32])puVar22[4];
    }
    puVar21 = (undefined8 *)puVar22[6];
    param_3 = puVar21;
    if (puVar20 < puVar21) {
      param_3 = puVar20;
    }
    uVar12 = FUN_180008f90(pauVar17,pauVar27,(ulonglong)param_3);
    if ((int)uVar12 == 0) {
      if (puVar21 < puVar20) goto LAB_18000b886;
LAB_18000b7a1:
      puVar21 = (undefined8 *)*puVar22;
      puVar19 = puVar22;
    }
    else {
      if (-1 < (int)uVar12) goto LAB_18000b7a1;
LAB_18000b886:
      puVar21 = (undefined8 *)puVar22[2];
    }
    puVar22 = puVar21;
    cVar6 = *(char *)((longlong)puVar21 + 0x19);
  }
  puVar22 = (undefined8 *)0xffffffffffffffff;
  if (*(char *)((longlong)puVar19 + 0x19) == '\0') {
    do {
      puVar21 = (undefined8 *)((longlong)puVar22 + 1);
      lVar14 = (longlong)puVar22 * 2;
      puVar22 = puVar21;
    } while (*(short *)(*pauVar27 + lVar14 + 2) != 0);
    pauVar17 = (undefined1 (*) [32])(puVar19 + 4);
    if (7 < (ulonglong)puVar19[7]) {
      pauVar17 = (undefined1 (*) [32])puVar19[4];
    }
    puVar22 = (undefined8 *)puVar19[6];
    param_3 = puVar22;
    if (puVar21 < puVar22) {
      param_3 = puVar21;
    }
    uVar12 = FUN_180008f90(pauVar17,pauVar27,(ulonglong)param_3);
    if ((int)uVar12 == 0) {
      if ((puVar22 < puVar21) || (puVar22 == puVar21)) goto LAB_18000b89a;
    }
    else if ((int)uVar12 < 1) {
LAB_18000b89a:
      if (puVar19 != *(undefined8 **)*pauVar9) {
        psVar15 = local_8548 + uVar26 * 0x84;
        lVar14 = 2;
        do {
          uVar12 = *(undefined8 *)(psVar15 + 4);
          *param_4 = *(undefined8 *)psVar15;
          param_4[1] = uVar12;
          uVar12 = *(undefined8 *)(psVar15 + 0xc);
          param_4[2] = *(undefined8 *)(psVar15 + 8);
          param_4[3] = uVar12;
          uVar12 = *(undefined8 *)(psVar15 + 0x14);
          param_4[4] = *(undefined8 *)(psVar15 + 0x10);
          param_4[5] = uVar12;
          uVar12 = *(undefined8 *)(psVar15 + 0x1c);
          param_4[6] = *(undefined8 *)(psVar15 + 0x18);
          param_4[7] = uVar12;
          uVar12 = *(undefined8 *)(psVar15 + 0x24);
          param_4[8] = *(undefined8 *)(psVar15 + 0x20);
          param_4[9] = uVar12;
          uVar12 = *(undefined8 *)(psVar15 + 0x2c);
          param_4[10] = *(undefined8 *)(psVar15 + 0x28);
          param_4[0xb] = uVar12;
          uVar12 = *(undefined8 *)(psVar15 + 0x34);
          param_4[0xc] = *(undefined8 *)(psVar15 + 0x30);
          param_4[0xd] = uVar12;
          puVar22 = param_4 + 0x10;
          uVar12 = *(undefined8 *)(psVar15 + 0x3c);
          param_4[0xe] = *(undefined8 *)(psVar15 + 0x38);
          param_4[0xf] = uVar12;
          psVar15 = psVar15 + 0x40;
          lVar14 = lVar14 + -1;
          param_4 = puVar22;
        } while (lVar14 != 0);
        *puVar22 = *(undefined8 *)psVar15;
        *param_2 = 0;
        param_2[1] = 0;
        if (puVar19[9] != 0) {
          LOCK();
          piVar3 = (int *)(puVar19[9] + 8);
          *piVar3 = *piVar3 + 1;
          UNLOCK();
        }
        *param_2 = puVar19[8];
        param_2[1] = puVar19[9];
        return param_2;
      }
    }
  }
  FUN_18001d110(local_85a0,(undefined8 *)&DAT_180059d28);
  lVar14 = local_8590;
  uVar24 = 0xffffffffffffffff;
  do {
    uVar23 = uVar24 + 1;
    lVar4 = uVar24 * 2;
    uVar24 = uVar23;
  } while (*(short *)(*pauVar27 + lVar4 + 2) != 0);
  if (local_8588 - local_8590 < uVar23) {
    FUN_180021e60(local_85a0,uVar23,param_3,(undefined8 *)pauVar27,uVar23);
  }
  else {
    pppppppuVar18 = local_85a0;
    if (7 < local_8588) {
      pppppppuVar18 = local_85a0[0];
    }
    param_3 = (undefined8 *)(uVar23 * 2);
    lVar4 = local_8590 * 2;
    local_8590 = uVar23 + local_8590;
    FUN_180048680((undefined8 *)((longlong)pppppppuVar18 + lVar4),(undefined8 *)pauVar27,
                  (ulonglong)param_3);
    *(undefined2 *)((longlong)pppppppuVar18 + (lVar14 + uVar23) * 2) = 0;
  }
  lVar14 = local_8590;
  if (local_8588 - local_8590 < 10) {
    FUN_180021e60(local_85a0,10,param_3,(undefined8 *)L";registry=",10);
  }
  else {
    pppppppuVar18 = local_85a0;
    if (7 < local_8588) {
      pppppppuVar18 = local_85a0[0];
    }
    lVar4 = local_8590 * 2;
    local_8590 = local_8590 + 10;
    FUN_180048680((undefined8 *)((longlong)pppppppuVar18 + lVar4),(undefined8 *)L";registry=",0x14);
    *(undefined2 *)((longlong)pppppppuVar18 + lVar14 * 2 + 0x14) = 0;
  }
  pppppppuVar18 = local_85a0;
  if (7 < local_8588) {
    pppppppuVar18 = local_85a0[0];
  }
  IVar13 = (*local_85b0[0])(pppppppuVar18);
  if (IVar13 == 0) {
    *param_2 = 0;
    param_2[1] = 0;
    if (local_8588 < 8) {
      return param_2;
    }
    if (local_8588 * 2 + 2 < 0x1000) goto LAB_18000ba2a;
    lVar14 = (longlong)local_85a0[0] - (longlong)local_85a0[0][-1];
    local_85a0[0] = (undefined8 *******)local_85a0[0][-1];
  }
  else {
    psVar15 = local_8548 + uVar26 * 0x84;
    lVar14 = 2;
    puVar19 = param_4;
    do {
      uVar12 = *(undefined8 *)(psVar15 + 4);
      *puVar19 = *(undefined8 *)psVar15;
      puVar19[1] = uVar12;
      uVar12 = *(undefined8 *)(psVar15 + 0xc);
      puVar19[2] = *(undefined8 *)(psVar15 + 8);
      puVar19[3] = uVar12;
      uVar12 = *(undefined8 *)(psVar15 + 0x14);
      puVar19[4] = *(undefined8 *)(psVar15 + 0x10);
      puVar19[5] = uVar12;
      uVar12 = *(undefined8 *)(psVar15 + 0x1c);
      puVar19[6] = *(undefined8 *)(psVar15 + 0x18);
      puVar19[7] = uVar12;
      uVar12 = *(undefined8 *)(psVar15 + 0x24);
      puVar19[8] = *(undefined8 *)(psVar15 + 0x20);
      puVar19[9] = uVar12;
      uVar12 = *(undefined8 *)(psVar15 + 0x2c);
      puVar19[10] = *(undefined8 *)(psVar15 + 0x28);
      puVar19[0xb] = uVar12;
      uVar12 = *(undefined8 *)(psVar15 + 0x34);
      puVar19[0xc] = *(undefined8 *)(psVar15 + 0x30);
      puVar19[0xd] = uVar12;
      puVar22 = puVar19 + 0x10;
      uVar12 = *(undefined8 *)(psVar15 + 0x3c);
      puVar19[0xe] = *(undefined8 *)(psVar15 + 0x38);
      puVar19[0xf] = uVar12;
      psVar15 = psVar15 + 0x40;
      lVar14 = lVar14 + -1;
      puVar19 = puVar22;
    } while (lVar14 != 0);
    *puVar22 = *(undefined8 *)psVar15;
    plVar16 = (longlong *)FUN_180025980(0x18);
    *(undefined4 *)(plVar16 + 1) = 1;
    *(undefined4 *)((longlong)plVar16 + 0xc) = 1;
    *plVar16 = (longlong)std::_Ref_count_obj2<HWrapper>::vftable;
    plVar1 = plVar16 + 2;
    *plVar1 = IVar13;
    FUN_18001d110(local_8580,param_4 + 0x10);
    LOCK();
    *(int *)(plVar16 + 1) = (int)plVar16[1] + 1;
    UNLOCK();
    local_8560 = plVar1;
    local_8558 = plVar16;
    FUN_18001d950(pauVar9,(longlong *)local_85b0,(undefined1 (*) [32])local_8580);
    plVar10 = local_8558;
    if (local_8558 != (longlong *)0x0) {
      LOCK();
      plVar2 = local_8558 + 1;
      lVar14 = *plVar2;
      *(int *)plVar2 = (int)*plVar2 + -1;
      UNLOCK();
      if ((int)lVar14 == 1) {
        (**(code **)*local_8558)(local_8558);
        LOCK();
        piVar3 = (int *)((longlong)plVar10 + 0xc);
        iVar5 = *piVar3;
        *piVar3 = *piVar3 + -1;
        UNLOCK();
        if (iVar5 == 1) {
          (**(code **)(*plVar10 + 8))(plVar10);
        }
      }
    }
    FUN_18001d090(local_8580);
    *param_2 = (longlong)plVar1;
    param_2[1] = (longlong)plVar16;
    if (local_8588 < 8) {
      return param_2;
    }
    if (local_8588 * 2 + 2 < 0x1000) goto LAB_18000ba2a;
    lVar14 = (longlong)local_85a0[0] - (longlong)local_85a0[0][-1];
    local_85a0[0] = (undefined8 *******)local_85a0[0][-1];
  }
  if (0x1f < lVar14 - 8U) {
                    /* WARNING: Subroutine does not return */
    _invoke_watson((wchar_t *)0x0,(wchar_t *)0x0,(wchar_t *)0x0,0,0);
  }
LAB_18000ba2a:
  thunk_FUN_180039da0(local_85a0[0]);
  return param_2;
}


```

