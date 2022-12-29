project(ba)
cmake_minimum_required(VERSION 3.16)

find_package(LLVM 14 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_PATH})

add_executable(ba
        main.cpp
        src/include/Bencher/Bencher.cpp
        src/include/Bencher/Bencher.h
        src/include/Rapl-reader/RegisterReader.cpp
        src/include/Rapl-reader/RegisterReader.h
        src/include/Rapl-reader/PowercapReader.cpp src/include/Rapl-reader/PowercapReader.h src/include/CSV-Handler/CSVHandler.cpp src/include/CSV-Handler/CSVHandler.h)
