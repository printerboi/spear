cmake_minimum_required(VERSION 3.23)
project(rapl_reader)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(rapl_reader
        main.cpp
        main.h
        RaplReader.cpp
        RaplReader.h)
