cmake_minimum_required(VERSION 3.6)
project(HW1_8080CPU_Emulation)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp 8080emu.cpp gtuos.cpp)
add_executable(HW1_8080CPU_Emulation ${SOURCE_FILES})