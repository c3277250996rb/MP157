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
CMAKE_SOURCE_DIR = /home/alientek/tools/paho.mqtt.c-1.3.8

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alientek/tools/paho.mqtt.c-1.3.8/build

# Include any dependencies generated for this target.
include src/CMakeFiles/Sha1Test.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/Sha1Test.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/Sha1Test.dir/flags.make

src/CMakeFiles/Sha1Test.dir/SHA1.c.o: src/CMakeFiles/Sha1Test.dir/flags.make
src/CMakeFiles/Sha1Test.dir/SHA1.c.o: ../src/SHA1.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/tools/paho.mqtt.c-1.3.8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/Sha1Test.dir/SHA1.c.o"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && /opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Sha1Test.dir/SHA1.c.o   -c /home/alientek/tools/paho.mqtt.c-1.3.8/src/SHA1.c

src/CMakeFiles/Sha1Test.dir/SHA1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Sha1Test.dir/SHA1.c.i"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && /opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alientek/tools/paho.mqtt.c-1.3.8/src/SHA1.c > CMakeFiles/Sha1Test.dir/SHA1.c.i

src/CMakeFiles/Sha1Test.dir/SHA1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Sha1Test.dir/SHA1.c.s"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && /opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alientek/tools/paho.mqtt.c-1.3.8/src/SHA1.c -o CMakeFiles/Sha1Test.dir/SHA1.c.s

# Object files for target Sha1Test
Sha1Test_OBJECTS = \
"CMakeFiles/Sha1Test.dir/SHA1.c.o"

# External object files for target Sha1Test
Sha1Test_EXTERNAL_OBJECTS =

src/Sha1Test: src/CMakeFiles/Sha1Test.dir/SHA1.c.o
src/Sha1Test: src/CMakeFiles/Sha1Test.dir/build.make
src/Sha1Test: src/CMakeFiles/Sha1Test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alientek/tools/paho.mqtt.c-1.3.8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Sha1Test"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Sha1Test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/Sha1Test.dir/build: src/Sha1Test

.PHONY : src/CMakeFiles/Sha1Test.dir/build

src/CMakeFiles/Sha1Test.dir/clean:
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && $(CMAKE_COMMAND) -P CMakeFiles/Sha1Test.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/Sha1Test.dir/clean

src/CMakeFiles/Sha1Test.dir/depend:
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alientek/tools/paho.mqtt.c-1.3.8 /home/alientek/tools/paho.mqtt.c-1.3.8/src /home/alientek/tools/paho.mqtt.c-1.3.8/build /home/alientek/tools/paho.mqtt.c-1.3.8/build/src /home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/Sha1Test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/Sha1Test.dir/depend

