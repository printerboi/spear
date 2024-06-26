cmake_minimum_required(VERSION 3.16)


target_sources(spear
        PUBLIC JSON-Handler/JSONHandler.cpp
        PUBLIC JSON-Handler/JSONHandler.h
        PUBLIC Rapl-reader/PowercapReader.cpp
        PUBLIC Rapl-reader/PowercapReader.h
        PUBLIC Rapl-reader/RegisterReader.cpp
        PUBLIC Rapl-reader/RegisterReader.h
        PUBLIC LLVM-Handler/InstructionCategory.cpp
        PUBLIC LLVM-Handler/InstructionCategory.h
        PUBLIC LLVM-Handler/EnergyFunction.h
        PUBLIC LLVM-Handler/EnergyFunction.cpp
        PUBLIC LLVM-Handler/LLVMHandler.cpp
        PUBLIC LLVM-Handler/LLVMHandler.h
        PUBLIC Profiler/Profiler.cpp
        PUBLIC Profiler/Profiler.h
        PUBLIC ProgramGraph/ProgramGraph.cpp
        PUBLIC ProgramGraph/ProgramGraph.h
        LoopTree/LoopTree.cpp
        LoopTree/LoopTree.h
        ProgramGraph/LoopNode.cpp
        ProgramGraph/Node.cpp
        ProgramGraph/Edge.cpp
        FunctionTree/FunctionTree.cpp
        FunctionTree/FunctionTree.h
        )

add_library(spearlib SHARED
        JSON-Handler/JSONHandler.cpp
        JSON-Handler/JSONHandler.h
        Rapl-reader/PowercapReader.cpp
        Rapl-reader/PowercapReader.h
        Rapl-reader/RegisterReader.cpp
        Rapl-reader/RegisterReader.h
        LLVM-Handler/LLVMHandler.cpp
        LLVM-Handler/LLVMHandler.h
        LLVM-Handler/InstructionCategory.cpp
        LLVM-Handler/InstructionCategory.h
        Profiler/Profiler.cpp
        Profiler/Profiler.h
        ProgramGraph/ProgramGraph.cpp
        ProgramGraph/ProgramGraph.h
        ../passes/energy/energy.cpp
        LoopTree/LoopTree.cpp
        LoopTree/LoopTree.h
        ProgramGraph/LoopNode.cpp
        ProgramGraph/Node.cpp
        ProgramGraph/Edge.cpp
        FunctionTree/FunctionTree.cpp
        FunctionTree/FunctionTree.h AnalysisStrategy/AnalysisStrategy.h LLVM-Handler/EnergyFunction.h LLVM-Handler/EnergyFunction.cpp CLIHandler/CLIHandler.cpp CLIHandler/CLIHandler.h CLIHandler/CLIOptions.cpp CLIHandler/CLIOptions.h CLIHandler/CLIExceptions.h ProgramGraph/InstructionElement.cpp)
