cmake_minimum_required(VERSION 3.12)

project("MX Core"
  VERSION 0.0.1
  HOMEPAGE_URL https://github.com/ktchen14/mxcore
  LANGUAGES C)

# Library
add_library(mxcore
  "${CMAKE_CURRENT_SOURCE_DIR}/source/common.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector.h")
set_property(TARGET mxcore PROPERTY
  PUBLIC_HEADER source/common.h source/vector.h)
set_property(TARGET mxcore PROPERTY
  VERSION ${PROJECT_VERSION})
target_compile_features(mxcore PUBLIC c_std_11)
target_compile_options(mxcore PRIVATE -Wall -Wextra)

# Install
include(GNUInstallDirs)
install(TARGETS mxcore
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mxcore")

# Documentation
find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
  include(CMakeDoxygen.cmake)
endif ()

# Test
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  if (BUILD_TESTING)
    add_subdirectory(test)
  endif ()
endif ()
