function(define_test name)
  add_executable(test_${name} ${name}.c $<TARGET_PROPERTY:mxcore,SOURCES>)
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
