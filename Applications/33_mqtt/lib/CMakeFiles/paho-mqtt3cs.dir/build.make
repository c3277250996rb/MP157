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
include src/CMakeFiles/paho-mqtt3cs.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/paho-mqtt3cs.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/paho-mqtt3cs.dir/flags.make

src/CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.o: src/CMakeFiles/paho-mqtt3cs.dir/flags.make
src/CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.o: ../src/MQTTClient.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/tools/paho.mqtt.c-1.3.8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.o"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && /opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.o   -c /home/alientek/tools/paho.mqtt.c-1.3.8/src/MQTTClient.c

src/CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.i"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && /opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alientek/tools/paho.mqtt.c-1.3.8/src/MQTTClient.c > CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.i

src/CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.s"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && /opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alientek/tools/paho.mqtt.c-1.3.8/src/MQTTClient.c -o CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.s

src/CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.o: src/CMakeFiles/paho-mqtt3cs.dir/flags.make
src/CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.o: ../src/SSLSocket.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/tools/paho.mqtt.c-1.3.8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.o"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && /opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.o   -c /home/alientek/tools/paho.mqtt.c-1.3.8/src/SSLSocket.c

src/CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.i"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && /opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alientek/tools/paho.mqtt.c-1.3.8/src/SSLSocket.c > CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.i

src/CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.s"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && /opt/st/stm32mp1/3.1-snapshot/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc --sysroot=/opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alientek/tools/paho.mqtt.c-1.3.8/src/SSLSocket.c -o CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.s

# Object files for target paho-mqtt3cs
paho__mqtt3cs_OBJECTS = \
"CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.o" \
"CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.o"

# External object files for target paho-mqtt3cs
paho__mqtt3cs_EXTERNAL_OBJECTS = \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/MQTTTime.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/MQTTProtocolClient.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/Clients.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/utf-8.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/MQTTPacket.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/MQTTPacketOut.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/Messages.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/Tree.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/Socket.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/Log.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/MQTTPersistence.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/Thread.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/MQTTProtocolOut.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/MQTTPersistenceDefault.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/SocketBuffer.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/LinkedList.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/MQTTProperties.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/MQTTReasonCodes.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/Base64.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/SHA1.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/WebSocket.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/StackTrace.c.o" \
"/home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/common_ssl_obj.dir/Heap.c.o"

src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/paho-mqtt3cs.dir/MQTTClient.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/paho-mqtt3cs.dir/SSLSocket.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/MQTTTime.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/MQTTProtocolClient.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/Clients.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/utf-8.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/MQTTPacket.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/MQTTPacketOut.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/Messages.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/Tree.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/Socket.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/Log.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/MQTTPersistence.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/Thread.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/MQTTProtocolOut.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/MQTTPersistenceDefault.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/SocketBuffer.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/LinkedList.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/MQTTProperties.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/MQTTReasonCodes.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/Base64.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/SHA1.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/WebSocket.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/StackTrace.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/common_ssl_obj.dir/Heap.c.o
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/paho-mqtt3cs.dir/build.make
src/libpaho-mqtt3cs.so.1.3.8: /opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi/usr/lib/libssl.so
src/libpaho-mqtt3cs.so.1.3.8: /opt/st/stm32mp1/3.1-snapshot/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi/usr/lib/libcrypto.so
src/libpaho-mqtt3cs.so.1.3.8: src/CMakeFiles/paho-mqtt3cs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alientek/tools/paho.mqtt.c-1.3.8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C shared library libpaho-mqtt3cs.so"
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/paho-mqtt3cs.dir/link.txt --verbose=$(VERBOSE)
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && $(CMAKE_COMMAND) -E cmake_symlink_library libpaho-mqtt3cs.so.1.3.8 libpaho-mqtt3cs.so.1 libpaho-mqtt3cs.so

src/libpaho-mqtt3cs.so.1: src/libpaho-mqtt3cs.so.1.3.8
	@$(CMAKE_COMMAND) -E touch_nocreate src/libpaho-mqtt3cs.so.1

src/libpaho-mqtt3cs.so: src/libpaho-mqtt3cs.so.1.3.8
	@$(CMAKE_COMMAND) -E touch_nocreate src/libpaho-mqtt3cs.so

# Rule to build all files generated by this target.
src/CMakeFiles/paho-mqtt3cs.dir/build: src/libpaho-mqtt3cs.so

.PHONY : src/CMakeFiles/paho-mqtt3cs.dir/build

src/CMakeFiles/paho-mqtt3cs.dir/clean:
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build/src && $(CMAKE_COMMAND) -P CMakeFiles/paho-mqtt3cs.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/paho-mqtt3cs.dir/clean

src/CMakeFiles/paho-mqtt3cs.dir/depend:
	cd /home/alientek/tools/paho.mqtt.c-1.3.8/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alientek/tools/paho.mqtt.c-1.3.8 /home/alientek/tools/paho.mqtt.c-1.3.8/src /home/alientek/tools/paho.mqtt.c-1.3.8/build /home/alientek/tools/paho.mqtt.c-1.3.8/build/src /home/alientek/tools/paho.mqtt.c-1.3.8/build/src/CMakeFiles/paho-mqtt3cs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/paho-mqtt3cs.dir/depend

