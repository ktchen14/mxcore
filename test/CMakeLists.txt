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
  add_executable(test_${name} EXCLUDE_FROM_ALL
    "${CMAKE_CURRENT_SOURCE_DIR}/test.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/${name}.c")
  target_compile_options(test_${name} PRIVATE
    -Wall -Wextra -Werror -fsanitize=address)
  target_link_libraries(test_${name} PRIVATE mxcore dl)
  target_link_options(test_${name} PRIVATE -fsanitize=address)

  add_test(make_${name}
    "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --config "$<CONFIG>"
    --target test_${name})
  set_tests_properties(make_${name} PROPERTIES FIXTURES_SETUP ${name})

  add_test(${name} test_${name})
  set_tests_properties(${name} PROPERTIES FIXTURES_REQUIRED ${name})
  set_tests_properties(${name} PROPERTIES
    ENVIRONMENT "ASAN_OPTIONS=allocator_may_return_null=1")
endfunction(define_test)

function(define_compilation_test name symbol)
  add_executable(test_${name}_compilation_${symbol} EXCLUDE_FROM_ALL "${name}.c")
  target_compile_definitions(test_${name}_compilation PRIVATE MAKE_TEST_WILL_FAIL)
  add_test(make_${name}_compilation
    "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --config "$<CONFIG>"
    --target test_${name}_compilation_${symbol})
  set_tests_properties(test_${name}_compilation PROPERTIES WILL_FAIL true)
endfunction(define_compilation_test)

define_test(vector_create)
define_test(vector_create_with)
define_test(vector_define)
add_executable(test_vector_define_fail EXCLUDE_FROM_ALL vector_define.c)
target_compile_options(test_vector_define_fail PRIVATE -Wall -Wextra -Werror)
target_compile_definitions(test_vector_define_fail PRIVATE MAKE_TEST_WILL_FAIL)
target_link_libraries(test_vector_define_fail mxcore dl)
add_test(make_vector_define_fail
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target test_vector_define_fail)
set_tests_properties(make_vector_define_fail PROPERTIES WILL_FAIL true)
define_test(vector_duplicate)
define_test(vector_delete)
define_test(vector_at)

add_executable(test_vector_at_fail EXCLUDE_FROM_ALL vector_at.c)
target_compile_options(test_vector_at_fail PRIVATE -Wall -Wextra -Werror)
target_compile_definitions(test_vector_at_fail PRIVATE MAKE_TEST_WILL_FAIL)
target_link_libraries(test_vector_at_fail mxcore dl)
add_test(make_vector_at_fail
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target test_vector_at_fail)
set_tests_properties(make_vector_at_fail PROPERTIES WILL_FAIL true)

define_test(vector_access)
define_test(vector_resize)

add_executable(test_vector_resize_fail EXCLUDE_FROM_ALL vector_resize.c)
target_compile_options(test_vector_resize_fail PRIVATE -Wall -Wextra -Werror)
target_compile_definitions(test_vector_resize_fail PRIVATE MAKE_TEST_WILL_FAIL)
target_link_libraries(test_vector_resize_fail mxcore dl)
add_test(make_vector_resize_fail
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target test_vector_resize_fail)
set_tests_properties(make_vector_resize_fail PROPERTIES WILL_FAIL true)

define_test(vector_shrink)
define_test(vector_swap)
define_test(vector_move)
