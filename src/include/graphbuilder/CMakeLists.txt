cmake_minimum_required(VERSION 3.23)
project(graphbuilder)

set(CMAKE_CXX_STANDARD 14)

find_package(LLVM 14 REQUIRED CONFIG)
include_directories($ENV{LLVM_PATH}/include/)
include_directories(.)

add_executable(graphbuilder
        Node.cpp
        Node.h
        ProgramDependenceGraph.cpp
        ProgramDependenceGraph.h
        GraphBuilder.cpp
        GraphBuilder.h)

llvm_map_components_to_libnames(llvm_libs support core irreader target)
target_link_libraries(graphbuilder ${llvm_libs})