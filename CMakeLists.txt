cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(aaarray)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("dependencies")
add_subdirectory("src")

include_directories(${SDL2_INCLUDE_DIR})
target_include_directories(aaarray PRIVATE ${SDL2_INCLUDE_DIR})
target_link_libraries(aaarray SDL2)
