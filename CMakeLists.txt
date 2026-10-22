cmake_minimum_required(VERSION 3.24)
project(MicroManagerLoupeCam LANGUAGES CXX)

if(NOT DEFINED MMCOREANDDEVICES_ROOT)
  message(FATAL_ERROR "Set MMCOREANDDEVICES_ROOT to a Device API 75 mmCoreAndDevices checkout")
endif()

if(NOT DEFINED LOUPECAM_ROOT)
  message(FATAL_ERROR "Set LOUPECAM_ROOT to the pinned LoupeCam checkout")
endif()

if(NOT DEFINED LOUPECAM_LIBRARY_DIR)
  message(FATAL_ERROR "Set LOUPECAM_LIBRARY_DIR to the directory containing loupecam_c.lib")
endif()

set(MMDEVICE_DIR "${MMCOREANDDEVICES_ROOT}/MMDevice")

add_library(mmgr_dal_LoupeCam SHARED
  adapter/LoupeCamAdapter.cpp
  ${MMDEVICE_DIR}/Debayer.cpp
  ${MMDEVICE_DIR}/DeviceUtils.cpp
  ${MMDEVICE_DIR}/ImgBuffer.cpp
  ${MMDEVICE_DIR}/MMDevice.cpp
  ${MMDEVICE_DIR}/ModuleInterface.cpp
  ${MMDEVICE_DIR}/Property.cpp
)

target_include_directories(mmgr_dal_LoupeCam PRIVATE
  adapter
  "${MMDEVICE_DIR}"
  "${LOUPECAM_ROOT}/bindings/c/include"
)

target_compile_features(mmgr_dal_LoupeCam PRIVATE cxx_std_17)
target_compile_definitions(mmgr_dal_LoupeCam PRIVATE
  NOMINMAX
  _CRT_SECURE_NO_WARNINGS
  _SCL_SECURE_NO_WARNINGS
)

if(MSVC)
  target_compile_options(mmgr_dal_LoupeCam PRIVATE /W4 /permissive- /utf-8)
  target_link_options(mmgr_dal_LoupeCam PRIVATE /DYNAMICBASE /NXCOMPAT)
endif()

if(WIN32)
  # Rust emits both a static archive (loupecam_c.lib) and an import library
  # (loupecam_c.dll.lib). Link the latter so the adapter uses the separately
  # auditable loupecam_c.dll shipped in the artifact bundle.
  set(LOUPECAM_C_LIBRARY "${LOUPECAM_LIBRARY_DIR}/loupecam_c.dll.lib")
  if(NOT EXISTS "${LOUPECAM_C_LIBRARY}")
    message(FATAL_ERROR "Missing LoupeCam DLL import library: ${LOUPECAM_C_LIBRARY}")
  endif()
else()
  find_library(LOUPECAM_C_LIBRARY
    NAMES loupecam_c
    PATHS "${LOUPECAM_LIBRARY_DIR}"
    NO_DEFAULT_PATH
    REQUIRED
  )
endif()
target_link_libraries(mmgr_dal_LoupeCam PRIVATE "${LOUPECAM_C_LIBRARY}")

if(WIN32)
  add_custom_command(TARGET mmgr_dal_LoupeCam POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
      "${LOUPECAM_LIBRARY_DIR}/loupecam_c.dll"
      "$<TARGET_FILE_DIR:mmgr_dal_LoupeCam>/loupecam_c.dll"
    VERBATIM
  )
endif()

set_target_properties(mmgr_dal_LoupeCam PROPERTIES
  PREFIX ""
  OUTPUT_NAME "mmgr_dal_LoupeCam"
)

# The compatibility pair deliberately has distinct names by default. Enable
# TOUPCAM_COMPAT_VENDOR_FILENAMES only in an isolated test directory after
# verifying the replacement; it would otherwise shadow the vendor DLL pair.
option(TOUPCAM_COMPAT_VENDOR_FILENAMES
  "Use vendor-compatible output names for the clean-room compatibility pair" OFF)

add_library(toupcam_compat SHARED
  compat/ToupcamCompat.cpp
)
target_include_directories(toupcam_compat PRIVATE
  compat
  "${LOUPECAM_ROOT}/bindings/c/include"
)
target_compile_features(toupcam_compat PRIVATE cxx_std_17)
target_compile_definitions(toupcam_compat PRIVATE
  TOUPCAM_COMPAT_BUILD
  NOMINMAX
  WIN32_LEAN_AND_MEAN
  _CRT_SECURE_NO_WARNINGS
)
if(MSVC)
  target_compile_options(toupcam_compat PRIVATE /W4 /permissive- /utf-8)
  target_link_options(toupcam_compat PRIVATE /DYNAMICBASE /NXCOMPAT)
endif()
target_link_libraries(toupcam_compat PRIVATE "${LOUPECAM_C_LIBRARY}")
if(WIN32)
  add_custom_command(TARGET toupcam_compat POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
      "${LOUPECAM_LIBRARY_DIR}/loupecam_c.dll"
      "$<TARGET_FILE_DIR:toupcam_compat>/loupecam_c.dll"
    VERBATIM
  )
endif()

add_library(mmgr_dal_Toupcam_compat SHARED
  adapter/ToupcamCompatAdapter.cpp
  ${MMDEVICE_DIR}/Debayer.cpp
  ${MMDEVICE_DIR}/DeviceUtils.cpp
  ${MMDEVICE_DIR}/ImgBuffer.cpp
  ${MMDEVICE_DIR}/MMDevice.cpp
  ${MMDEVICE_DIR}/ModuleInterface.cpp
  ${MMDEVICE_DIR}/Property.cpp
)
target_include_directories(mmgr_dal_Toupcam_compat PRIVATE
  adapter
  compat
  "${MMDEVICE_DIR}"
)
target_compile_features(mmgr_dal_Toupcam_compat PRIVATE cxx_std_17)
target_compile_definitions(mmgr_dal_Toupcam_compat PRIVATE
  NOMINMAX
  _CRT_SECURE_NO_WARNINGS
  _SCL_SECURE_NO_WARNINGS
)
if(MSVC)
  target_compile_options(mmgr_dal_Toupcam_compat PRIVATE /W4 /permissive- /utf-8)
  target_link_options(mmgr_dal_Toupcam_compat PRIVATE /DYNAMICBASE /NXCOMPAT)
endif()
target_link_libraries(mmgr_dal_Toupcam_compat PRIVATE toupcam_compat)
if(WIN32)
  add_custom_command(TARGET mmgr_dal_Toupcam_compat POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
      "$<TARGET_FILE:toupcam_compat>"
      "$<TARGET_FILE_DIR:mmgr_dal_Toupcam_compat>/"
    VERBATIM
  )
endif()

if(TOUPCAM_COMPAT_VENDOR_FILENAMES)
  set_target_properties(toupcam_compat PROPERTIES PREFIX "" OUTPUT_NAME "toupcam")
  set_target_properties(mmgr_dal_Toupcam_compat PROPERTIES PREFIX "" OUTPUT_NAME "mmgr_dal_Toupcam")
else()
  set_target_properties(toupcam_compat PROPERTIES PREFIX "" OUTPUT_NAME "toupcam_compat")
  set_target_properties(mmgr_dal_Toupcam_compat PROPERTIES PREFIX "" OUTPUT_NAME "mmgr_dal_Toupcam_compat")
endif()

enable_testing()
add_executable(loupecam_adapter_smoke tests/adapter_smoke.cpp)
target_include_directories(loupecam_adapter_smoke PRIVATE "${MMDEVICE_DIR}")
target_compile_features(loupecam_adapter_smoke PRIVATE cxx_std_17)
if(MSVC)
  target_compile_options(loupecam_adapter_smoke PRIVATE /W4 /permissive- /utf-8)
endif()
target_link_libraries(loupecam_adapter_smoke PRIVATE mmgr_dal_LoupeCam)
add_test(NAME loupecam_adapter_simulation COMMAND loupecam_adapter_smoke)

add_executable(toupcam_compat_adapter_smoke tests/adapter_smoke.cpp)
target_include_directories(toupcam_compat_adapter_smoke PRIVATE "${MMDEVICE_DIR}")
target_compile_features(toupcam_compat_adapter_smoke PRIVATE cxx_std_17)
target_compile_definitions(toupcam_compat_adapter_smoke PRIVATE
  SMOKE_DEVICE_NAME="ToupcamCompat"
)
if(MSVC)
  target_compile_options(toupcam_compat_adapter_smoke PRIVATE /W4 /permissive- /utf-8)
endif()
target_link_libraries(toupcam_compat_adapter_smoke PRIVATE mmgr_dal_Toupcam_compat)
add_test(NAME toupcam_compat_adapter_simulation COMMAND toupcam_compat_adapter_smoke)
