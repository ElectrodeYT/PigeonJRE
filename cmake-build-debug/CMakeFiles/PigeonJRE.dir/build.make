# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_SOURCE_DIR = /home/alexander/projects/PigeonJRE

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alexander/projects/PigeonJRE/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/PigeonJRE.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/PigeonJRE.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PigeonJRE.dir/flags.make

CMakeFiles/PigeonJRE.dir/src/class.cpp.o: CMakeFiles/PigeonJRE.dir/flags.make
CMakeFiles/PigeonJRE.dir/src/class.cpp.o: ../src/class.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alexander/projects/PigeonJRE/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PigeonJRE.dir/src/class.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PigeonJRE.dir/src/class.cpp.o -c /home/alexander/projects/PigeonJRE/src/class.cpp

CMakeFiles/PigeonJRE.dir/src/class.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PigeonJRE.dir/src/class.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alexander/projects/PigeonJRE/src/class.cpp > CMakeFiles/PigeonJRE.dir/src/class.cpp.i

CMakeFiles/PigeonJRE.dir/src/class.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PigeonJRE.dir/src/class.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alexander/projects/PigeonJRE/src/class.cpp -o CMakeFiles/PigeonJRE.dir/src/class.cpp.s

CMakeFiles/PigeonJRE.dir/src/frame.cpp.o: CMakeFiles/PigeonJRE.dir/flags.make
CMakeFiles/PigeonJRE.dir/src/frame.cpp.o: ../src/frame.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alexander/projects/PigeonJRE/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/PigeonJRE.dir/src/frame.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PigeonJRE.dir/src/frame.cpp.o -c /home/alexander/projects/PigeonJRE/src/frame.cpp

CMakeFiles/PigeonJRE.dir/src/frame.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PigeonJRE.dir/src/frame.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alexander/projects/PigeonJRE/src/frame.cpp > CMakeFiles/PigeonJRE.dir/src/frame.cpp.i

CMakeFiles/PigeonJRE.dir/src/frame.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PigeonJRE.dir/src/frame.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alexander/projects/PigeonJRE/src/frame.cpp -o CMakeFiles/PigeonJRE.dir/src/frame.cpp.s

CMakeFiles/PigeonJRE.dir/src/jre.cpp.o: CMakeFiles/PigeonJRE.dir/flags.make
CMakeFiles/PigeonJRE.dir/src/jre.cpp.o: ../src/jre.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alexander/projects/PigeonJRE/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/PigeonJRE.dir/src/jre.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PigeonJRE.dir/src/jre.cpp.o -c /home/alexander/projects/PigeonJRE/src/jre.cpp

CMakeFiles/PigeonJRE.dir/src/jre.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PigeonJRE.dir/src/jre.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alexander/projects/PigeonJRE/src/jre.cpp > CMakeFiles/PigeonJRE.dir/src/jre.cpp.i

CMakeFiles/PigeonJRE.dir/src/jre.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PigeonJRE.dir/src/jre.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alexander/projects/PigeonJRE/src/jre.cpp -o CMakeFiles/PigeonJRE.dir/src/jre.cpp.s

CMakeFiles/PigeonJRE.dir/src/main.cpp.o: CMakeFiles/PigeonJRE.dir/flags.make
CMakeFiles/PigeonJRE.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alexander/projects/PigeonJRE/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/PigeonJRE.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PigeonJRE.dir/src/main.cpp.o -c /home/alexander/projects/PigeonJRE/src/main.cpp

CMakeFiles/PigeonJRE.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PigeonJRE.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alexander/projects/PigeonJRE/src/main.cpp > CMakeFiles/PigeonJRE.dir/src/main.cpp.i

CMakeFiles/PigeonJRE.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PigeonJRE.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alexander/projects/PigeonJRE/src/main.cpp -o CMakeFiles/PigeonJRE.dir/src/main.cpp.s

CMakeFiles/PigeonJRE.dir/src/thread.cpp.o: CMakeFiles/PigeonJRE.dir/flags.make
CMakeFiles/PigeonJRE.dir/src/thread.cpp.o: ../src/thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alexander/projects/PigeonJRE/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/PigeonJRE.dir/src/thread.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PigeonJRE.dir/src/thread.cpp.o -c /home/alexander/projects/PigeonJRE/src/thread.cpp

CMakeFiles/PigeonJRE.dir/src/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PigeonJRE.dir/src/thread.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alexander/projects/PigeonJRE/src/thread.cpp > CMakeFiles/PigeonJRE.dir/src/thread.cpp.i

CMakeFiles/PigeonJRE.dir/src/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PigeonJRE.dir/src/thread.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alexander/projects/PigeonJRE/src/thread.cpp -o CMakeFiles/PigeonJRE.dir/src/thread.cpp.s

# Object files for target PigeonJRE
PigeonJRE_OBJECTS = \
"CMakeFiles/PigeonJRE.dir/src/class.cpp.o" \
"CMakeFiles/PigeonJRE.dir/src/frame.cpp.o" \
"CMakeFiles/PigeonJRE.dir/src/jre.cpp.o" \
"CMakeFiles/PigeonJRE.dir/src/main.cpp.o" \
"CMakeFiles/PigeonJRE.dir/src/thread.cpp.o"

# External object files for target PigeonJRE
PigeonJRE_EXTERNAL_OBJECTS =

PigeonJRE: CMakeFiles/PigeonJRE.dir/src/class.cpp.o
PigeonJRE: CMakeFiles/PigeonJRE.dir/src/frame.cpp.o
PigeonJRE: CMakeFiles/PigeonJRE.dir/src/jre.cpp.o
PigeonJRE: CMakeFiles/PigeonJRE.dir/src/main.cpp.o
PigeonJRE: CMakeFiles/PigeonJRE.dir/src/thread.cpp.o
PigeonJRE: CMakeFiles/PigeonJRE.dir/build.make
PigeonJRE: CMakeFiles/PigeonJRE.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alexander/projects/PigeonJRE/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable PigeonJRE"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PigeonJRE.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PigeonJRE.dir/build: PigeonJRE
.PHONY : CMakeFiles/PigeonJRE.dir/build

CMakeFiles/PigeonJRE.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PigeonJRE.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PigeonJRE.dir/clean

CMakeFiles/PigeonJRE.dir/depend:
	cd /home/alexander/projects/PigeonJRE/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alexander/projects/PigeonJRE /home/alexander/projects/PigeonJRE /home/alexander/projects/PigeonJRE/cmake-build-debug /home/alexander/projects/PigeonJRE/cmake-build-debug /home/alexander/projects/PigeonJRE/cmake-build-debug/CMakeFiles/PigeonJRE.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PigeonJRE.dir/depend

