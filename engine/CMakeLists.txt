# CMakeLists.txt for Engine Project
project(engine)

set(SOURCES
	engine.c
	engine_variables.h
)

set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SDL2_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")

if(NOT EXISTS "${SDL2_INCLUDE_DIR}/SDL2/SDL.h")
    message(FATAL_ERROR "SDL2 includes not found in ${SDL2_INCLUDE_DIR}")
endif()

if(NOT EXISTS "${SDL2_LIB_DIR}/libSDL2.so")
    message(FATAL_ERROR "SDL2 libraries not found in ${SDL2_LIB_DIR}")
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})

target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE 
			              ${CMAKE_SOURCE_DIR}/lib/libSDL2.so
)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/dist/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
