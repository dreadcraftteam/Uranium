#
# CMAKELISTS.TXT FOR MATERIALSYSTEM PROJECT
#

project(materialsystem)

set(SOURCES
    commands.c 
    commands.h 
	materialsystem.c 
	materialsystem.h 
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
		${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/common/
)

target_link_directories(${PROJECT_NAME} PRIVATE ${PROJECT_LIB_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL GLEW::GLEW GLUT::GLUT glfw)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/../build/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../build/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)
