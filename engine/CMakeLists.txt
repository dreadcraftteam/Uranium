#
# CMAKELISTS.TXT FOR ENGINE PROJECT
#

project(engine)

set(SOURCES
	audio/audio.c 
	audio/audio.h
	audio/miniaudio.h
	defines.h
	dynlib.c 
	dynlib.h 
	engine.c 
	engine.h 
	gameui/pause.c 
	gameui/pause.h 
	umap.c 
	umap.h 
	stb_image.h
	variables.h 
	msg.c 
	msg.h 
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
		${CMAKE_SOURCE_DIR}/include/
		${CMAKE_SOURCE_DIR}/common/
		${CMAKE_CURRENT_SOURCE_DIR}/inputsystem/
		${CMAKE_CURRENT_SOURCE_DIR}/audio/
)

target_link_directories(${PROJECT_NAME} PRIVATE ${PROJECT_LIB_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL GLEW::GLEW GLUT::GLUT glfw inputsystem)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)
