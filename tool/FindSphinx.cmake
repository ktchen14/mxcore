find_program(SPHINX_EXECUTABLE
  NAMES sphinx-build
  DOC "Sphinx Documentation Builder (sphinx-doc.org)")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
  Sphinx DEFAULT_MSG SPHINX_EXECUTABLE)
