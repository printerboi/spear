cmake_minimum_required(VERSION 3.16)

target_sources(ba
        PUBLIC JSON-Handler/JSONHandler.cpp
        PUBLIC JSON-Handler/JSONHandler.h
        PUBLIC Rapl-reader/PowercapReader.cpp
        PUBLIC Rapl-reader/PowercapReader.h
        PUBLIC LLVM-Handler/InstructionCategory.cpp
        PUBLIC LLVM-Handler/InstructionCategory.h
        PUBLIC LLVM-Handler/LLVMHandler.cpp
        PUBLIC LLVM-Handler/LLVMHandler.h
        PUBLIC LLVM-Handler/LoopTree.cpp
        PUBLIC LLVM-Handler/LoopTree.h
        PUBLIC Profiler/Profiler.cpp
        PUBLIC Profiler/Profiler.h
        )

add_library(spear SHARED
        JSON-Handler/JSONHandler.cpp
        JSON-Handler/JSONHandler.h
        Rapl-reader/PowercapReader.cpp
        Rapl-reader/PowercapReader.h
        LLVM-Handler/InstructionCategory.cpp
        LLVM-Handler/InstructionCategory.h
        LLVM-Handler/LLVMHandler.cpp
        LLVM-Handler/LLVMHandler.h
        LLVM-Handler/LoopTree.cpp
        LLVM-Handler/LoopTree.h
        Profiler/Profiler.cpp
        Profiler/Profiler.h
        ../../../external/json/jsoncpp.cpp
        )