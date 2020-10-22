cmake_minimum_required(VERSION 3.12)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tool")

project("Vector"
  VERSION 0.1.0
  HOMEPAGE_URL https://github.com/ktchen14/vector
  LANGUAGES C)

include(GNUInstallDirs)

# Read Module.list into VECTOR_MODULE_LIST. This must be done before header/ or
# source/ is handled.
file(STRINGS Module.list VECTOR_MODULE_LIST)
add_subdirectory(header)
add_subdirectory(source)

function(define_vector_target name)
  set_source_files_properties(${VECTOR_HEADER_LIST} PROPERTIES
    HEADER_FILE_ONLY ON)
  add_library(${ARGV} ${VECTOR_HEADER_LIST} ${VECTOR_SOURCE_LIST})
  target_compile_features("${name}" PUBLIC c_std_11)
  target_compile_options("${name}" PRIVATE -Wall)
  target_include_directories("${name}" PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/header>"
    "$<INSTALL_INTERFACE:include>")
  set_property(TARGET vector PROPERTY VERSION "${Vector_VERSION}")
endfunction(define_vector_target)

# Main target
define_vector_target(vector)
install(TARGETS vector
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

add_subdirectory(documentation)

# Include test we're in the top level project
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  add_subdirectory(test)
endif ()
