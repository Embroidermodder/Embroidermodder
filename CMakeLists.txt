cmake_minimum_required(VERSION 3.16)

project(embroidermodder VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/glew/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/extern/glew/lib/Release/x64)
find_package(GLEW REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

add_compile_options(
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

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/extern/libembroidery)
add_subdirectory(${CMAKE_SOURCE_DIR}/extern/imgui)

# The makers of CMake advise that you do not glob source files.
#
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#glob
#
# Since our ASSETS are essential for the program to run, we are going to
# give them the same treatment and list them explicitly here.
#

set(SRC_ASSETS ${CMAKE_SOURCE_DIR}/assets)

add_executable(embroidermodder
	${CMAKE_SOURCE_DIR}/src/gui_backend.cpp
	${CMAKE_SOURCE_DIR}/src/main.c

	${CMAKE_SOURCE_DIR}/extern/tomlc99/toml.c
)

# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
# target_compile_options(embroidermodder
# 	PRIVATE
# 	-Werror
# )
# endif()

include_directories(
	${CMAKE_SOURCE_DIR}/extern/glew/include
	${CMAKE_SOURCE_DIR}/extern/glfw/include
	${CMAKE_SOURCE_DIR}/extern/imgui
	${CMAKE_SOURCE_DIR}/extern/tinydir
	${CMAKE_SOURCE_DIR}/extern/libembroidery/src
	${CMAKE_SOURCE_DIR}/extern/libembroidery/src/stb
	${CMAKE_SOURCE_DIR}/extern/tomlc99
	${CMAKE_SOURCE_DIR}/src
)

set_target_properties(embroidermodder PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

target_link_libraries(embroidermodder PRIVATE
	glfw
	embroidery_static
	imgui_static
	${OPENGL_LIBRARIES})

if (WIN32)
set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder.exe)

else(WIN32)
target_link_libraries(embroidermodder PRIVATE m)
set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder)

endif()

set(EXEC_DIR ".")
set(ASSETS_DIR ".")

install(
	FILES ${EXEC}
	DESTINATION ${EXEC_DIR}
)

install(
	FILES
		${SRC_ASSETS}/em2_config.toml
	DESTINATION
		${ASSETS_DIR}
)

install(
	FILES
		${SRC_ASSETS}/translations/arabic.toml
		${SRC_ASSETS}/translations/chinese_simplified.toml
		${SRC_ASSETS}/translations/chinese_traditional.toml
		${SRC_ASSETS}/translations/french.toml
		${SRC_ASSETS}/translations/hindi.toml
		${SRC_ASSETS}/translations/russian.toml
		${SRC_ASSETS}/translations/spanish.toml
	DESTINATION
		${ASSETS_DIR}/translations
)

# The necessary licenses are included in installation, should the user
# not retain the code after installation and so this more resembles
# the NSIS style installation.
install(
	FILES
		${SRC_ASSETS}/fonts/source-code-pro-license.md
		${SRC_ASSETS}/fonts/source-code-pro-readme.md
		${SRC_ASSETS}/fonts/SourceCodePro-Regular.ttf
		${SRC_ASSETS}/fonts/source-sans-license.md
		${SRC_ASSETS}/fonts/source-sans-readme.md
		${SRC_ASSETS}/fonts/SourceSans3-Regular.ttf
	DESTINATION
		${ASSETS_DIR}/fonts
)
