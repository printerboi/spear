project(ba)
cmake_minimum_required(VERSION 3.16)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_PATH})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)




add_executable(ba src/main/include/ProgramTree/ProgramTree.cpp src/main/include/ProgramTree/ProgramTree.cpp src/main/include/ProgramTree/ProgramTree.h src/main/include/ProgramTree/Node.cpp src/main/include/ProgramTree/Node.h src/main/include/ProgramTree/LoopNode.cpp src/main/include/ProgramTree/LoopNode.h src/main/include/ProgramTree/IfNode.cpp src/main/include/ProgramTree/IfNode.h)
add_subdirectory(src)
target_sources(ba
        PUBLIC main.cpp
        PUBLIC external/json/jsoncpp.cpp
        )

llvm_map_components_to_libnames(llvm_libs support core irreader passes)

target_link_libraries(ba ${llvm_libs})

enable_testing()