cmake_minimum_required(VERSION 3.16)

project(embroidermodder2 VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
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

set(SRC ${CMAKE_SOURCE_DIR}/src)
set(EXTERN ${CMAKE_SOURCE_DIR}/extern)

set(C_CORE
	${EXTERN}/libembroidery/embroidery.c
	${EXTERN}/libembroidery/embroidery.h

	${EXTERN}/tomlc99/toml.c
	${EXTERN}/tomlc99/toml.h

	${SRC}/data/commands.c
	${SRC}/data/editors.c
	${SRC}/data/menus.c
	${SRC}/data/objects.c
	${SRC}/data/properties.c
	${SRC}/data/selectors.c
	${SRC}/data/settings.c
	${SRC}/data/testing.c
	${SRC}/data/tips.c
	${SRC}/data/toolbars.c
	${SRC}/data/paths.c
	${SRC}/data/constants.h

	${SRC}/script.c
	${SRC}/objects.c
	${SRC}/commands.c
	${SRC}/geometry.c
	${SRC}/document.c
	${SRC}/properties.c
	${SRC}/xpm.c

	${SRC}/core.h
)

qt_add_executable(embroidermodder2
	${SRC}/widgets.cpp

	${C_CORE}
)

add_executable(emb2_glfw
	${C_CORE}

	${EXTERN}/nanovg/src/nanovg.c
	${EXTERN}/nanovg/src/nanovg.h

	${SRC}/main_glfw.c
)

include_directories(
	${EXTERN}/libembroidery
	${EXTERN}/tomlc99
	${EXTERN}/glfw/include
	${EXTERN}/nanovg/src
#	${GLEW_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	${SRC}
)

target_link_libraries(embroidermodder2 PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
)

target_link_libraries(emb2_glfw PRIVATE
	glfw
	${GLEW_LIBRARIES}
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

