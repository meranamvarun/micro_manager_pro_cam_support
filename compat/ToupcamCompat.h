// SPDX-License-Identifier: Apache-2.0
// Clean-room compatibility surface. This is not vendor source code.

#pragma once

#include <stdint.h>
#include <wchar.h>

#ifdef _WIN32
#define TOUPCAM_COMPAT_API __declspec(dllexport)
#else
#define TOUPCAM_COMPAT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ToupcamCompatHandle ToupcamCompatHandle;

typedef struct ToupcamCompatFrameInfo {
   uint32_t width;
   uint32_t height;
   uint32_t channels;
   uint32_t depth;
} ToupcamCompatFrameInfo;

// New, documented API used by mmgr_dal_Toupcam_compat. It intentionally does
// not claim binary compatibility with the vendor's undocumented structures.
TOUPCAM_COMPAT_API int ToupcamCompat_OpenFirst(
   int simulation, ToupcamCompatHandle** handle);
TOUPCAM_COMPAT_API void ToupcamCompat_Close(ToupcamCompatHandle* handle);
TOUPCAM_COMPAT_API int ToupcamCompat_Start(
   ToupcamCompatHandle* handle, double exposureMs, double gain);
TOUPCAM_COMPAT_API int ToupcamCompat_SetExposure(
   ToupcamCompatHandle* handle, double exposureMs, double* actualExposureMs);
TOUPCAM_COMPAT_API int ToupcamCompat_SetGain(
   ToupcamCompatHandle* handle, double gain);
TOUPCAM_COMPAT_API int ToupcamCompat_GetFrameInfo(
   ToupcamCompatHandle* handle, ToupcamCompatFrameInfo* info);
TOUPCAM_COMPAT_API int ToupcamCompat_SnapRgb24(
   ToupcamCompatHandle* handle, int timeoutMs, unsigned char* destination,
   uint32_t destinationBytes);
TOUPCAM_COMPAT_API const char* ToupcamCompat_LastError(void);

// Observed vendor entry-point names. Signatures are deliberately conservative
// and are not represented as a drop-in vendor ABI. They exist so callers can
// migrate incrementally to the documented ToupcamCompat_* functions above.
TOUPCAM_COMPAT_API const wchar_t* DllMmgrName(void);
TOUPCAM_COMPAT_API const wchar_t* DllCamName(void);
TOUPCAM_COMPAT_API const wchar_t* DllCompany(void);
TOUPCAM_COMPAT_API const wchar_t* DllVersion(void);
TOUPCAM_COMPAT_API void DllAppName(const wchar_t* applicationName);
TOUPCAM_COMPAT_API uint32_t DllEnumCamera(void* records);
TOUPCAM_COMPAT_API int DllGetDefAE(uint32_t* exposureUs, uint16_t* gain);
TOUPCAM_COMPAT_API int DllGigeEnable(void* reserved, uint64_t flags);
TOUPCAM_COMPAT_API ToupcamCompatHandle* DllOpen(const wchar_t* identifier);

#ifdef __cplusplus
}
#endif
