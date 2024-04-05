project(spear)
cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)


find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_PATH})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})

include_directories(${LLVM_INCLUDE_DIRS})

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

find_package(nlohmann_json 3.2.0 REQUIRED)


add_executable(spear src/main/include/CLIHandler/CLIHandler.cpp src/main/include/CLIHandler/CLIHandler.h src/main/include/CLIHandler/CLIOptions.cpp src/main/include/CLIHandler/CLIOptions.h src/main/include/CLIHandler/CLIExceptions.h src/main/include/ProgramGraph/InstructionElement.cpp)
add_subdirectory(src)
target_sources(spear
        PUBLIC main.cpp
        )

llvm_map_components_to_libnames(llvm_libs support core irreader passes)

target_link_libraries(spear LLVM nlohmann_json::nlohmann_json)

enable_testing()