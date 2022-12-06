cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(rapl_reader
        RaplReader.cpp
        RaplReader.h RegisterReader.cpp RegisterReader.h)
