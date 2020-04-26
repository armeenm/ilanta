find_package(PkgConfig)
pkg_check_modules(PC_tensorflow_cc QUIET tensorflow_cc)

find_path(tensorflow_cc_INCLUDE_DIR
  NAMES tensorflow/core
  PATHS ${PC_tensorflow_cc_INCLUDE_DIRS})

find_library(tensorflow_cc_LIBRARIES
  NAMES tensorflow_cc
  PATHS ${PC_tensorflow_cc_LIBRARY_DIRS})

set(tensorflow_cc_VERSION ${PC_tensorflow_cc_VERSION})

mark_as_advanced(tensorflow_cc_INCLUDE_DIR tensorflow_cc_LIBRARIES tensorflow_cc_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(tensorflow_cc
  REQUIRED_VARS tensorflow_cc_INCLUDE_DIR tensorflow_cc_LIBRARIES
  VERSION_VAR tensorflow_cc_VERSION)

if(tensorflow_cc_FOUND AND NOT TARGET tensorflow_cc::tensorflow_cc)
  add_library(tensorflow_cc::tensorflow_cc SHARED IMPORTED)
  set_target_properties(tensorflow_cc::tensorflow_cc PROPERTIES
    IMPORTED_LOCATION "${tensorflow_cc_LIBRARIES}"
    INTERFACE_LINK_LIBRARIES tensorflow_framework
    INTERFACE_INCLUDE_DIRECTORIES "${tensorflow_cc_INCLUDE_DIR}")
endif()
