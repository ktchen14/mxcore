cmake_minimum_required(VERSION 3.12)

project("MX Core"
  VERSION 0.0.1
  HOMEPAGE_URL https://github.com/ktchen14/mxcore
  LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Module")

# If this project is the top level project then include CTest. If testing is on
# then create shared libraries and specify the Debug build type.
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  if (BUILD_TESTING)
    set(BUILD_SHARED_LIBS ON)
    set(CMAKE_BUILD_TYPE Debug)
  endif ()
endif ()

# Library
add_library(vector
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/access.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/access.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/common.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/create.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/create.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/insert.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/insert.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/move.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/move.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/remove.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/remove.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/resize.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/resize.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/search.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/search.h"
)
set_source_files_properties(
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/access.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/create.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/insert.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/move.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/remove.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/resize.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/search.c"
  PROPERTIES HEADER_FILE_ONLY ON)
set_property(TARGET vector PROPERTY PUBLIC_HEADER
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/access.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/access.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/common.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/create.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/create.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/insert.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/insert.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/move.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/move.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/remove.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/remove.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/resize.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/resize.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/search.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/vector/search.h")
set_property(TARGET vector PROPERTY
  VERSION ${PROJECT_VERSION})
target_compile_features(vector PUBLIC c_std_11)
target_compile_options(vector PRIVATE -Wall -Wextra)

# Install
include(GNUInstallDirs)
install(TARGETS vector
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/vector")

# Documentation
find_package(Doxygen QUIET)
if (DOXYGEN_FOUND)
  include(CMakeDoxygen.cmake)
endif ()

# Test
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(test)
endif ()
