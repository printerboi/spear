# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit/cmake-build-debug-coverage

# Utility rule file for static_testdata.

# Include any custom commands dependencies for this target.
include src/test/CMakeFiles/static_testdata.dir/compiler_depend.make

# Include the progress variables for this target.
include src/test/CMakeFiles/static_testdata.dir/progress.make

static_testdata: src/test/CMakeFiles/static_testdata.dir/build.make
.PHONY : static_testdata

# Rule to build all files generated by this target.
src/test/CMakeFiles/static_testdata.dir/build: static_testdata
.PHONY : src/test/CMakeFiles/static_testdata.dir/build

src/test/CMakeFiles/static_testdata.dir/clean:
	cd /home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit/cmake-build-debug-coverage/src/test && $(CMAKE_COMMAND) -P CMakeFiles/static_testdata.dir/cmake_clean.cmake
.PHONY : src/test/CMakeFiles/static_testdata.dir/clean

src/test/CMakeFiles/static_testdata.dir/depend:
	cd /home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit/cmake-build-debug-coverage && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit /home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit/src/test /home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit/cmake-build-debug-coverage /home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit/cmake-build-debug-coverage/src/test /home/maximiliank/Dokumente/workbench/Bachelor/bachelorarbeit/cmake-build-debug-coverage/src/test/CMakeFiles/static_testdata.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/test/CMakeFiles/static_testdata.dir/depend

