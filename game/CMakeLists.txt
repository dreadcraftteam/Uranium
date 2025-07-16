# For game project

project(game)

set(SOURCES
		examples/func_cube.c
		examples/func_cube.h
		examples/func_pyramid.c
		examples/func_pyramid.h
		func_ground.c
		func_ground.h
		game.c
)

set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(COMMON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common")

set(PROJECT_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIR} ${COMMON_INCLUDE_DIR})

target_link_directories(${PROJECT_NAME} PRIVATE ${PROJECT_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib/libGLEW.so ${CMAKE_SOURCE_DIR}/lib/libglut.so)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_definitions(-w)
