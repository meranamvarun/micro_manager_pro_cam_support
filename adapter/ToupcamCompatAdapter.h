// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "DeviceBase.h"
#include "ImgBuffer.h"
#include "ToupcamCompat.h"

class ToupcamCompatAdapter final : public CLegacyCameraBase<ToupcamCompatAdapter>
{
public:
   ToupcamCompatAdapter();
   ~ToupcamCompatAdapter() override;

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

private:
   int ReportError(const char* operation, int status);
   void ReleaseCamera();

   ImgBuffer image_;
   ToupcamCompatHandle* camera_;
   bool initialized_;
   bool simulation_;
   double exposureMs_;
   double gain_;
   MMThreadLock cameraLock_;
};
