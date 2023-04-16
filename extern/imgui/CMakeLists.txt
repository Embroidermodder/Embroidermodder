# So embroidermodder doesn't have to treat warnings about ImGui as errors to be fixed
# this is compiled seperately as a static library.

cmake_minimum_required(VERSION 3.5)

project(ImGui VERSION 1.0.0 LANGUAGES CXX)

find_package(OpenGL REQUIRED)

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/../glew/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/../glew/lib/Release/x64)
find_package(GLEW REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CXX_SOURCES
    imgui.h
    imgui.cpp
    imgui_demo.cpp
	imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
	backends/imgui_impl_glfw.cpp
	backends/imgui_impl_opengl2.cpp
)

add_compile_options(
	-g
	-O2
	-fPIC
	-Wall
	-Wextra
)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../glfw/include
)

add_library(imgui_static STATIC ${CXX_SOURCES})
