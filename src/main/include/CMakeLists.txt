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
        PUBLIC ../../../external/json/jsoncpp.cpp;
        PUBLIC Profiler/Profiler.cpp
        PUBLIC Profiler/Profiler.h
        PUBLIC ProgramTree/ProgramTree.cpp
        PUBLIC ProgramTree/ProgramTree.h
        LoopTree/LoopTree.cpp
        LoopTree/LoopTree.h
        ProgramTree/LoopNode.cpp
        ProgramTree/LoopNode.h
        ProgramTree/Node.cpp
        ProgramTree/Node.h
        ProgramTree/Edge.cpp
        ProgramTree/Edge.h
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
        Profiler/Profiler.cpp
        Profiler/Profiler.h
        ../../../external/json/jsoncpp.cpp
        ProgramTree/ProgramTree.cpp
        ProgramTree/ProgramTree.h
        ../passes/energy/energy.cpp
        LoopTree/LoopTree.cpp
        LoopTree/LoopTree.h
        ProgramTree/LoopNode.cpp
        ProgramTree/LoopNode.h
        ProgramTree/Node.cpp
        ProgramTree/Node.h
        ProgramTree/Edge.cpp
        ProgramTree/Edge.h
        )