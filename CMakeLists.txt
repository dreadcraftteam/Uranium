#
# CMAKELISTS.TXT FOR URANIUM ENGINE
#

cmake_minimum_required(VERSION 3.10)

project(Uranium, LANGUAGES C)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(launcher)
add_subdirectory(inputsystem)
add_subdirectory(md3system)

add_definitions(-w)
