cmake_minimum_required(VERSION 2.6)
project(FREESIA)

file(GLOB SOURCES "./*.c")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -pthread")
add_executable(freesia ${SOURCES})

