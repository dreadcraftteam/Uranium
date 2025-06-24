# CMakeLists.txt for Engine Project
project(engine)

set(SOURCES
	main.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
