cmake_minimum_required (VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("TAiO")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("app")
add_subdirectory("libs")
