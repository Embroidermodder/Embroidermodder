# The Embroidermodder Project
#

cmake_minimum_required(VERSION 3.16)
project(embroidermodder VERSION 2.0.0 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)

include(config.cmake)

find_package(Qt6 REQUIRED COMPONENTS Core OpenGL Widgets PrintSupport)

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

qt_add_executable(embroidermodder2 WIN32 MACOSX_BUNDLE
    ${CMAKE_SOURCE_DIR}/src/cmdprompt.cpp
    ${CMAKE_SOURCE_DIR}/src/embdetails-dialog.cpp
    ${CMAKE_SOURCE_DIR}/src/embroidermodder.cpp
    ${CMAKE_SOURCE_DIR}/src/imagewidget.cpp
    ${CMAKE_SOURCE_DIR}/src/interface.cpp
    ${CMAKE_SOURCE_DIR}/src/layer-manager.cpp
    ${CMAKE_SOURCE_DIR}/src/mainwindow-menus.cpp
    ${CMAKE_SOURCE_DIR}/src/mainwindow-toolbars.cpp
    ${CMAKE_SOURCE_DIR}/src/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/src/mdiarea.cpp
    ${CMAKE_SOURCE_DIR}/src/mdiwindow.cpp
    ${CMAKE_SOURCE_DIR}/src/objects.cpp
    ${CMAKE_SOURCE_DIR}/src/preview-dialog.cpp
    ${CMAKE_SOURCE_DIR}/src/property-editor.cpp
    ${CMAKE_SOURCE_DIR}/src/selectbox.cpp
    ${CMAKE_SOURCE_DIR}/src/settings-dialog.cpp
    ${CMAKE_SOURCE_DIR}/src/statusbar.cpp
    ${CMAKE_SOURCE_DIR}/src/undo-commands.cpp
    ${CMAKE_SOURCE_DIR}/src/undo-editor.cpp
    ${CMAKE_SOURCE_DIR}/src/view.cpp

    ${CMAKE_SOURCE_DIR}/src/embroidermodder.h
    ${CMAKE_SOURCE_DIR}/src/data.h

    ${CMAKE_SOURCE_DIR}/extern/tomlc99/toml.c

    ${CMAKE_SOURCE_DIR}/assets/assets.qrc
)

include_directories(
    ${CMAKE_SOURCE_DIR}/extern/tomlc99
    ${CMAKE_SOURCE_DIR}/extern/libembroidery/src
    ${CMAKE_SOURCE_DIR}/extern/libembroidery/src/stb
    ${CMAKE_SOURCE_DIR}/extern/libembroidery/src/nanosvg
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(embroidermodder2 PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::OpenGL
    embroidery_static)

if (WIN32)
set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder2.exe)

else(WIN32)
target_link_libraries(embroidermodder2 PRIVATE m)
set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder2)

endif()

install(TARGETS embroidermodder2
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/build
)
