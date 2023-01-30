project(Google_tests)

# 'lib' is the folder with Google Test sources
include_directories(../../external/googletest/googletest/include ./../external/googletest/googletest)

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_run sample/sample.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)