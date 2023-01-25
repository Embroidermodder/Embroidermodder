cmake_minimum_required(VERSION 3.20)

project(embroidermodder VERSION 2.0.0 LANGUAGES C CXX)

set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder_thumbnailer)
set(EXEC_DIR "/usr/bin")

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/glew/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/extern/glew/lib/Release/x64)
find_package(GLEW REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
set(CMAKE_PREFIX_PATH "C:/Qt/6.4.1/mingw_64")
endif(WIN32)

add_compile_options(
	-g
	-O2
	-Wall
)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets PrintSupport)
qt_standard_project_setup()

add_subdirectory(${CMAKE_SOURCE_DIR}/../extern/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/../extern/libembroidery)

# The makers of CMake advise that you do not glob source files.
#
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#glob
#
# Since our ASSETS are essential for the program to run, we are going to
# give them the same treatment and list them explicitly here.
#

set(SRC_ASSETS ${CMAKE_SOURCE_DIR}/assets)

add_executable(imgui_embroidermodder
	${CMAKE_SOURCE_DIR}/src/imgui_embroidermodder.cpp
	${CMAKE_SOURCE_DIR}/../extern/ImGuiColorTextEdit/TextEditor.cpp
	${CMAKE_SOURCE_DIR}/../extern/imgui/imgui.cpp
	${CMAKE_SOURCE_DIR}/../extern/imgui/imgui_demo.cpp
	${CMAKE_SOURCE_DIR}/../extern/imgui/imgui_draw.cpp
	${CMAKE_SOURCE_DIR}/../extern/imgui/imgui_tables.cpp
	${CMAKE_SOURCE_DIR}/../extern/imgui/imgui_widgets.cpp
	${CMAKE_SOURCE_DIR}/../extern/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/../extern/imgui/backends/imgui_impl_opengl2.cpp
)

add_executable(embroidermodder
	${CMAKE_SOURCE_DIR}/libembroidery-thumbnailer-kde4.cpp
)

include_directories(
	${CMAKE_SOURCE_DIR}/extern/glew/include
	${CMAKE_SOURCE_DIR}/extern/glfw/include
	${CMAKE_SOURCE_DIR}/extern/imgui
	${CMAKE_SOURCE_DIR}/extern/toml11
	${CMAKE_SOURCE_DIR}/extern/libembroidery/src
	${CMAKE_SOURCE_DIR}/extern/libembroidery/src/stb
	${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(embroidermodder_thumbnailer PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	embroidery_static
)

set_target_properties(embroidermodder_thumbnailer PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

target_link_libraries(embroidermodder PUBLIC m)

install(
	FILES ${EXEC}
	DESTINATION ${EXEC_DIR}
)

