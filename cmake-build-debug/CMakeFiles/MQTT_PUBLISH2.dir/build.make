# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /snap/clion/178/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/178/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/MQTT_PUBLISH2.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/MQTT_PUBLISH2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MQTT_PUBLISH2.dir/flags.make

CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.o: CMakeFiles/MQTT_PUBLISH2.dir/flags.make
CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.o: ../MQTT_PUBLISH2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.o -c "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/MQTT_PUBLISH2.c"

CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/MQTT_PUBLISH2.c" > CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.i

CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/MQTT_PUBLISH2.c" -o CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.s

CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.o: CMakeFiles/MQTT_PUBLISH2.dir/flags.make
CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.o: ../SMP_MQTT_UDP.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.o -c "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/SMP_MQTT_UDP.c"

CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/SMP_MQTT_UDP.c" > CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.i

CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/SMP_MQTT_UDP.c" -o CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.s

# Object files for target MQTT_PUBLISH2
MQTT_PUBLISH2_OBJECTS = \
"CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.o" \
"CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.o"

# External object files for target MQTT_PUBLISH2
MQTT_PUBLISH2_EXTERNAL_OBJECTS =

MQTT_PUBLISH2: CMakeFiles/MQTT_PUBLISH2.dir/MQTT_PUBLISH2.c.o
MQTT_PUBLISH2: CMakeFiles/MQTT_PUBLISH2.dir/SMP_MQTT_UDP.c.o
MQTT_PUBLISH2: CMakeFiles/MQTT_PUBLISH2.dir/build.make
MQTT_PUBLISH2: CMakeFiles/MQTT_PUBLISH2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable MQTT_PUBLISH2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MQTT_PUBLISH2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MQTT_PUBLISH2.dir/build: MQTT_PUBLISH2
.PHONY : CMakeFiles/MQTT_PUBLISH2.dir/build

CMakeFiles/MQTT_PUBLISH2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MQTT_PUBLISH2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MQTT_PUBLISH2.dir/clean

CMakeFiles/MQTT_PUBLISH2.dir/depend:
	cd "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git" "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git" "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug" "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug" "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug/CMakeFiles/MQTT_PUBLISH2.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/MQTT_PUBLISH2.dir/depend

