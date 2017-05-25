cmake_minimum_required(VERSION 3.6)
project(HW3_MMU)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp)
file(GLOB SOURCE_FILES "./*.cpp"  "./*.h")


add_executable(HW3_MMU ${SOURCE_FILES})