cmake_minimum_required(VERSION 3.10)

project(akinator)

set(CMAKE_CXX_COMPILER C:/msys64/mingw64/bin)
set(CMAKE_C_COMPILER C:/msys64/mingw64/bin)

set(CMAKE_CXX_STANDARD 11)

add_executable(akinator akinator.cpp)
add_executable(test test.cpp)