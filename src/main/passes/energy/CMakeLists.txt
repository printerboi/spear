
add_llvm_library(Energy MODULE
         ../../../main/include/JSON-Handler/JSONHandler.cpp
        ../../../main/include/JSON-Handler/JSONHandler.h
        ../../../main/include/Rapl-reader/PowercapReader.cpp
        ../../../main/include/Rapl-reader/PowercapReader.h
        ../../../main/include/LLVM-Handler/InstructionCategory.cpp
        ../../../main/include/LLVM-Handler/InstructionCategory.h
        ../../../main/include/LLVM-Handler/LLVMHandler.cpp
        ../../../main/include/LLVM-Handler/LLVMHandler.h
         ../../../../external/json/jsoncpp.cpp;
        ../../../main/include/Profiler/Profiler.cpp
        ../../../main/include/Profiler/Profiler.h
        ../../../main/include/ProgramTree/ProgramTree.cpp
        ../../../main/include/ProgramTree/ProgramTree.h
        ../../../main/include/LoopTree/LoopTree.cpp
        ../../../main/include/LoopTree/LoopTree.h
        ../../../main/include/ProgramTree/LoopNode.cpp
        ../../../main/include/ProgramTree/Node.cpp
        ../../../main/include/ProgramTree/Edge.cpp
        ../../../main/include/FunctionTree/FunctionTree.cpp
        ../../../main/include/FunctionTree/FunctionTree.h
       energy.cpp
        )

target_compile_options(Energy PUBLIC -fexceptions)