find_package(PkgConfig)
pkg_check_modules(PC_tensorrt QUIET tensorrt)

find_path(tensorrt_INCLUDE_DIR
  NAMES NvInfer.h
  PATHS ${PC_tensorrt_INCLUDE_DIRS})

find_library(tensorrt_LIBRARIES
  NAMES nvinfer
  PATHS ${PC_tensorrt_LIBRARY_DIRS})

set(tensorrt_VERSION ${PC_tensorrt_VERSION})

mark_as_advanced(tensorrt_FOUND tensorrt_INCLUDE_DIR tensorrt_LIBRARIES tensorrt_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(tensorrt
  REQUIRED_VARS tensorrt_INCLUDE_DIR tensorrt_LIBRARIES
  VERSION_VAR tensorrt_VERSION)

if(tensorrt_FOUND AND NOT TARGET tensorrt::tensorrt)
  add_library(tensorrt::tensorrt SHARED IMPORTED)
  set_target_properties(tensorrt::tensorrt PROPERTIES
    IMPORTED_LOCATION "${tensorrt_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${tensorrt_INCLUDE_DIR}")
endif()
