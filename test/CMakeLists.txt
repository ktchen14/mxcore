# On macOS with `dyld`: Alters how symbols are resolved at build time and
# runtime. With -flat_namespace, the linker searches all dylibs on the command
# line and all dylibs those original dylibs depend on. The linker does not
# record which dylib an external symbol came from, so at runtime dyld again
# searches all images and uses the first definition it finds.
include(CheckLinkerFlag)
check_linker_flag(-flat_namespace FLAT_NAMESPACE_FLAG)
if (FLAT_NAMESPACE_FLAG)
  target_link_options(mxcore PRIVATE -flat_namespace)
endif ()

# Define a test `name` from the file test/`name`.c
function(define_test name)
  add_executable(test_${name} EXCLUDE_FROM_ALL ${name}.c)
  target_compile_options(test_${name} PRIVATE -Wall -Wextra -Werror)
  target_link_libraries(test_${name} mxcore dl)

  add_test(make_${name}
    "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --config "$<CONFIG>"
    --target test_${name})
  set_tests_properties(make_${name} PROPERTIES FIXTURES_SETUP ${name})

  add_test(${name} test_${name})
  set_tests_properties(${name} PROPERTIES FIXTURES_REQUIRED ${name})
endfunction(define_test)

define_test(vector_create)
define_test(vector_duplicate)
define_test(vector_delete)
define_test(vector_at)

add_executable(test_vector_at_fail EXCLUDE_FROM_ALL
  vector_at.c $<TARGET_PROPERTY:mxcore,SOURCES>)
target_compile_options(test_vector_at_fail PRIVATE -Wall -Wextra -Werror)
target_compile_definitions(test_vector_at_fail PRIVATE MAKE_TEST_WILL_FAIL)
add_test(make_vector_at_fail
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target test_vector_at_fail)
set_tests_properties(make_vector_at_fail PROPERTIES WILL_FAIL true)

define_test(vector_access)
define_test(vector_resize)
