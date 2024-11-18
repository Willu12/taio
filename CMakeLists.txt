cmake_minimum_required (VERSION 3.9)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("TAiO")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

add_executable(app "src/main.cpp")

set_target_properties(app PROPERTIES CXX_STANDARD 23)
target_include_directories(app PUBLIC include)

