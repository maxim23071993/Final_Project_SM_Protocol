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
CMAKE_COMMAND = /snap/clion/175/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/175/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/SMP_MQTT_UDP.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/SMP_MQTT_UDP.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SMP_MQTT_UDP.dir/flags.make

CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.o: CMakeFiles/SMP_MQTT_UDP.dir/flags.make
CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.o: ../SMP_MQTT_UDP.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.o -c "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/SMP_MQTT_UDP.c"

CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/SMP_MQTT_UDP.c" > CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.i

CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/SMP_MQTT_UDP.c" -o CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.s

# Object files for target SMP_MQTT_UDP
SMP_MQTT_UDP_OBJECTS = \
"CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.o"

# External object files for target SMP_MQTT_UDP
SMP_MQTT_UDP_EXTERNAL_OBJECTS =

SMP_MQTT_UDP: CMakeFiles/SMP_MQTT_UDP.dir/SMP_MQTT_UDP.c.o
SMP_MQTT_UDP: CMakeFiles/SMP_MQTT_UDP.dir/build.make
SMP_MQTT_UDP: CMakeFiles/SMP_MQTT_UDP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable SMP_MQTT_UDP"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SMP_MQTT_UDP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SMP_MQTT_UDP.dir/build: SMP_MQTT_UDP
.PHONY : CMakeFiles/SMP_MQTT_UDP.dir/build

CMakeFiles/SMP_MQTT_UDP.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SMP_MQTT_UDP.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SMP_MQTT_UDP.dir/clean

CMakeFiles/SMP_MQTT_UDP.dir/depend:
	cd "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git" "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git" "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug" "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug" "/home/max/Desktop/MQTT Subscribe+msg_que+udp_git/cmake-build-debug/CMakeFiles/SMP_MQTT_UDP.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/SMP_MQTT_UDP.dir/depend
