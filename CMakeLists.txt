cmake_minimum_required(VERSION 3.16)

project(embroidermodder VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

add_compile_options(
	-g
	-pg
	-O2
	-Wall
	-Wextra
	--coverage
)

add_link_options(
	-pg
	--coverage
)

else()
add_compile_options(
	-g
	-O2
	-Wall
)
endif()

set(SRC ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(extern/libembroidery)
add_subdirectory(extern/lua)

# The makers of CMake advise that you do not glob source files.
#
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#glob
#
qt_add_executable(embroidermodder2
    ${SRC}/cmdprompt.cpp
    ${SRC}/data.cpp
    ${SRC}/embdetails-dialog.cpp
    ${SRC}/fill.cpp
    ${SRC}/generate.cpp
    ${SRC}/geometry.cpp
    ${SRC}/imagewidget.cpp
    ${SRC}/layer-manager.cpp
    ${SRC}/mainwindow.cpp
    ${SRC}/mdiarea.cpp
    ${SRC}/mdiwindow.cpp
    ${SRC}/object-arc.cpp
    ${SRC}/object-base.cpp
    ${SRC}/object-circle.cpp
    ${SRC}/object-dimleader.cpp
    ${SRC}/object-ellipse.cpp
    ${SRC}/object-image.cpp
    ${SRC}/object-line.cpp
    ${SRC}/object-path.cpp
    ${SRC}/object-point.cpp
    ${SRC}/object-polygon.cpp
    ${SRC}/object-polyline.cpp
    ${SRC}/object-rect.cpp
    ${SRC}/object-save.cpp
    ${SRC}/object-textsingle.cpp
    ${SRC}/preview-dialog.cpp
    ${SRC}/property-editor.cpp
    ${SRC}/script.cpp
    ${SRC}/selectbox.cpp
    ${SRC}/settings-dialog.cpp
    ${SRC}/statusbar.cpp
    ${SRC}/undo-commands.cpp
    ${SRC}/undo-editor.cpp
    ${SRC}/view.cpp

    ${SRC}/embroidermodder.h

    ${SRC}/core.c
    ${SRC}/core.h
)

# WARNING: This does NOT regenerate when the assets change, so you have to
# rerun cmake to ensure that this retriggers.
#
file(COPY "${CMAKE_SOURCE_DIR}/images"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/icons"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/scripts"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/samples"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/translations"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/docs"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/data"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/LICENSE.md"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

include_directories(
	${CMAKE_SOURCE_DIR}/embroidermodder2
	${CMAKE_SOURCE_DIR}/extern/libembroidery
	${CMAKE_SOURCE_DIR}/extern/lua/src
)

target_link_libraries(embroidermodder2 PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	embroidery_static
	lua
	${OPENGL_LIBRARIES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
target_link_libraries(embroidermodder2 PRIVATE
	gcov
)
endif()

set_target_properties(embroidermodder2 PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)

if (WIN32)
	set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder2.exe)
else(WIN32)
	target_link_libraries(embroidermodder2 PRIVATE m)
	set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder)
endif()
