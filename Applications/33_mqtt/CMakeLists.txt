cmake_minimum_required(VERSION 2.8.12)
project(MQTT_CLIENT C)	# 项目的名称和支持的编程语言
message(STATUS "CMake version: " ${CMAKE_VERSION})
message(STATUS "CMake system name: " ${CMAKE_SYSTEM_NAME})
message(STATUS "CMake system processor: " ${CMAKE_SYSTEM_PROCESSOR})

# 设置可执行文件输出路径
# set(executable_output_path ~/ZAT/obj) cmake参数区分大小写
set(EXECUTABLE_OUTPUT_PATH  ~/ZAT/obj)
# 定义可执行文件目标
add_executable(33_mqtt_client ${PROJECT_SOURCE_DIR}/src/33_mqtt_client.c)

# 指定MQTT客户端库头文件路径、库路径以及链接库
target_include_directories(33_mqtt_client PRIVATE ${PROJECT_SOURCE_DIR}/inc)	# 头文件路径
target_link_directories(33_mqtt_client PRIVATE ${PROJECT_SOURCE_DIR}/lib)		# 库文件路径
target_link_libraries(33_mqtt_client PRIVATE paho-mqtt3c)	# 指定库文件