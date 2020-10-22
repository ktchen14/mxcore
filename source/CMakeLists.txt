# Construct the source list from VECTOR_MODULE_LIST
set(source_list "${CMAKE_CURRENT_SOURCE_DIR}/vector.c")
foreach (module IN LISTS VECTOR_MODULE_LIST)
  list(APPEND source_list "${CMAKE_CURRENT_SOURCE_DIR}/vector/${module}.c")
endforeach (module)
set(VECTOR_SOURCE_LIST ${source_list} PARENT_SCOPE)
