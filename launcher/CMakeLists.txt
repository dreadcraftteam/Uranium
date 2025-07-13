# For launcher project
project(launcher)

set(LAUNCHER_NAME "launcher")

set(SOURCES
	launcher.c
)

add_executable(${LAUNCHER_NAME} ${SOURCES})

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)
