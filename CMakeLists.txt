# CMakeLists for Uranium
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(Uranium)

set(SOURCES 
    src/main.c
    src/engine/variables.h
    src/video/video_main.c
    src/video/video_main.h
)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
include_directories(include/)

link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/SDL3.lib
        ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
)

target_link_options(Uranium PRIVATE "/ENTRY:mainCRTStartup")

set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/dist/)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${INSTALL_DIR}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --config $<CONFIG>
)

if(MSVC)
    target_link_options(Uranium PRIVATE
        $<$<CONFIG:Release>:/SAFESEH:NO>
        $<$<CONFIG:Debug>:/SAFESEH:NO>
    )
endif()