# CMakeLists.txt for Engine Project
project(engine)

set(SOURCES
	engine.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
