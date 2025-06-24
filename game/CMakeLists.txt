# CMakeLists.txt for Game Project
project(game)

set(SOURCES
	main.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
