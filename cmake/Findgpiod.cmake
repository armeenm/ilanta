find_package(PkgConfig)
pkg_check_modules(PC_gpiod QUIET gpiod)

find_path(gpiod_INCLUDE_DIR
  NAMES gpiod.h
  PATHS ${PC_gpiod_INCLUDE_DIRS})

find_library(gpiod_LIBRARIES
  NAMES gpiod
  PATHS ${PC_gpiod_LIBRARY_DIRS})

set(gpiod_VERSION ${PC_gpiod_VERSION})

mark_as_advanced(gpiod_INCLUDE_DIR gpiod_LIBRARIES gpiod_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(gpiod
  REQUIRED_VARS gpiod_INCLUDE_DIR gpiod_LIBRARIES
  VERSION_VAR gpiod_VERSION)

if(gpiod_FOUND AND NOT gpiod::gpiod)
  add_library(gpiod::gpiod SHARED IMPORTED)
  set_target_properties(gpiod::gpiod PROPERTIES
    IMPORTED_LOCATION "${gpiod_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${gpiod_INCLUDE_DIR}")
endif()
