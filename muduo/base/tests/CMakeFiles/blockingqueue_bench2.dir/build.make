# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tuchi/muduo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tuchi/muduo

# Include any dependencies generated for this target.
include muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/compiler_depend.make

# Include the progress variables for this target.
include muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/progress.make

# Include the compile flags for this target's objects.
include muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/flags.make

muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.o: muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/flags.make
muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.o: muduo/base/tests/BlockingQueue_bench2.cc
muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.o: muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tuchi/muduo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.o"
	cd /home/tuchi/muduo/muduo/base/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.o -MF CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.o.d -o CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.o -c /home/tuchi/muduo/muduo/base/tests/BlockingQueue_bench2.cc

muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.i"
	cd /home/tuchi/muduo/muduo/base/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tuchi/muduo/muduo/base/tests/BlockingQueue_bench2.cc > CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.i

muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.s"
	cd /home/tuchi/muduo/muduo/base/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tuchi/muduo/muduo/base/tests/BlockingQueue_bench2.cc -o CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.s

# Object files for target blockingqueue_bench2
blockingqueue_bench2_OBJECTS = \
"CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.o"

# External object files for target blockingqueue_bench2
blockingqueue_bench2_EXTERNAL_OBJECTS =

bin/blockingqueue_bench2: muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/BlockingQueue_bench2.cc.o
bin/blockingqueue_bench2: muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/build.make
bin/blockingqueue_bench2: lib/libmuduo_base.a
bin/blockingqueue_bench2: muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tuchi/muduo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bin/blockingqueue_bench2"
	cd /home/tuchi/muduo/muduo/base/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/blockingqueue_bench2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/build: bin/blockingqueue_bench2
.PHONY : muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/build

muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/clean:
	cd /home/tuchi/muduo/muduo/base/tests && $(CMAKE_COMMAND) -P CMakeFiles/blockingqueue_bench2.dir/cmake_clean.cmake
.PHONY : muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/clean

muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/depend:
	cd /home/tuchi/muduo && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tuchi/muduo /home/tuchi/muduo/muduo/base/tests /home/tuchi/muduo /home/tuchi/muduo/muduo/base/tests /home/tuchi/muduo/muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : muduo/base/tests/CMakeFiles/blockingqueue_bench2.dir/depend

