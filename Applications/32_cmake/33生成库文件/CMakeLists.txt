cmake_minimum_required(VERSION 3.5)
project(HELLO)

add_library(libhello hello.c)
set_target_properties(libhello PROPERTIES OUTPUT_NAME "hello")
set(SOURCE main.c)
add_executable(hello ${SOURCE})
target_link_libraries(hello libhello)
