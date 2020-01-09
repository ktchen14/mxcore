# Honor link flags in try_compile() source-file signature
cmake_policy(SET CMP0056 NEW)

include(CheckCCompilerFlag)

function(check_linker_flag flag variable)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${flag}")
  check_c_compiler_flag("" ${variable})
endfunction()
