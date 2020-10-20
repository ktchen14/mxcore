option(DOCUMENTATION "Generate API documentation with Sphinx" OFF)

if (NOT DOCUMENTATION)
  return()
endif ()

find_package(Doxygen REQUIRED)
include(Doxygen.cmake)
doxygen_add_docs(documentation
  ${VECTOR_HEADER_LIST} ${VECTOR_SOURCE_LIST}
  USE_STAMP_FILE
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

add_custom_target(sphinx
  COMMAND sphinx-build
    -b html
    -Daerate_doxygen_root="${CMAKE_CURRENT_BINARY_DIR}/xml"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}/sphinx"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Generating documentation with Sphinx")
