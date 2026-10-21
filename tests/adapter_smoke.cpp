// SPDX-License-Identifier: Apache-2.0
// Primary researcher: Malavika S

#include "MMDevice.h"
#include "ModuleInterface.h"

#include <iostream>

int main()
{
   InitializeModuleData();
   if (GetDeviceInterfaceVersion() != 75)
   {
      std::cerr << "Expected Device API 75, got "
                << GetDeviceInterfaceVersion() << '\n';
      return 1;
   }

   MM::Device* device = CreateDevice("LoupeCam");
   if (device == nullptr)
   {
      std::cerr << "CreateDevice returned null\n";
      return 2;
   }

   int result = device->SetProperty("Simulation", "On");
   if (result == DEVICE_OK)
      result = device->Initialize();

   MM::Camera* camera = dynamic_cast<MM::Camera*>(device);
   if (result == DEVICE_OK && camera == nullptr)
      result = DEVICE_ERR;
   if (result == DEVICE_OK)
      result = camera->SnapImage();

   if (result == DEVICE_OK)
   {
      if (camera->GetImageWidth() == 0 ||
          camera->GetImageHeight() == 0 ||
          camera->GetImageBytesPerPixel() != 4 ||
          camera->GetNumberOfComponents() != 4 ||
          camera->GetImageBuffer() == nullptr)
      {
         std::cerr << "Invalid simulated image layout\n";
         result = DEVICE_ERR;
      }
   }

   device->Shutdown();
   DeleteDevice(device);

   if (result != DEVICE_OK)
   {
      std::cerr << "Adapter smoke test failed with code " << result << '\n';
      return 3;
   }

   std::cout << "LoupeCam adapter simulation smoke test passed\n";
   return 0;
}

