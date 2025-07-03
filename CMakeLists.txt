# CMakeLists.txt for Uranium Engine

cmake_minimum_required(VERSION 3.10)

project(Uranium, LANGUAGES C)

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(launcher)

add_definitions(-w)
