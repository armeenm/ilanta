find_package(PkgConfig)
pkg_check_modules(PC_gpiod QUIET gpiod)

# C #
find_path(gpiod_INCLUDE_DIR
  NAMES gpiod.h
  PATHS ${PC_gpiod_INCLUDE_DIRS})

find_library(gpiod_LIBRARIES
  NAMES gpiod
  PATHS ${PC_gpiod_LIBRARY_DIRS})

mark_as_advanced(gpiod_INCLUDE_DIR gpiod_LIBRARIES)

set(gpiod_VERSION ${PC_gpiod_VERSION})

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

# CXX #
find_package(PkgConfig)
pkg_check_modules(PC_gpiodcxx QUIET gpiodcxx)

find_path(gpiodcxx_INCLUDE_DIR
  NAMES gpiod.hpp
  PATHS ${PC_gpiodcxx_INCLUDE_DIRS})

find_library(gpiodcxx_LIBRARIES
  NAMES gpiodcxx
  PATHS ${PC_gpiodcxx_LIBRARY_DIRS})

mark_as_advanced(gpiodcxx_INCLUDE_DIR gpiodcxx_LIBRARIES)

set(gpiodcxx_VERSION ${PC_gpiodcxx_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(gpiodcxx
  REQUIRED_VARS gpiodcxx_INCLUDE_DIR
  VERSION_VAR gpiodcxx_VERSION)

if(gpiodcxx_FOUND AND NOT gpiod::gpiodcxx)
  add_library(gpiod::gpiodcxx SHARED IMPORTED)
  set_target_properties(gpiod::gpiodcxx PROPERTIES
    IMPORTED_LOCATION "${gpiodcxx_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${gpiodcxx_INCLUDE_DIR}")
endif()
