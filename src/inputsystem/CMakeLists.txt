#
# CMAKELISTS.TXT FOR INPUTSYSTEM PROJECT
#

project(inputsystem)

set(SOURCES
	inputsystem.c 
	inputsystem.h 
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
		${CMAKE_SOURCE_DIR}/include/
		${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_SOURCE_DIR}/common/
)

target_link_directories(${PROJECT_NAME} PRIVATE ${PROJECT_LIB_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/../build/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../build/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)

