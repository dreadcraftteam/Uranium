# CMakeLists.txt for Game Project
project(game)

set(SOURCES
	game.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/dist/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)
