project(ba)
cmake_minimum_required(VERSION 3.16)

find_package(LLVM 14 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_PATH})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_subdirectory(src/passes/energy)

add_executable(ba
        main.cpp
        src/include/Bencher/Bencher.cpp
        src/include/Bencher/Bencher.h
        src/include/Rapl-reader/RegisterReader.cpp
        src/include/Rapl-reader/RegisterReader.h
        src/include/Rapl-reader/PowercapReader.cpp
        src/include/Rapl-reader/PowercapReader.h
        src/include/LLVM-Handler/LLVMHandler.cpp
        src/include/LLVM-Handler/LLVMHandler.h
        src/include/LLVM-Handler/InstructionCategory.cpp
        src/include/LLVM-Handler/InstructionCategory.h
        src/include/JSON-Handler/JSONHandler.cpp
        src/include/JSON-Handler/JSONHandler.h
        dist/jsoncpp.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader src/passes)

target_link_libraries(ba ${llvm_libs})