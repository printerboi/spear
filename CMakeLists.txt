project(spear)
cmake_minimum_required(VERSION 3.16)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_PATH})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_SOURCE_DIR .)

include_directories(${LLVM_INCLUDE_DIRS} "${CMAKE_SOURCE_DIR}/external/json")

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)




add_executable(spear)
add_subdirectory(src)
target_sources(spear
        PUBLIC main.cpp
        )

llvm_map_components_to_libnames(llvm_libs support core irreader passes)

target_link_libraries(spear ${llvm_libs})

enable_testing()