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
CMAKE_SOURCE_DIR = "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj/build"

# Include any dependencies generated for this target.
include CMakeFiles/mqttClient.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mqttClient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mqttClient.dir/flags.make

CMakeFiles/mqttClient.dir/mqttClient.c.o: CMakeFiles/mqttClient.dir/flags.make
CMakeFiles/mqttClient.dir/mqttClient.c.o: ../mqttClient.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/mqttClient.dir/mqttClient.c.o"
	/opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mqttClient.dir/mqttClient.c.o   -c "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj/mqttClient.c"

CMakeFiles/mqttClient.dir/mqttClient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mqttClient.dir/mqttClient.c.i"
	/opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj/mqttClient.c" > CMakeFiles/mqttClient.dir/mqttClient.c.i

CMakeFiles/mqttClient.dir/mqttClient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mqttClient.dir/mqttClient.c.s"
	/opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj/mqttClient.c" -o CMakeFiles/mqttClient.dir/mqttClient.c.s

# Object files for target mqttClient
mqttClient_OBJECTS = \
"CMakeFiles/mqttClient.dir/mqttClient.c.o"

# External object files for target mqttClient
mqttClient_EXTERNAL_OBJECTS =

/home/alientek/ZAT/obj/mqttClient: CMakeFiles/mqttClient.dir/mqttClient.c.o
/home/alientek/ZAT/obj/mqttClient: CMakeFiles/mqttClient.dir/build.make
/home/alientek/ZAT/obj/mqttClient: CMakeFiles/mqttClient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable /home/alientek/ZAT/obj/mqttClient"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mqttClient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mqttClient.dir/build: /home/alientek/ZAT/obj/mqttClient

.PHONY : CMakeFiles/mqttClient.dir/build

CMakeFiles/mqttClient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mqttClient.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mqttClient.dir/clean

CMakeFiles/mqttClient.dir/depend:
	cd "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj" "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj" "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj/build" "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj/build" "/home/alientek/ZAT/13、Linux C应用编程例程源码/33_mqtt/mqtt_prj/build/CMakeFiles/mqttClient.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/mqttClient.dir/depend

