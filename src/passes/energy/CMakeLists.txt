
add_llvm_library(LLVMEnergy MODULE
        energy.cpp
        ../../include/LLVM-Handler/LLVMHandler.h
        ../../include/LLVM-Handler/LLVMHandler.cpp
        ../../include/LLVM-Handler/InstructionCategory.cpp
        ../../include/LLVM-Handler/InstructionCategory.h
        ../../include/JSON-Handler/JSONHandler.cpp
        ../../include/JSON-Handler/JSONHandler.h
        ../../../dist/jsoncpp.cpp
)

target_compile_options(LLVMEnergy PUBLIC -fexceptions)
