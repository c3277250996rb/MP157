# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/alientek/tools/cmake-3.16.2/bin/cmake

# The command to remove a file.
RM = /home/alientek/tools/cmake-3.16.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alientek/ZAT/Applications/33_mqtt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alientek/ZAT/Applications/33_mqtt/build

# Include any dependencies generated for this target.
include CMakeFiles/33_mqtt_client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/33_mqtt_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/33_mqtt_client.dir/flags.make

CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.o: CMakeFiles/33_mqtt_client.dir/flags.make
CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.o: ../src/33_mqtt_client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/ZAT/Applications/33_mqtt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.o"
	/opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.o   -c /home/alientek/ZAT/Applications/33_mqtt/src/33_mqtt_client.c

CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.i"
	/opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alientek/ZAT/Applications/33_mqtt/src/33_mqtt_client.c > CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.i

CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.s"
	/opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alientek/ZAT/Applications/33_mqtt/src/33_mqtt_client.c -o CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.s

# Object files for target 33_mqtt_client
33_mqtt_client_OBJECTS = \
"CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.o"

# External object files for target 33_mqtt_client
33_mqtt_client_EXTERNAL_OBJECTS =

/home/alientek/ZAT/obj/33_mqtt_client: CMakeFiles/33_mqtt_client.dir/src/33_mqtt_client.c.o
/home/alientek/ZAT/obj/33_mqtt_client: CMakeFiles/33_mqtt_client.dir/build.make
/home/alientek/ZAT/obj/33_mqtt_client: CMakeFiles/33_mqtt_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alientek/ZAT/Applications/33_mqtt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable /home/alientek/ZAT/obj/33_mqtt_client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/33_mqtt_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/33_mqtt_client.dir/build: /home/alientek/ZAT/obj/33_mqtt_client

.PHONY : CMakeFiles/33_mqtt_client.dir/build

CMakeFiles/33_mqtt_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/33_mqtt_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/33_mqtt_client.dir/clean

CMakeFiles/33_mqtt_client.dir/depend:
	cd /home/alientek/ZAT/Applications/33_mqtt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alientek/ZAT/Applications/33_mqtt /home/alientek/ZAT/Applications/33_mqtt /home/alientek/ZAT/Applications/33_mqtt/build /home/alientek/ZAT/Applications/33_mqtt/build /home/alientek/ZAT/Applications/33_mqtt/build/CMakeFiles/33_mqtt_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/33_mqtt_client.dir/depend

