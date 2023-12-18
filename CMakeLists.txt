# The Embroidermodder Project
#

cmake_minimum_required(VERSION 3.16)
project(embroidermodder VERSION 2.0.0 LANGUAGES C CXX)

#include(config.cmake)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport OpenGLWidgets)

qt_standard_project_setup()

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
    -g
    -O2
    -Wall
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    add_compile_options(
        -Wextra
        -fprofile-arcs
        -ftest-coverage
    )

    add_link_options(
        -fprofile-arcs
        -ftest-coverage
    )

endif()

add_subdirectory(extern/libembroidery)
add_subdirectory(extern/SDL2-2.28.5)
add_subdirectory(extern/freetype-2.13.2)

qt_add_executable(embroidermodder2 WIN32 MACOSX_BUNDLE
    ${CMAKE_SOURCE_DIR}/src/interface.cpp
    ${CMAKE_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/src/objects.cpp
    ${CMAKE_SOURCE_DIR}/src/settings-dialog.cpp
    ${CMAKE_SOURCE_DIR}/src/view.cpp

    ${CMAKE_SOURCE_DIR}/src/embroidermodder.h
    ${CMAKE_SOURCE_DIR}/src/core.h
    ${CMAKE_SOURCE_DIR}/src/geometry.h

    ${CMAKE_SOURCE_DIR}/src/core.c
    ${CMAKE_SOURCE_DIR}/src/widgets.c
    ${CMAKE_SOURCE_DIR}/src/object_core.c

    ${CMAKE_SOURCE_DIR}/assets/assets.qrc
)

include_directories(
    ${CMAKE_SOURCE_DIR}/extern/libembroidery/src
    ${CMAKE_SOURCE_DIR}/extern/libembroidery/src/stb
    ${CMAKE_SOURCE_DIR}/extern/libembroidery/src/nanosvg
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(embroidermodder2 PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::OpenGLWidgets
    SDL2-static
    freetype
    embroidery_static)

if (WIN32)
set_target_properties(embroidermodder2
    WIN32_EXECUTIBLE ON
    MACOS BUNDLE ON)
else(WIN32)
target_link_libraries(embroidermodder2 PRIVATE m)
endif()

install(TARGETS embroidermodder2
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/build
)
