# CMakeLists.txt for launcher project
project(launcher)

set(SOURCES
	launcher.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)
