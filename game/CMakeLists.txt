#
# CMAKELISTS.TXT FOR GAME PROJECT
#

project(game)

set(SOURCES
	defines.h 
	collision.c 
	collision.h 
	game.c 
	game.h 
	player.c 
	player.h 
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
		${CMAKE_SOURCE_DIR}/include/
		${CMAKE_SOURCE_DIR}/common/
		${CMAKE_SOURCE_DIR}/inputsystem/
		${CMAKE_SOURCE_DIR}/engine/audio/
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL GLEW::GLEW GLUT::GLUT glfw engine inputsystem)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)

