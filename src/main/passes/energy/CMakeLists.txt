
add_llvm_library(Energy MODULE
       energy.cpp
        )

target_compile_options(Energy PUBLIC -fexceptions)