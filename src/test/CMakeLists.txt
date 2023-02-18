project(Google_tests)

# 'lib' is the folder with Google Test sources
include_directories(../../external/googletest/googletest/include ./../external/googletest/googletest)

llvm_map_components_to_libnames(llvm_libs support core irreader passes)

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run sample/sample.cpp JSON-Handler/json_handler_test.cpp LLVM-Handler/Instruction_category_test.cpp LLVM-Handler/LLVM_handler_test.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main ${llvm_libs} spearlib)