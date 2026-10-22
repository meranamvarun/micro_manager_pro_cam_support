// SPDX-License-Identifier: Apache-2.0
// Primary researcher: Malavika S

#include "LoupeCamAdapter.h"

#include "ModuleInterface.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <sstream>

namespace
{
const char* const kDeviceName = "LoupeCam";
const char* const kDeviceDescription =
   "Source-auditable USB microscope camera adapter backed by LoupeCam";
const char* const kSimulationProperty = "Simulation";
const char* const kGainProperty = "Gain";
const char* const kDemosaicProperty = "Demosaic";
const char* const kOn = "On";
const char* const kOff = "Off";
const char* const kBilinear = "Bilinear";
const char* const kSuperpixel = "Superpixel";
const char* const kMhc = "Malvar-He-Cutler";

const int kLoupeCamError = 20001;
const int kInvalidImage = 20002;

std::string FormatVersion(const LoupecamCameraInfo& info)
{
   std::ostringstream stream;
   stream << static_cast<unsigned>(info.fpga_version_major) << '.'
          << static_cast<unsigned>(info.fpga_version_minor);
   return stream.str();
}
}

MODULE_API void InitializeModuleData()
{
   RegisterDevice(kDeviceName, MM::CameraDevice, kDeviceDescription);
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
   if (deviceName != nullptr && std::strcmp(deviceName, kDeviceName) == 0)
      return new LoupeCamAdapter();
   return nullptr;
}

MODULE_API void DeleteDevice(MM::Device* device)
{
   delete device;
}

LoupeCamAdapter::LoupeCamAdapter() :
   camera_(nullptr),
   stream_(nullptr),
   initialized_(false),
   simulation_(false),
   exposureMs_(20.0),
   gain_(1.0),
   demosaic_(LOUPECAM_DEMOSAIC_BILINEAR),
   cameraInfo_{}
{
   InitializeDefaultErrorMessages();
   SetErrorText(kLoupeCamError, "LoupeCam operation failed");
   SetErrorText(kInvalidImage, "LoupeCam returned an unsupported image layout");

   auto* simulationAction =
      new CPropertyAction(this, &LoupeCamAdapter::OnSimulation);
   CreateProperty(
      kSimulationProperty, kOff, MM::String, false, simulationAction, true);
   AddAllowedValue(kSimulationProperty, kOff);
   AddAllowedValue(kSimulationProperty, kOn);
}

LoupeCamAdapter::~LoupeCamAdapter()
{
   Shutdown();
}

int LoupeCamAdapter::Initialize()
{
   MMThreadGuard guard(cameraLock_);
   if (initialized_)
      return DEVICE_OK;

   int result = OpenCamera();
   if (result != DEVICE_OK)
      return result;

   result = StartStream();
   if (result != DEVICE_OK)
   {
      ReleaseCamera();
      return result;
   }

   CreateProperty(MM::g_Keyword_Name, kDeviceName, MM::String, true);
   CreateProperty(MM::g_Keyword_Description, kDeviceDescription, MM::String, true);
   CreateProperty("Driver Version", loupecam_version(), MM::String, true);
   CreateProperty("Camera Model", cameraInfo_.model, MM::String, true);
   CreateProperty("Serial Number", cameraInfo_.serial, MM::String, true);
   CreateProperty("Firmware Version", cameraInfo_.firmware_version, MM::String, true);
   CreateProperty("FPGA Version", FormatVersion(cameraInfo_).c_str(), MM::String, true);
   CreateProperty("USB Vendor ID", std::to_string(cameraInfo_.vendor_id).c_str(), MM::Integer, true);
   CreateProperty("USB Product ID", std::to_string(cameraInfo_.product_id).c_str(), MM::Integer, true);

   CreateProperty(
      MM::g_Keyword_Exposure,
      CDeviceUtils::ConvertToString(exposureMs_),
      MM::Float,
      false);
   SetPropertyLimits(MM::g_Keyword_Exposure, 0.001, 60000.0);

   auto* gainAction = new CPropertyAction(this, &LoupeCamAdapter::OnGain);
   CreateProperty(kGainProperty, "1.0", MM::Float, false, gainAction);
   SetPropertyLimits(kGainProperty, 1.0, loupecam_max_gain());

   auto* demosaicAction = new CPropertyAction(this, &LoupeCamAdapter::OnDemosaic);
   CreateProperty(kDemosaicProperty, kBilinear, MM::String, false, demosaicAction);
   AddAllowedValue(kDemosaicProperty, kBilinear);
   AddAllowedValue(kDemosaicProperty, kSuperpixel);
   AddAllowedValue(kDemosaicProperty, kMhc);

   CreateProperty(MM::g_Keyword_PixelType, "32bitRGB", MM::String, true);
   CreateProperty(MM::g_Keyword_Binning, "1", MM::Integer, true);

   initialized_ = true;
   return DEVICE_OK;
}

int LoupeCamAdapter::Shutdown()
{
   StopSequenceAcquisition();
   MMThreadGuard guard(cameraLock_);
   ReleaseCamera();
   initialized_ = false;
   return DEVICE_OK;
}

void LoupeCamAdapter::GetName(char* name) const
{
   CDeviceUtils::CopyLimitedString(name, kDeviceName);
}

int LoupeCamAdapter::OpenCamera()
{
   LoupecamStatus status = simulation_
      ? loupecam_camera_open_simulated(&camera_)
      : loupecam_camera_open(0, &camera_);
   if (status != LOUPECAM_STATUS_OK)
      return ReportLoupeCamError("open camera", status);

   status = loupecam_camera_info(camera_, &cameraInfo_);
   if (status != LOUPECAM_STATUS_OK) {
      loupecam_camera_free(camera_);
      camera_ = nullptr;
      return ReportLoupeCamError("read camera information", status);
   }

   return DEVICE_OK;
}

int LoupeCamAdapter::StartStream()
{
   LoupecamStartOptions options = loupecam_start_options_default();
   // Keep CI and local simulation fast while real hardware defaults to its
   // highest-detail mode. A user-facing resolution selector will replace this
   // temporary policy when the first physical model is validated.
   options.size = simulation_ ? 2 : 0;
   options.pixel_mode = LOUPECAM_PIXEL_MODE_RAW8;
   options.exposure_us = static_cast<uint32_t>(
      std::clamp(exposureMs_ * 1000.0, 1.0, 4294967295.0));
   options.gain = gain_;

   LoupecamStatus status = loupecam_camera_start(camera_, &options, &stream_);
   if (status != LOUPECAM_STATUS_OK)
      return ReportLoupeCamError("start stream", status);

   LoupecamGeometry geometry{};
   status = loupecam_camera_geometry(camera_, &geometry);
   if (status != LOUPECAM_STATUS_OK)
      return ReportLoupeCamError("read stream geometry", status);

   image_.Resize(geometry.width, geometry.height, 4);
   return DEVICE_OK;
}

void LoupeCamAdapter::ReleaseCamera()
{
   if (stream_ != nullptr)
   {
      loupecam_stream_free(stream_);
      stream_ = nullptr;
   }
   if (camera_ != nullptr)
   {
      loupecam_camera_free(camera_);
      camera_ = nullptr;
   }
}

int LoupeCamAdapter::SnapImage()
{
   MMThreadGuard guard(cameraLock_);
   if (stream_ == nullptr)
      return DEVICE_NOT_CONNECTED;

   LoupecamFrame* frame = nullptr;
   const int timeoutMs = static_cast<int>(
      std::clamp(exposureMs_ + 2000.0, 2000.0, 60000.0));
   LoupecamStatus status = loupecam_stream_latest(stream_, timeoutMs, &frame);
   if (status != LOUPECAM_STATUS_OK)
      return ReportLoupeCamError("read frame", status);

   LoupecamDevelopOptions options = loupecam_develop_options_default();
   options.demosaic = demosaic_;
   options.depth = 8;

   LoupecamImage* developed = nullptr;
   status = loupecam_frame_develop(frame, &options, &developed);
   loupecam_frame_free(frame);
   if (status != LOUPECAM_STATUS_OK)
      return ReportLoupeCamError("develop frame", status);

   LoupecamImageInfo info{};
   const void* source = nullptr;
   size_t sourceLength = 0;
   status = loupecam_image_info(developed, &info);
   if (status == LOUPECAM_STATUS_OK)
      status = loupecam_image_data(developed, &source, &sourceLength);

   if (status != LOUPECAM_STATUS_OK)
   {
      loupecam_image_free(developed);
      return ReportLoupeCamError("access developed image", status);
   }

   const size_t pixelCount = static_cast<size_t>(info.width) * info.height;
   if (info.channels != 3 || info.depth != 8 || sourceLength < pixelCount * 3)
   {
      loupecam_image_free(developed);
      return kInvalidImage;
   }

   if (image_.Width() != info.width || image_.Height() != info.height)
      image_.Resize(info.width, info.height, 4);

   CopyRgbToBgra(
      static_cast<const unsigned char*>(source), image_.GetPixelsRW(), pixelCount);
   loupecam_image_free(developed);
   return DEVICE_OK;
}

void LoupeCamAdapter::CopyRgbToBgra(
   const unsigned char* source,
   unsigned char* destination,
   size_t pixelCount)
{
   for (size_t index = 0; index < pixelCount; ++index)
   {
      destination[index * 4] = source[index * 3 + 2];
      destination[index * 4 + 1] = source[index * 3 + 1];
      destination[index * 4 + 2] = source[index * 3];
      destination[index * 4 + 3] = 0;
   }
}

const unsigned char* LoupeCamAdapter::GetImageBuffer()
{
   return image_.GetPixels();
}

unsigned LoupeCamAdapter::GetImageWidth() const
{
   return image_.Width();
}

unsigned LoupeCamAdapter::GetImageHeight() const
{
   return image_.Height();
}

unsigned LoupeCamAdapter::GetImageBytesPerPixel() const
{
   return image_.Depth();
}

unsigned LoupeCamAdapter::GetBitDepth() const
{
   return 8;
}

long LoupeCamAdapter::GetImageBufferSize() const
{
   return static_cast<long>(image_.Width() * image_.Height() * image_.Depth());
}

unsigned LoupeCamAdapter::GetNumberOfComponents() const
{
   return 4;
}

int LoupeCamAdapter::GetBinning() const
{
   return 1;
}

int LoupeCamAdapter::SetBinning(int binSize)
{
   return binSize == 1 ? DEVICE_OK : DEVICE_UNSUPPORTED_COMMAND;
}

void LoupeCamAdapter::SetExposure(double exposureMs)
{
   exposureMs_ = std::max(0.001, exposureMs);
   MMThreadGuard guard(cameraLock_);
   if (camera_ != nullptr)
   {
      double actualUs = 0.0;
      const uint32_t requestedUs = static_cast<uint32_t>(
         std::clamp(exposureMs_ * 1000.0, 1.0, 4294967295.0));
      if (loupecam_camera_set_exposure(camera_, requestedUs, &actualUs) == LOUPECAM_STATUS_OK)
         exposureMs_ = actualUs / 1000.0;
   }
}

double LoupeCamAdapter::GetExposure() const
{
   return exposureMs_;
}

int LoupeCamAdapter::SetROI(unsigned, unsigned, unsigned, unsigned)
{
   return DEVICE_UNSUPPORTED_COMMAND;
}

int LoupeCamAdapter::GetROI(
   unsigned& x, unsigned& y, unsigned& xSize, unsigned& ySize)
{
   x = 0;
   y = 0;
   xSize = image_.Width();
   ySize = image_.Height();
   return DEVICE_OK;
}

int LoupeCamAdapter::ClearROI()
{
   return DEVICE_OK;
}

int LoupeCamAdapter::IsExposureSequenceable(bool& isSequenceable) const
{
   isSequenceable = false;
   return DEVICE_OK;
}

int LoupeCamAdapter::OnSimulation(
   MM::PropertyBase* property, MM::ActionType action)
{
   if (action == MM::BeforeGet)
      property->Set(simulation_ ? kOn : kOff);
   else if (action == MM::AfterSet)
   {
      std::string value;
      property->Get(value);
      simulation_ = value == kOn;
   }
   return DEVICE_OK;
}

int LoupeCamAdapter::OnGain(MM::PropertyBase* property, MM::ActionType action)
{
   if (action == MM::BeforeGet)
      property->Set(gain_);
   else if (action == MM::AfterSet)
   {
      property->Get(gain_);
      MMThreadGuard guard(cameraLock_);
      if (camera_ != nullptr)
      {
         const LoupecamStatus status = loupecam_camera_set_gain(camera_, gain_);
         if (status != LOUPECAM_STATUS_OK)
            return ReportLoupeCamError("set gain", status);
      }
   }
   return DEVICE_OK;
}

int LoupeCamAdapter::OnDemosaic(
   MM::PropertyBase* property, MM::ActionType action)
{
   if (action == MM::BeforeGet)
   {
      const char* value = kBilinear;
      if (demosaic_ == LOUPECAM_DEMOSAIC_SUPERPIXEL)
         value = kSuperpixel;
      else if (demosaic_ == LOUPECAM_DEMOSAIC_MHC)
         value = kMhc;
      property->Set(value);
   }
   else if (action == MM::AfterSet)
   {
      std::string value;
      property->Get(value);
      if (value == kSuperpixel)
         demosaic_ = LOUPECAM_DEMOSAIC_SUPERPIXEL;
      else if (value == kMhc)
         demosaic_ = LOUPECAM_DEMOSAIC_MHC;
      else
         demosaic_ = LOUPECAM_DEMOSAIC_BILINEAR;
   }
   return DEVICE_OK;
}

int LoupeCamAdapter::ReportLoupeCamError(
   const char* operation, LoupecamStatus status)
{
   std::ostringstream message;
   message << operation << " failed (status " << static_cast<int>(status) << "): "
           << loupecam_last_error();
   SetErrorText(kLoupeCamError, message.str().c_str());
   LogMessage(message.str().c_str(), false);
   return kLoupeCamError;
}
