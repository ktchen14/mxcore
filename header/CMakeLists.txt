# Construct the header list from VECTOR_MODULE_LIST
set(header_list "${CMAKE_CURRENT_SOURCE_DIR}/vector.h")
foreach (module IN LISTS VECTOR_MODULE_LIST)
  list(APPEND header_list "${CMAKE_CURRENT_SOURCE_DIR}/vector/${module}.c")
  list(APPEND header_list "${CMAKE_CURRENT_SOURCE_DIR}/vector/${module}.h")
endforeach (module)
set(VECTOR_HEADER_LIST ${header_list} PARENT_SCOPE)

install(DIRECTORY ./
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  FILES_MATCHING
  PATTERN "*.c" PATTERN "*.h")
