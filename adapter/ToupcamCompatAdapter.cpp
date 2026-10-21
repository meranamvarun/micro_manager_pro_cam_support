// SPDX-License-Identifier: Apache-2.0

#include "ToupcamCompatAdapter.h"

#include "ModuleInterface.h"

#include <algorithm>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace {
const char* const kDeviceName = "ToupcamCompat";
const char* const kDescription =
   "Clean-room camera adapter backed by ToupcamCompat and LoupeCam";
const char* const kSimulation = "Simulation";
const char* const kGain = "Gain";
const char* const kOn = "On";
const char* const kOff = "Off";
const int kCompatError = 21001;

void RgbToBgra(const unsigned char* source, unsigned char* destination, size_t pixels)
{
   for (size_t i = 0; i < pixels; ++i) {
      destination[i * 4] = source[i * 3 + 2];
      destination[i * 4 + 1] = source[i * 3 + 1];
      destination[i * 4 + 2] = source[i * 3];
      destination[i * 4 + 3] = 0;
   }
}
}

MODULE_API void InitializeModuleData()
{
   RegisterDevice(kDeviceName, MM::CameraDevice, kDescription);
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   return deviceName != nullptr && std::strcmp(deviceName, kDeviceName) == 0
      ? new ToupcamCompatAdapter() : nullptr;
}

MODULE_API void DeleteDevice(MM::Device* device)
{
   delete device;
}

ToupcamCompatAdapter::ToupcamCompatAdapter() :
   camera_(nullptr), initialized_(false), simulation_(false), exposureMs_(20.0), gain_(1.0)
{
   InitializeDefaultErrorMessages();
   SetErrorText(kCompatError, "ToupcamCompat operation failed");
   auto* simulationAction = new CPropertyAction(this, &ToupcamCompatAdapter::OnSimulation);
   CreateProperty(kSimulation, kOff, MM::String, false, simulationAction, true);
   AddAllowedValue(kSimulation, kOff);
   AddAllowedValue(kSimulation, kOn);
}

ToupcamCompatAdapter::~ToupcamCompatAdapter() { Shutdown(); }

int ToupcamCompatAdapter::Initialize()
{
   MMThreadGuard guard(cameraLock_);
   if (initialized_)
      return DEVICE_OK;
   int status = ToupcamCompat_OpenFirst(simulation_ ? 1 : 0, &camera_);
   if (status != 0)
      return ReportError("open camera", status);
   status = ToupcamCompat_Start(camera_, exposureMs_, gain_);
   if (status != 0) {
      ReleaseCamera();
      return ReportError("start stream", status);
   }
   ToupcamCompatFrameInfo frame{};
   status = ToupcamCompat_GetFrameInfo(camera_, &frame);
   if (status != 0 || frame.width == 0 || frame.height == 0) {
      ReleaseCamera();
      return ReportError("read frame geometry", status);
   }
   image_.Resize(frame.width, frame.height, 4);

   CreateProperty(MM::g_Keyword_Name, kDeviceName, MM::String, true);
   CreateProperty(MM::g_Keyword_Description, kDescription, MM::String, true);
   CreateProperty("Driver Version", "0.1.0", MM::String, true);
   CreateProperty(MM::g_Keyword_Exposure, CDeviceUtils::ConvertToString(exposureMs_), MM::Float, false);
   SetPropertyLimits(MM::g_Keyword_Exposure, 0.001, 60000.0);
   auto* gainAction = new CPropertyAction(this, &ToupcamCompatAdapter::OnGain);
   CreateProperty(kGain, CDeviceUtils::ConvertToString(gain_), MM::Float, false, gainAction);
   CreateProperty(MM::g_Keyword_PixelType, "32bitRGB", MM::String, true);
   CreateProperty(MM::g_Keyword_Binning, "1", MM::Integer, true);
   initialized_ = true;
   return DEVICE_OK;
}

int ToupcamCompatAdapter::Shutdown()
{
   StopSequenceAcquisition();
   MMThreadGuard guard(cameraLock_);
   ReleaseCamera();
   initialized_ = false;
   return DEVICE_OK;
}

void ToupcamCompatAdapter::GetName(char* name) const { CDeviceUtils::CopyLimitedString(name, kDeviceName); }

int ToupcamCompatAdapter::SnapImage()
{
   MMThreadGuard guard(cameraLock_);
   if (camera_ == nullptr)
      return DEVICE_NOT_CONNECTED;
   const size_t rgbBytes = static_cast<size_t>(image_.Width()) * image_.Height() * 3;
   std::vector<unsigned char> rgb(rgbBytes);
   const int timeoutMs = static_cast<int>(std::clamp(exposureMs_ + 2000.0, 2000.0, 60000.0));
   const int status = ToupcamCompat_SnapRgb24(camera_, timeoutMs, rgb.data(), static_cast<uint32_t>(rgb.size()));
   if (status != 0)
      return ReportError("read frame", status);
   RgbToBgra(rgb.data(), image_.GetPixelsRW(), static_cast<size_t>(image_.Width()) * image_.Height());
   return DEVICE_OK;
}

const unsigned char* ToupcamCompatAdapter::GetImageBuffer() { return image_.GetPixels(); }
unsigned ToupcamCompatAdapter::GetImageWidth() const { return image_.Width(); }
unsigned ToupcamCompatAdapter::GetImageHeight() const { return image_.Height(); }
unsigned ToupcamCompatAdapter::GetImageBytesPerPixel() const { return image_.Depth(); }
unsigned ToupcamCompatAdapter::GetBitDepth() const { return 8; }
long ToupcamCompatAdapter::GetImageBufferSize() const { return static_cast<long>(image_.Width() * image_.Height() * image_.Depth()); }
unsigned ToupcamCompatAdapter::GetNumberOfComponents() const { return 4; }
int ToupcamCompatAdapter::GetBinning() const { return 1; }
int ToupcamCompatAdapter::SetBinning(int value) { return value == 1 ? DEVICE_OK : DEVICE_UNSUPPORTED_COMMAND; }

void ToupcamCompatAdapter::SetExposure(double exposureMs)
{
   MMThreadGuard guard(cameraLock_);
   exposureMs_ = std::max(0.001, exposureMs);
   if (camera_ != nullptr) {
      double actual = exposureMs_;
      const int status = ToupcamCompat_SetExposure(camera_, exposureMs_, &actual);
      if (status == 0)
         exposureMs_ = actual;
      else
         ReportError("set exposure", status);
   }
}

double ToupcamCompatAdapter::GetExposure() const { return exposureMs_; }
int ToupcamCompatAdapter::SetROI(unsigned, unsigned, unsigned, unsigned) { return DEVICE_UNSUPPORTED_COMMAND; }
int ToupcamCompatAdapter::GetROI(unsigned& x, unsigned& y, unsigned& xSize, unsigned& ySize)
{
   x = 0; y = 0; xSize = image_.Width(); ySize = image_.Height(); return DEVICE_OK;
}
int ToupcamCompatAdapter::ClearROI() { return DEVICE_OK; }
int ToupcamCompatAdapter::IsExposureSequenceable(bool& value) const { value = false; return DEVICE_OK; }

int ToupcamCompatAdapter::OnSimulation(MM::PropertyBase* property, MM::ActionType action)
{
   if (action == MM::BeforeGet)
      property->Set(simulation_ ? kOn : kOff);
   else if (action == MM::AfterSet) {
      std::string value;
      property->Get(value);
      simulation_ = value == kOn;
   }
   return DEVICE_OK;
}

int ToupcamCompatAdapter::OnGain(MM::PropertyBase* property, MM::ActionType action)
{
   if (action == MM::BeforeGet)
      property->Set(gain_);
   else if (action == MM::AfterSet) {
      property->Get(gain_);
      MMThreadGuard guard(cameraLock_);
      if (camera_ != nullptr) {
         const int status = ToupcamCompat_SetGain(camera_, gain_);
         if (status != 0)
            return ReportError("set gain", status);
      }
   }
   return DEVICE_OK;
}

int ToupcamCompatAdapter::ReportError(const char* operation, int status)
{
   std::ostringstream message;
   message << operation << " failed (status " << status << "): " << ToupcamCompat_LastError();
   SetErrorText(kCompatError, message.str().c_str());
   LogMessage(message.str().c_str(), false);
   return kCompatError;
}

void ToupcamCompatAdapter::ReleaseCamera()
{
   if (camera_ != nullptr) {
      ToupcamCompat_Close(camera_);
      camera_ = nullptr;
   }
}
