project(ba)
cmake_minimum_required(VERSION 3.23)

find_package(LLVM 14 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_PATH})

add_subdirectory(src)
