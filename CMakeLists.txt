cmake_minimum_required(VERSION 3.8)
project(Netword)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES week4/echo_server.c)
add_executable(Netword ${SOURCE_FILES})
