# CMakeLists.txt for game Project

project(game)

set(SOURCES
		model/cube.c
		model/cube.h
		game.c
)

set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SDL2_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})

target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE 
				      		${CMAKE_SOURCE_DIR}/lib/libGLEW.so
							${CMAKE_SOURCE_DIR}/lib/libglut.so
)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/dist/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)