project(ba)
cmake_minimum_required(VERSION 3.16)

find_package(LLVM 14 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_PATH})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions(${LLVM_DEFINITIONS})

add_executable(ba
        main.cpp
        src/include/Bencher/Bencher.cpp
        src/include/Bencher/Bencher.h
        src/include/Rapl-reader/RegisterReader.cpp
        src/include/Rapl-reader/RegisterReader.h
        src/include/Rapl-reader/PowercapReader.cpp src/include/Rapl-reader/PowercapReader.h src/include/CSV-Handler/CSVHandler.cpp src/include/CSV-Handler/CSVHandler.h src/include/LLVM-Handler/LLVMHandler.cpp src/include/LLVM-Handler/LLVMHandler.h)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(ba ${llvm_libs})