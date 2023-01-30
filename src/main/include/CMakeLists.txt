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