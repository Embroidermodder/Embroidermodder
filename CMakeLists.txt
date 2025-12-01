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

# The makers of CMake advise that you do not glob source files.
#
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#glob
#
qt_add_executable(embroidermodder2
    ${SRC}/commands/about.c
    ${SRC}/commands/arc.c
    ${SRC}/commands/changelog.c
    ${SRC}/commands/circle.c
    ${SRC}/commands/clear.c
    ${SRC}/commands/clear_rubber.c
    ${SRC}/commands/copy.c
    ${SRC}/commands/cut.c
    ${SRC}/commands/day.c
    ${SRC}/commands/delete.c
    ${SRC}/commands/details.c
    ${SRC}/commands/dim_leader.c
    ${SRC}/commands/distance.c
    ${SRC}/commands/ellipse.c
    ${SRC}/commands/end.c
    ${SRC}/commands/exit.c
    ${SRC}/commands/fill.c
    ${SRC}/commands/generate.c
    ${SRC}/commands/get.c
    ${SRC}/commands/help.c
    ${SRC}/commands/horizontal_dim.c
    ${SRC}/commands/icon_resize.c
    ${SRC}/commands/image.c
    ${SRC}/commands/infinite_line.c
    ${SRC}/commands/line.c
    ${SRC}/commands/macro.c
    ${SRC}/commands/move.c
    ${SRC}/commands/new.c
    ${SRC}/commands/night.c
    ${SRC}/commands/open.c
    ${SRC}/commands/pan.c
    ${SRC}/commands/paste.c
    ${SRC}/commands/path.c
    ${SRC}/commands/platform.c
    ${SRC}/commands/play.c
    ${SRC}/commands/point.c
    ${SRC}/commands/polygon.c
    ${SRC}/commands/polyline.c
    ${SRC}/commands/print.c
    ${SRC}/commands/ray.c
    ${SRC}/commands/rectangle.c
    ${SRC}/commands/redo.c
    ${SRC}/commands/regular_polygon.c
    ${SRC}/commands/repeat.c
    ${SRC}/commands/rgb.c
    ${SRC}/commands/rotate.c
    ${SRC}/commands/rounded_rectangle.c
    ${SRC}/commands/save_as.c
    ${SRC}/commands/save.c
    ${SRC}/commands/scale.c
    ${SRC}/commands/script.c
    ${SRC}/commands/select_all.c
    ${SRC}/commands/set.c
    ${SRC}/commands/settings.c
    ${SRC}/commands/sleep.c
    ${SRC}/commands/slot.c
    ${SRC}/commands/stop.c
    ${SRC}/commands/stub.c
    ${SRC}/commands/text_multi.c
    ${SRC}/commands/text_single.c
    ${SRC}/commands/triangle.c
    ${SRC}/commands/undo.c
    ${SRC}/commands/updates.c
    ${SRC}/commands/vertical_dim.c
    ${SRC}/commands/whats_this.c
    ${SRC}/commands/window.c
    ${SRC}/commands/zoom.c
    ${SRC}/commands.h

    ${SRC}/designs/dolphin.c
    ${SRC}/designs/heart.c
    ${SRC}/designs/snowflake.c
    ${SRC}/designs/star.c
    ${SRC}/designs.h

    ${SRC}/fills/brick.c
    ${SRC}/fills/gradient.c
    ${SRC}/fills/satin.c
    ${SRC}/fills.h

    ${SRC}/generators/drawing.c
    ${SRC}/generators/guilloche.c
    ${SRC}/generators/knot.c
    ${SRC}/generators/photo.c
    ${SRC}/generators/qr.c
    ${SRC}/generators.h

    ${SRC}/core.c
    ${SRC}/debug.c
    ${SRC}/core.h

    ${SRC}/commands.cpp
    ${SRC}/cmdprompt.cpp
    ${SRC}/data.cpp
    ${SRC}/dialogs.cpp
    ${SRC}/embdetails-dialog.cpp
    ${SRC}/geometry.cpp
    ${SRC}/imagewidget.cpp
    ${SRC}/layer-manager.cpp
    ${SRC}/mainwindow.cpp
    ${SRC}/mdiarea.cpp
    ${SRC}/mdiwindow.cpp
    ${SRC}/preview-dialog.cpp
    ${SRC}/property-editor.cpp
    ${SRC}/rubber.cpp
    ${SRC}/save.cpp
    ${SRC}/selectbox.cpp
    ${SRC}/settings-dialog.cpp
    ${SRC}/statusbar.cpp
    ${SRC}/undo-commands.cpp
    ${SRC}/undo-editor.cpp
    ${SRC}/view.cpp

    ${SRC}/objects/arc.cpp
    ${SRC}/objects/base.cpp
    ${SRC}/objects/circle.cpp
    ${SRC}/objects/dimleader.cpp
    ${SRC}/objects/ellipse.cpp
    ${SRC}/objects/image.cpp
    ${SRC}/objects/line.cpp
    ${SRC}/objects/path.cpp
    ${SRC}/objects/point.cpp
    ${SRC}/objects/polygon.cpp
    ${SRC}/objects/polyline.cpp
    ${SRC}/objects/rectangle.cpp
    ${SRC}/objects/textsingle.cpp

    ${SRC}/embroidermodder.h
)

# WARNING: This does NOT regenerate when the assets change, so you have to
# rerun cmake to ensure that this retriggers.
#
file(COPY "${CMAKE_SOURCE_DIR}/images"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/icons"
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
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/extern/libembroidery
)

target_link_libraries(embroidermodder2 PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	embroidery_static
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
