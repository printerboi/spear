cmake_minimum_required(VERSION 3.16)
project(spear VERSION 0.2.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)

# Should we use the external version of the json lib or use the locally installed version
option(USE_EXTERNAL_JSON "Use external JSON lib" OFF)

# ========= LLVM DANGER ZONE =========
# ===== PLEASE EDIT WITH CAUTION =====

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
# Include the necessary LLVM files
include_directories(${LLVM_INCLUDE_PATH})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})

include_directories(${LLVM_INCLUDE_DIRS})

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)
llvm_map_components_to_libnames(llvm_libs support core irreader passes)

# ========= LLVM DANGER ZONE =========

# Add the external subdirectory where we handle third party libs
add_subdirectory(external)

# Include all files used by spear
add_library(SpearLib
        src/main/include/JSON-Handler/JSONHandler.cpp
        src/main/include/JSON-Handler/JSONHandler.h
        src/main/include/Rapl-reader/PowercapReader.cpp
        src/main/include/Rapl-reader/PowercapReader.h
        src/main/include/Rapl-reader/RegisterReader.cpp
        src/main/include/Rapl-reader/RegisterReader.h
        src/main/include/LLVM-Handler/InstructionCategory.cpp
        src/main/include/LLVM-Handler/InstructionCategory.h
        src/main/include/LLVM-Handler/EnergyFunction.h
        src/main/include/LLVM-Handler/EnergyFunction.cpp
        src/main/include/LLVM-Handler/LLVMHandler.cpp
        src/main/include/LLVM-Handler/LLVMHandler.h
        src/main/include/Profiler/Profiler.cpp
        src/main/include/Profiler/Profiler.h
        src/main/include/ProgramGraph/ProgramGraph.cpp
        src/main/include/ProgramGraph/ProgramGraph.h
        src/main/include/LoopTree/LoopTree.cpp
        src/main/include/LoopTree/LoopTree.h
        src/main/include/ProgramGraph/LoopNode.cpp
        src/main/include/ProgramGraph/Node.cpp
        src/main/include/ProgramGraph/Edge.cpp
        src/main/include/FunctionTree/FunctionTree.cpp
        src/main/include/FunctionTree/FunctionTree.h
        src/main/include/CLIHandler/CLIHandler.cpp
        src/main/include/CLIHandler/CLIHandler.h
        src/main/include/CLIHandler/CLIOptions.cpp
        src/main/include/CLIHandler/CLIOptions.h
        src/main/include/ProgramGraph/InstructionElement.cpp
)

# Add spear as exeexecutable depending on main.cpp
add_executable(spear main.cpp)

# Link the json lib and LLVM to spears lib
target_link_libraries(SpearLib PRIVATE LLVM nlohmann_json::nlohmann_json)
# Link the lib itself to the executable of spear
target_link_libraries(spear PRIVATE SpearLib LLVM nlohmann_json::nlohmann_json)

enable_testing()