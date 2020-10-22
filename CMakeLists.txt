cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tool")

project("Vector"
  VERSION 0.1.0
  HOMEPAGE_URL https://github.com/ktchen14/vector
  LANGUAGES C)

include(GNUInstallDirs)

file(STRINGS Module.list VECTOR_MODULE_LIST)

# Construct the header list
set(VECTOR_HEADER_LIST "${PROJECT_SOURCE_DIR}/header/vector.h")
foreach (module IN LISTS VECTOR_MODULE_LIST)
  list(APPEND VECTOR_HEADER_LIST "${PROJECT_SOURCE_DIR}/header/vector/${module}.h")
  list(APPEND VECTOR_HEADER_LIST "${PROJECT_SOURCE_DIR}/header/vector/${module}.c")
endforeach (module)

# Construct the source list
set(VECTOR_SOURCE_LIST "${PROJECT_SOURCE_DIR}/source/vector.c")
foreach (module IN LISTS VECTOR_MODULE_LIST)
  list(APPEND VECTOR_SOURCE_LIST "${PROJECT_SOURCE_DIR}/source/vector/${module}.c")
endforeach (module)

function(define_vector_target name)
  # Construct the header list. Mark each header file with HEADER_FILE_ONLY.
  set(header_list "${PROJECT_SOURCE_DIR}/header/vector.h")
  foreach (module IN LISTS VECTOR_MODULE_LIST)
    list(APPEND header_list "${PROJECT_SOURCE_DIR}/header/vector/${module}.h")
    list(APPEND header_list "${PROJECT_SOURCE_DIR}/header/vector/${module}.c")
  endforeach (module)
  set_source_files_properties(${header_list} PROPERTIES HEADER_FILE_ONLY ON)

  # Construct the source list
  set(source_list "${PROJECT_SOURCE_DIR}/source/vector.c")
  foreach (module IN LISTS VECTOR_MODULE_LIST)
    list(APPEND source_list "${PROJECT_SOURCE_DIR}/source/vector/${module}.c")
  endforeach (module)

  add_library(${ARGV} ${header_list} ${source_list})
  target_compile_features("${name}" PUBLIC c_std_11)
  target_include_directories("${name}" PUBLIC
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/header>")
  set_property(TARGET "${name}" PROPERTY PUBLIC_HEADER ${header_list})
endfunction(define_vector_target)

# Library
define_vector_target(vector)
set_property(TARGET vector PROPERTY VERSION "${PROJECT_VERSION}")
target_compile_options(vector PRIVATE -Wall -Wextra)

# Install
install(TARGETS vector
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/vector")

# Documentation
add_subdirectory(documentation)

# If this project is the top level project then include CTest. If testing is on
# then specify the Debug build type.
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # See https://stackoverflow.com/a/57240389 on this hack to hide CTest targets
  set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
  include(CTest)

  if (BUILD_TESTING)
    add_subdirectory(test)
  endif ()
endif ()
