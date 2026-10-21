// SPDX-License-Identifier: Apache-2.0
// Primary researcher: Malavika S

#pragma once

#include "DeviceBase.h"
#include "ImgBuffer.h"
#include "loupecam.h"

#include <string>

class LoupeCamAdapter final : public CLegacyCameraBase<LoupeCamAdapter>
{
public:
   LoupeCamAdapter();
   ~LoupeCamAdapter() override;

   int Initialize() override;
   int Shutdown() override;
   void GetName(char* name) const override;

   int SnapImage() override;
   const unsigned char* GetImageBuffer() override;
   unsigned GetImageWidth() const override;
   unsigned GetImageHeight() const override;
   unsigned GetImageBytesPerPixel() const override;
   unsigned GetBitDepth() const override;
   long GetImageBufferSize() const override;
   unsigned GetNumberOfComponents() const override;

   int GetBinning() const override;
   int SetBinning(int binSize) override;
   void SetExposure(double exposureMs) override;
   double GetExposure() const override;
   int SetROI(unsigned x, unsigned y, unsigned xSize, unsigned ySize) override;
   int GetROI(unsigned& x, unsigned& y, unsigned& xSize, unsigned& ySize) override;
   int ClearROI() override;
   int IsExposureSequenceable(bool& isSequenceable) const override;

   int OnSimulation(MM::PropertyBase* property, MM::ActionType action);
   int OnGain(MM::PropertyBase* property, MM::ActionType action);
   int OnDemosaic(MM::PropertyBase* property, MM::ActionType action);

private:
   int ReportLoupeCamError(const char* operation, LoupecamStatus status);
   int OpenCamera();
   int StartStream();
   void ReleaseCamera();
   static void CopyRgbToBgra(
      const unsigned char* source,
      unsigned char* destination,
      size_t pixelCount);

   ImgBuffer image_;
   LoupecamCamera* camera_;
   LoupecamStream* stream_;
   bool initialized_;
   bool simulation_;
   double exposureMs_;
   double gain_;
   unsigned demosaic_;
   LoupecamCameraInfo cameraInfo_;
   MMThreadLock cameraLock_;
};

