#
# CMAKELISTS.TXT FOR GAME PROJECT
#

project(game)

set(SOURCES
    entities/brush.c
    entities/brush.h
    entities/light.c
    entities/light.h
    entities/pushable.c
    entities/pushable.h
    gameui/hud_armor.c
	gameui/hud_armor.h
	gameui/hud_crosshair.c
	gameui/hud_crosshair.h
	gameui/hud_health.c
	gameui/hud_health.h
	collision.c
	collision.h
	commands.c
	commands.h
	defines.h
	game.c
	game.h
	gameui_game.c
	gameui_game.h
	player.c
	player.h
    umap_game.c
    umap_game.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
		${CMAKE_SOURCE_DIR}/include/
		${CMAKE_SOURCE_DIR}/inputsystem/
		${CMAKE_SOURCE_DIR}/engine/audio/
		${CMAKE_SOURCE_DIR}/gameui/
        ${CMAKE_SOURCE_DIR}/common/
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL GLEW::GLEW GLUT::GLUT glfw engine inputsystem materialsystem gameui)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/../build/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../build/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)
