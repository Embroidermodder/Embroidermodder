cmake_minimum_required(VERSION 3.16)

project(embroidermodder2 VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets PrintSupport OpenGL OpenGLWidgets REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
add_compile_options(
	--coverage
	-g
	-O2
	-Wall
	-Wextra
	-fprofile-arcs
	-ftest-coverage
)

add_link_options(
	-fprofile-arcs
	-ftest-coverage
)
else()
add_compile_options(
	-g
	-O2
	-Wall
)
endif()

# The makers of CMake advise that you do not glob source files.
#
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#glob
#
# Since our ASSETS are essential for the program to run, we are going to
# give them the same treatment and list them explicitly here.
#

set(SRC_ASSETS ${CMAKE_SOURCE_DIR}/assets)

set(SRC ${CMAKE_SOURCE_DIR}/src)
set(EXTERN ${CMAKE_SOURCE_DIR}/extern)

set(C_DATA
	${SRC}/data/commands.c
	${SRC}/data/editors.c
	${SRC}/data/menus.c
	${SRC}/data/objects.c
	${SRC}/data/properties.c
	${SRC}/data/settings.c
	${SRC}/data/testing.c
	${SRC}/data/tips.c
	${SRC}/data/toolbars.c
)

qt_add_executable(embroidermodder2
	${EXTERN}/libembroidery/embroidery.c
	${EXTERN}/libembroidery/embroidery.h

	${EXTERN}/tomlc99/toml.c
	${EXTERN}/tomlc99/toml.h

	${SRC}/commands.cpp
	${SRC}/mainwindow.cpp
	${SRC}/object.cpp
	${SRC}/property-editor.cpp
	${SRC}/settings-dialog.cpp
	${SRC}/view.cpp
	${SRC}/widgets.cpp
	${SRC}/wrapper.cpp

	${SRC}/data/qt.cpp

	${C_DATA}

	${SRC}/script.c
	${SRC}/xpm.c

	${SRC}/embroidermodder.h
	${SRC}/core.h
)

add_executable(emb2_glfw
	${EXTERN}/libembroidery/embroidery.c
	${EXTERN}/libembroidery/embroidery.h

	${EXTERN}/tomlc99/toml.c
	${EXTERN}/tomlc99/toml.h

	${C_DATA}

	${SRC}/main_glfw.c
	${SRC}/script.c

	${SRC}/core.h
)

include_directories(
	${EXTERN}/libembroidery
	${EXTERN}/tomlc99
	${SRC}
)

target_link_libraries(embroidermodder2 PRIVATE
	Qt6::Core
	Qt6::OpenGL
	Qt6::Widgets
	Qt6::OpenGLWidgets
	Qt6::PrintSupport
	${OPENGL_LIBRARIES}
)

target_link_libraries(emb2_glfw PRIVATE
	glfw
	${OPENGL_LIBRARIES}
)

set_target_properties(embroidermodder2 PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)

if (WIN32)
else(WIN32)
target_link_libraries(embroidermodder2 PRIVATE m)
target_link_libraries(emb2_glfw PRIVATE m)
endif()

