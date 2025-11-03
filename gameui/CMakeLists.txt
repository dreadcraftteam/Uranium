#
# CMAKELISTS.TXT FOR GAMEUI PROJECT
#

project(gameui)

set(SOURCES
    button.c 
    button.h 
    gameui.c 
	gameui.h 
    gradientpanel.c 
    gradientpanel.h 
    imagepanel.c 
    imagepanel.h 
	label.c 
	label.h 
	panel.c 
	panel.h 
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
		${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/engine/audio/
        ${CMAKE_SOURCE_DIR}/common/
)

target_link_directories(${PROJECT_NAME} PRIVATE ${PROJECT_LIB_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL GLEW::GLEW GLUT::GLUT materialsystem)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)
