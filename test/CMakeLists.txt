if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ..."
    FORCE)
endif ()

option(CODE_COVERAGE "Generate code coverage report" OFF)

define_vector_target(test_vector SHARED EXCLUDE_FROM_ALL)
target_compile_definitions(test_vector PRIVATE VECTOR_TEST)

# On macOS with `dyld`: Alters how symbols are resolved at build time and
# runtime. With -flat_namespace, the linker searches all dylibs on the command
# line and all dylibs those original dylibs depend on. The linker does not
# record which dylib an external symbol came from, so at runtime dyld again
# searches all images and uses the first definition it finds.
include(CheckLinkerFlag)
check_linker_flag(-flat_namespace FLAT_NAMESPACE_FLAG)
if (FLAT_NAMESPACE_FLAG)
  target_link_options(test_vector PRIVATE -flat_namespace)
endif ()

# Replace malloc, realloc, and free with stub_malloc, stub_realloc, and
# stub_free. Then link the stub into the test target.
add_library(stub SHARED EXCLUDE_FROM_ALL "${CMAKE_CURRENT_SOURCE_DIR}/stub.c")
target_compile_definitions(stub INTERFACE
  malloc=stub_malloc
  realloc=stub_realloc
  free=stub_free)
target_link_libraries(test_vector PRIVATE stub)

# Add the --coverage option if code coverage is on
if (CODE_COVERAGE)
  target_compile_options(test_vector PRIVATE --coverage)
  target_link_options(test_vector PUBLIC --coverage)
endif ()

# Define a test `name` from the file test/`name`.c
function(define_test name)
  add_executable("test_${name}" EXCLUDE_FROM_ALL
    "${CMAKE_CURRENT_SOURCE_DIR}/test.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/${name}.c")

  # The _GNU_SOURCE feature test macro must be defined in order to obtain the
  # definitions of RTLD_DEFAULT and RTLD_NEXT from <dlfcn.h>. Define this here
  # (rather than in "test.h") so that it's effective even if an included file,
  # included before "test.h", includes <dlfcn.h>
  target_compile_definitions("test_${name}" PRIVATE _GNU_SOURCE VECTOR_TEST)

  target_compile_options("test_${name}" PRIVATE
    -Wall -Wextra -Werror)
  target_link_libraries("test_${name}" PRIVATE dl test_vector)

  add_test("make_${name}"
    "${CMAKE_COMMAND}"
    --build "${CMAKE_BINARY_DIR}"
    --config "$<CONFIG>"
    --target "test_${name}")
  set_tests_properties("make_${name}" PROPERTIES FIXTURES_SETUP "${name}")

  add_test("${name}" "test_${name}")
  set_tests_properties("${name}" PROPERTIES FIXTURES_REQUIRED "${name}")
endfunction(define_test)

foreach (module IN LISTS VECTOR_MODULE_LIST)
  define_test("vector_${module}")
endforeach (module)

define_test(vector_define)
define_test(vector_delete)
define_test(vector_duplicate)
define_test(vector_ensure)
define_test(vector_shrink)
define_test(vector_swap)

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

add_executable(test_vector_define_fail EXCLUDE_FROM_ALL vector_define.c)
target_compile_options(test_vector_define_fail PRIVATE -Wall -Wextra -Werror)
target_compile_definitions(test_vector_define_fail PRIVATE MAKE_TEST_WILL_FAIL)
target_link_libraries(test_vector_define_fail test_vector dl)
add_test(make_vector_define_fail
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target test_vector_define_fail)
set_tests_properties(make_vector_define_fail PROPERTIES WILL_FAIL true)

# add_executable(test_vector_at_fail EXCLUDE_FROM_ALL vector_at.c)
# target_compile_options(test_vector_at_fail PRIVATE -Wall -Wextra -Werror)
# target_compile_definitions(test_vector_at_fail PRIVATE MAKE_TEST_WILL_FAIL)
# target_link_libraries(test_vector_at_fail vector dl)
# add_test(make_vector_at_fail
#   "${CMAKE_COMMAND}"
#   --build "${CMAKE_BINARY_DIR}"
#   --config "$<CONFIG>"
#   --target test_vector_at_fail)
# set_tests_properties(make_vector_at_fail PROPERTIES WILL_FAIL true)

add_executable(test_vector_resize_fail EXCLUDE_FROM_ALL vector_resize.c)
target_compile_options(test_vector_resize_fail PRIVATE -Wall -Wextra -Werror)
target_compile_definitions(test_vector_resize_fail PRIVATE MAKE_TEST_WILL_FAIL)
target_link_libraries(test_vector_resize_fail test_vector dl)
add_test(make_vector_resize_fail
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config "$<CONFIG>"
  --target test_vector_resize_fail)
set_tests_properties(make_vector_resize_fail PROPERTIES WILL_FAIL true)
