cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tool")

file(READ VERSION PROJECT_VERSION)
string(STRIP "${PROJECT_VERSION}" PROJECT_VERSION)
project("Vector"
  VERSION "${PROJECT_VERSION}"
  DESCRIPTION "An unobtrusive vector implementation in C"
  HOMEPAGE_URL https://github.com/ktchen14/vector
  LANGUAGES C)

include(GNUInstallDirs)

# Read Module.list into VECTOR_MODULE_LIST. This must be done before header/ or
# source/ is handled.
file(STRINGS Module.list VECTOR_MODULE_LIST)
add_subdirectory(header)
add_subdirectory(source)

function(define_vector_target name)
  add_library(${ARGV} ${VECTOR_HEADER_LIST} ${VECTOR_SOURCE_LIST})
  target_compile_features("${name}" PUBLIC c_std_11)
  target_compile_options("${name}" PRIVATE -Wall)
  target_include_directories("${name}" PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/header>"
    "$<INSTALL_INTERFACE:include>")
  set_source_files_properties(${VECTOR_HEADER_LIST} PROPERTIES
    HEADER_FILE_ONLY ON)
  set_property(TARGET "${name}" PROPERTY VERSION "${Vector_VERSION}")
endfunction(define_vector_target)

# Main target
define_vector_target(vector)
install(TARGETS vector
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

add_subdirectory(data)
add_subdirectory(documentation)

# Include test if we're in the main project
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  add_subdirectory(test)
endif ()
