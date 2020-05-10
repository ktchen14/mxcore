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
  # See https://stackoverflow.com/a/57240389 on this hack to hide CTest targets
  set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
  include(CTest)
endif ()

# Test
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  set(CMAKE_BUILD_TYPE Debug)
endif ()

set(VECTOR_MODULE_LIST common access create insert move remove resize search)
set(VECTOR_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")

function(define_vector_target)
  list(POP_FRONT ARGV NAME)

  set(header_list "${VECTOR_SOURCE_DIR}/vector.h")
  foreach (module IN LISTS VECTOR_MODULE_LIST)
    list(APPEND header_list "${VECTOR_SOURCE_DIR}/vector/${module}.h")
    list(APPEND header_list "${VECTOR_SOURCE_DIR}/vector/${module}.c")
    set_source_files_properties("${VECTOR_SOURCE_DIR}/vector/${module}.c"
      PROPERTIES HEADER_FILE_ONLY ON)
  endforeach (module)

  add_library("${NAME}" ${ARGV}
    "${VECTOR_SOURCE_DIR}/vector.h" "${VECTOR_SOURCE_DIR}/vector.c"
    ${header_list})
  set_property(TARGET "${NAME}" PROPERTY PUBLIC_HEADER ${header_list})
endfunction(define_vector_target)

# Library
define_vector_target(vector)
set_property(TARGET vector PROPERTY VERSION "${PROJECT_VERSION}")
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
