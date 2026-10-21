// SPDX-License-Identifier: Apache-2.0
// Clean-room implementation backed by the LoupeCam C API.

#include "ToupcamCompat.h"

#include "loupecam.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>

struct ToupcamCompatHandle {
   LoupecamCamera* camera = nullptr;
   LoupecamStream* stream = nullptr;
   LoupecamGeometry geometry{};
   double exposureMs = 20.0;
   double gain = 1.0;
};

namespace {
thread_local std::string g_lastError;
std::wstring g_applicationName;
std::mutex g_applicationNameLock;

int Fail(const char* operation, LoupecamStatus status)
{
   g_lastError = std::string(operation) + " failed (status " +
      std::to_string(static_cast<int>(status)) + "): " + loupecam_last_error();
   return static_cast<int>(status) == 0 ? -1 : static_cast<int>(status);
}

void CloseInternal(ToupcamCompatHandle* handle)
{
   if (handle == nullptr)
      return;
   if (handle->stream != nullptr)
      loupecam_stream_free(handle->stream);
   if (handle->camera != nullptr)
      loupecam_camera_free(handle->camera);
   delete handle;
}
}

int ToupcamCompat_OpenFirst(int simulation, ToupcamCompatHandle** handle)
{
   if (handle == nullptr)
      return -1;
   *handle = nullptr;

   auto candidate = std::make_unique<ToupcamCompatHandle>();
   const LoupecamStatus status = simulation != 0
      ? loupecam_camera_open_simulated(&candidate->camera)
      : loupecam_camera_open(0, &candidate->camera);
   if (status != LOUPECAM_STATUS_OK)
      return Fail("open camera", status);

   *handle = candidate.release();
   return 0;
}

void ToupcamCompat_Close(ToupcamCompatHandle* handle)
{
   CloseInternal(handle);
}

int ToupcamCompat_Start(ToupcamCompatHandle* handle, double exposureMs, double gain)
{
   if (handle == nullptr || handle->camera == nullptr)
      return -1;
   if (handle->stream != nullptr)
      return 0;

   LoupecamStartOptions options = loupecam_start_options_default();
   options.size = 0;
   options.pixel_mode = LOUPECAM_PIXEL_MODE_RAW8;
   options.exposure_us = static_cast<uint32_t>(std::clamp(
      exposureMs * 1000.0, 1.0, 4294967295.0));
   options.gain = gain;
   const LoupecamStatus status =
      loupecam_camera_start(handle->camera, &options, &handle->stream);
   if (status != LOUPECAM_STATUS_OK)
      return Fail("start stream", status);

   status = loupecam_camera_geometry(handle->camera, &handle->geometry);
   if (status != LOUPECAM_STATUS_OK)
      return Fail("read camera geometry", status);

   handle->exposureMs = exposureMs;
   handle->gain = gain;
   return 0;
}

int ToupcamCompat_SetExposure(
   ToupcamCompatHandle* handle, double exposureMs, double* actualExposureMs)
{
   if (handle == nullptr || handle->camera == nullptr)
      return -1;
   double actualUs = 0.0;
   const uint32_t requestedUs = static_cast<uint32_t>(std::clamp(
      exposureMs * 1000.0, 1.0, 4294967295.0));
   const LoupecamStatus status =
      loupecam_camera_set_exposure(handle->camera, requestedUs, &actualUs);
   if (status != LOUPECAM_STATUS_OK)
      return Fail("set exposure", status);
   handle->exposureMs = actualUs / 1000.0;
   if (actualExposureMs != nullptr)
      *actualExposureMs = handle->exposureMs;
   return 0;
}

int ToupcamCompat_SetGain(ToupcamCompatHandle* handle, double gain)
{
   if (handle == nullptr || handle->camera == nullptr)
      return -1;
   const LoupecamStatus status = loupecam_camera_set_gain(handle->camera, gain);
   if (status != LOUPECAM_STATUS_OK)
      return Fail("set gain", status);
   handle->gain = gain;
   return 0;
}

int ToupcamCompat_GetFrameInfo(ToupcamCompatHandle* handle, ToupcamCompatFrameInfo* info)
{
   if (handle == nullptr || info == nullptr)
      return -1;
   info->width = handle->geometry.width;
   info->height = handle->geometry.height;
   info->channels = 3;
   info->depth = 8;
   return 0;
}

int ToupcamCompat_SnapRgb24(
   ToupcamCompatHandle* handle, int timeoutMs, unsigned char* destination,
   uint32_t destinationBytes)
{
   if (handle == nullptr || handle->stream == nullptr || destination == nullptr)
      return -1;
   LoupecamFrame* frame = nullptr;
   LoupecamStatus status = loupecam_stream_latest(handle->stream, timeoutMs, &frame);
   if (status != LOUPECAM_STATUS_OK)
      return Fail("read frame", status);

   LoupecamDevelopOptions options = loupecam_develop_options_default();
   options.demosaic = LOUPECAM_DEMOSAIC_BILINEAR;
   options.depth = 8;
   LoupecamImage* image = nullptr;
   status = loupecam_frame_develop(frame, &options, &image);
   loupecam_frame_free(frame);
   if (status != LOUPECAM_STATUS_OK)
      return Fail("develop frame", status);

   LoupecamImageInfo info{};
   const void* source = nullptr;
   size_t sourceBytes = 0;
   status = loupecam_image_info(image, &info);
   if (status == LOUPECAM_STATUS_OK)
      status = loupecam_image_data(image, &source, &sourceBytes);
   const size_t required = static_cast<size_t>(info.width) * info.height * 3;
   if (status != LOUPECAM_STATUS_OK || info.channels != 3 || info.depth != 8 ||
       sourceBytes < required || destinationBytes < required) {
      loupecam_image_free(image);
      return status == LOUPECAM_STATUS_OK ? -1 : Fail("access developed image", status);
   }
   std::memcpy(destination, source, required);
   loupecam_image_free(image);
   return 0;
}

const char* ToupcamCompat_LastError(void)
{
   return g_lastError.c_str();
}

const wchar_t* DllMmgrName(void) { return L"ToupcamCompat"; }
const wchar_t* DllCamName(void) { return DllMmgrName(); }
const wchar_t* DllCompany(void) { return L"Independent clean-room implementation"; }
const wchar_t* DllVersion(void) { return L"0.1.0"; }

void DllAppName(const wchar_t* applicationName)
{
   std::lock_guard<std::mutex> guard(g_applicationNameLock);
   g_applicationName = applicationName == nullptr ? L"" : applicationName;
}

uint32_t DllEnumCamera(void*)
{
   // The vendor's 264-byte record format is observed but its field layout is
   // not recovered. Use ToupcamCompat_OpenFirst instead of this opaque ABI.
   g_lastError = "DllEnumCamera record layout is intentionally unsupported; use ToupcamCompat_OpenFirst";
   return 0;
}

int DllGetDefAE(uint32_t* exposureUs, uint16_t* gain)
{
   if (exposureUs != nullptr)
      *exposureUs = 350000;
   if (gain != nullptr)
      *gain = 500;
   return 0;
}

int DllGigeEnable(void*, uint64_t)
{
   g_lastError = "GigE discovery is not implemented by the LoupeCam-backed compatibility SDK";
   return -1;
}

ToupcamCompatHandle* DllOpen(const wchar_t*)
{
   ToupcamCompatHandle* handle = nullptr;
   return ToupcamCompat_OpenFirst(0, &handle) == 0 ? handle : nullptr;
}
