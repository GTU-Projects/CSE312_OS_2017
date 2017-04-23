cmake_minimum_required(VERSION 3.6)
project(HW2_MultiProcessOS)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp)
file(GLOB SOURCE_FILES "./*.cpp"  "./*.h")


add_executable(HW2_MultiProcessOS ${SOURCE_FILES})