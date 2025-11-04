#
# CMAKELISTS.TXT FOR ENGINE PROJECT
#

project(engine)

set(SOURCES
	audio/audio.c
	audio/audio.h
	audio/miniaudio.h
    build_number.c
    build_number.h
    commands.c
	commands.h
    console.c
    console.h
    debugpanel.c
    debugpanel.h
    defines.h
	dynlib.c
	dynlib.h
	engine.c
	engine.h
	gameui_engine.c
	gameui_engine.h
	msg.c
	msg.h
	umap.c
	umap.h
	variables.h
)

if(EXISTS "${CMAKE_SOURCE_DIR}/engine/build_number.txt")
    file(READ "${CMAKE_SOURCE_DIR}/engine/build_number.txt" BUILD_NUMBER)
    math(EXPR BUILD_NUMBER "${BUILD_NUMBER} + 1")
else()
    set(BUILD_NUMBER "1")
endif()

file(WRITE "${CMAKE_SOURCE_DIR}/engine/build_number.txt" "${BUILD_NUMBER}")

configure_file("${CMAKE_SOURCE_DIR}/engine/build_number.h.in" "${CMAKE_SOURCE_DIR}/engine/build_number.h")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/include/ 
        ${CMAKE_SOURCE_DIR}/engine/audio/ 
        ${CMAKE_SOURCE_DIR}/gameui/
        ${CMAKE_SOURCE_DIR}/common/
)

target_link_directories(${PROJECT_NAME} PRIVATE ${PROJECT_LIB_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL GLEW::GLEW GLUT::GLUT glfw inputsystem materialsystem gameui)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/../build/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../build/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)
