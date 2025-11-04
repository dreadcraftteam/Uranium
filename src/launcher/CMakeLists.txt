#
# CMAKELISTS.TXT FOR LAUNCHER PROJECT
#

project(launcher)

set(LAUNCHER_NAME "uranium")

set(SOURCES
	dynlib.c 
	dynlib.h 
	launcher.c 
	launcher.h 
)

add_executable(${LAUNCHER_NAME} ${SOURCES})

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/../build)
set_target_properties(${LAUNCHER_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../build)

set_target_properties(${LAUNCHER_NAME} PROPERTIES PREFIX "")

add_definitions(-w)
