cmake_minimum_required(VERSION 3.16)

project(embroidermodder2 VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets PrintSupport Svg REQUIRED)

if(WIN32)
else(WIN32)
find_package(X11 REQUIRED)
endif(WIN32)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

set(EMBROIDERY_BUILD_SHARED_LIBS OFF)
set(EMBROIDERY_BUILD_CLI OFF)

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
set(TEST ${CMAKE_SOURCE_DIR}/test)
set(EXTERN ${CMAKE_SOURCE_DIR}/extern)
set(LIBEMB ${CMAKE_SOURCE_DIR}/extern/libembroidery/src)

add_subdirectory(extern/glfw)
add_subdirectory(extern/libembroidery)

qt_add_executable(embroidermodder2
	${EXTERN}/nanovg/src/nanovg.c
	${EXTERN}/nanovg/src/nanovg.h

	${EXTERN}/glad/src/glad.c
	${EXTERN}/glad/include/glad/glad.h
	${EXTERN}/glad/include/KHR/khrplatform.h

	${SRC}/commands.cpp
	${SRC}/data.cpp
	${SRC}/interface.cpp
	${SRC}/objects.cpp
	${SRC}/state.cpp
	${SRC}/widgets.cpp

	${SRC}/core.h
	${SRC}/qt.h
)

# WARNING: This does NOT regenerate when the assets change, so you have to
# make a new build folder to ensure that this retriggers.
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

include_directories(
	${EXTERN}/libembroidery/src
	${EXTERN}/glfw/include
	${EXTERN}/glad/include
	${EXTERN}/nanovg/src
	${EXTERN}/nanosvg/src
	${EXTERN}/tomlc99
	${OPENGL_INCLUDE_DIRS}
	${SRC}
)

target_link_libraries(embroidermodder2 PRIVATE
	glfw
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	Qt6::Svg
	embroidery_static
	${OPENGL_LIBRARIES}
)

set_target_properties(embroidermodder2 PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)

if (WIN32)
else(WIN32)
target_link_libraries(embroidermodder2 PRIVATE
	${X11_LIBRARIES}
	m
)
endif()
