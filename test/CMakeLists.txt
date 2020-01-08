function(define_test name)
  add_executable(test_${name} EXCLUDE_FROM_ALL
    ${name}.c $<TARGET_PROPERTY:mxcore,SOURCES>)
  target_compile_options(test_${name} PRIVATE -Wall -Wextra -Werror)

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
