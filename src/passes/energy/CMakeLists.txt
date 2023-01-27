
add_llvm_library(Energy MODULE
        energy.h
        energy.cpp
        ../../include/LLVM-Handler/LLVMHandler.h
        ../../include/LLVM-Handler/LLVMHandler.cpp
        ../../include/LLVM-Handler/LoopTree.h
        ../../include/LLVM-Handler/LoopTree.cpp
        ../../include/LLVM-Handler/InstructionCategory.cpp
        ../../include/LLVM-Handler/InstructionCategory.h
        ../../include/JSON-Handler/JSONHandler.cpp
        ../../include/JSON-Handler/JSONHandler.h
        ../../../dist/jsoncpp.cpp
)

target_compile_options(Energy PUBLIC -fexceptions)