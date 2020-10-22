find_program(SPHINX_EXECUTABLE
  NAMES sphinx-build
  DOC "Sphinx documentation generation tool (http://www.sphinx-doc.org/)")
mark_as_advanced(SPHINX_EXECUTABLE)

if (SPHINX_EXECUTABLE)
  execute_process(COMMAND "${SPHINX_EXECUTABLE}" --version
    OUTPUT_VARIABLE sphinx_version
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  if (sphinx_version MATCHES "^sphinx-build [0-9]")
    string(REPLACE "sphinx-build " "" SPHINX_VERSION_STRING "${sphinx_version}")
  endif ()
  unset(sphinx_version)
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sphinx
  REQUIRED_VARS SPHINX_EXECUTABLE
  VERSION_VAR SPHINX_VERSION_STRING)
