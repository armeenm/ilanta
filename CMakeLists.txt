cmake_minimum_required(VERSION 3.15...3.17 FATAL_ERROR)

include(cmake/BuildType.cmake)
include(cmake/ExportCompileCommands.cmake)

project(ilanta
  VERSION 0.0.1
  DESCRIPTION "C++ library for Linux-based robotics platforms"
  LANGUAGES CXX)

option(FORCE_COLORED_OUTPUT
  "Always produce ANSI-colored output (GNU/Clang only)" 
  OFF)

option(STATIC_ANALYSIS
  "Enable static analysis tools"
  OFF)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED SHARED)
find_package(gpiodcxx REQUIRED MODULE)
find_package(TensorFlow 2.0 REQUIRED MODULE)

set(BUILD_OPTS 
  -Wshadow
  -Warray-bounds
  -Wzero-length-bounds
  -Wstring-compare
  -Wstringop-overflow
  #-Wmismatched-tags
  #-Wredundant-tags
  -Wduplicated-cond
  -Wlogical-op
  -Wduplicated-branches
  -Wnull-dereference
  -Wold-style-cast
  -Wuseless-cast
  -Wformat=2 
  -Wall
  -Wextra
  -Wconversion
  -Wpedantic
  -pipe
  -flto
  -fsanitize=address
  -fsanitize=undefined)

file(GLOB_RECURSE SRCS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/ilanta/*.cpp")

add_library(ilanta SHARED ${SRCS})
add_library(Ilanta::Ilanta ALIAS ilanta)

target_include_directories(ilanta
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(ilanta PRIVATE ${BUILD_OPTS})
target_compile_features(ilanta PUBLIC cxx_std_20)

target_link_libraries(ilanta PRIVATE
  ubsan
  spdlog::spdlog
  gpiodcxx::gpiodcxx
  TensorFlow_DEP)

set_target_properties(ilanta PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 1
  INTERFACE_ilanta_MAJOR_VERSION 0
  COMPATIBLE_INTERFACE_STRING ilanta_MAJOR_VERSION)

if(FORCE_COLORED_OUTPUT OR CMAKE_GENERATOR STREQUAL "Ninja")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(ilanta PRIVATE -fdiagnostics-color=always)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(ilanta PRIVATE -fcolor-diagnostics)
  endif()
endif()

if(STATIC_ANALYSIS)
  set_target_properties(ilanta PROPERTIES
    CXX_CLANG_TIDY "clang-tidy;-checks=*;-header-filter=.*"
    CXX_CPPCHECK "cppcheck;--std=c++11")
endif()

################################################################################
### Tests
################################################################################

find_package(Catch2 REQUIRED)

file(GLOB_RECURSE TESTS
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS
  "test/*.cpp")

add_executable(tests ${TESTS})

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests PRIVATE asan ubsan Catch2::Catch2 ilanta)
target_compile_options(tests PRIVATE ${BUILD_OPTS})
target_compile_features(tests PRIVATE cxx_std_20)

if(FORCE_COLORED_OUTPUT OR CMAKE_GENERATOR STREQUAL "Ninja")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(tests PRIVATE -fdiagnostics-color=always)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(tests PRIVATE -fcolor-diagnostics)
  endif()
endif()

include(CTest)
include(Catch)
catch_discover_tests(tests)

################################################################################
### Installation
################################################################################

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Ilanta)

install(TARGETS ilanta
  EXPORT ilanta-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

set_target_properties(ilanta PROPERTIES EXPORT_NAME Ilanta)

install(
  DIRECTORY ilanta/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ilanta
  FILES_MATCHING PATTERN "*.hpp")

install(EXPORT ilanta-targets
  FILE IlantaTargets.cmake
  NAMESPACE Ilanta::
  DESTINATION ${INSTALL_CONFIGDIR})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/IlantaConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/IlantaConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/IlantaConfig.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

configure_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/Findgpiodcxx.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/Findgpiodcxx.cmake
  COPYONLY)

configure_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/FindTensorFlow.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/FindTensorFlow.cmake
  COPYONLY)

configure_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/FindTensorFlow.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/Findtensorflow.cmake
  COPYONLY)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/Findgpiodcxx.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/FindTensorFlow.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/Findtensorflow.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/IlantaConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/IlantaConfigVersion.cmake
  DESTINATION ${INSTALL_CONFIGDIR})

export(EXPORT ilanta-targets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/IlantaTargets.cmake
  NAMESPACE Ilanta::)

export(PACKAGE Ilanta)
