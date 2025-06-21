# CMakeLists for Uranium Engine
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(Uranium)

set(SOURCES 
    src/main.c
    src/engine/console.c
    src/engine/console.h
    src/engine/variables.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
include_directories(include/)

link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/SDL3.lib
        ${CMAKE_SOURCE_DIR}/lib/SDL3_ttf.lib
        ${CMAKE_SOURCE_DIR}/lib/glew32.lib
        ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
        ${CMAKE_SOURCE_DIR}/lib/cglm.lib
        opengl32
)

set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/dist/)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${INSTALL_DIR}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --config $<CONFIG>
)