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
set(LIBEMB ${CMAKE_SOURCE_DIR}/extern/libembroidery/src)

add_subdirectory(extern/glfw)

set(LIBRARY_SRC
	${LIBEMB}/pattern.c
	${LIBEMB}/compress.c
	${LIBEMB}/formats.c
	${LIBEMB}/geometry.c
	${LIBEMB}/script.c
	${LIBEMB}/testing.c
	${LIBEMB}/internal.h
	${LIBEMB}/embroidery.h
	${LIBEMB}/data/arc_polyester_colors.c
	${LIBEMB}/data/arc_rayon_colors.c
	${LIBEMB}/data/coats_and_clark_rayon_colors.c
	${LIBEMB}/data/dxf_colors.c
	${LIBEMB}/data/exquisite_polyester_colors.c
	${LIBEMB}/data/fufu_polyester_colors.c
	${LIBEMB}/data/fufu_rayon_colors.c
	# ${LIBEMB}/data/hemingworth_polyester_colors.c
	${LIBEMB}/data/hus_colors.c
	# ${LIBEMB}/data/isacord_polyester_colors.c
	# ${LIBEMB}/data/isafil_rayon_colors.c
	${LIBEMB}/data/jef_colors.c
	# ${LIBEMB}/data/madeira_polyester_colors.c
	# ${LIBEMB}/data/madeira_rayon_colors.c
	# ${LIBEMB}/data/marathon_polyester_colors.c
	# ${LIBEMB}/data/marathon_rayon_colors.c
	# ${LIBEMB}/data/metro_polyester_colors.c
	# ${LIBEMB}/data/pantone_colors.c
	${LIBEMB}/data/pcm_colors.c
	${LIBEMB}/data/pec_colors.c
	# ${LIBEMB}/data/robison_anton_polyester_colors.c
	# ${LIBEMB}/data/robison_anton_rayon_colors.c
	${LIBEMB}/data/shv_colors.c
	# ${LIBEMB}/data/sigma_polyester_colors.c
	# ${LIBEMB}/data/sulky_rayon_colors.c
	${LIBEMB}/data/svg_colors.c
	# ${LIBEMB}/data/thread_art_polyester_colors.c
	# ${LIBEMB}/data/thread_art_rayon_colors.c
	# ${LIBEMB}/data/threadelight_polyester_colors.c
	# ${LIBEMB}/data/z102_isacord_polyester_colors.c
)

qt_add_executable(embroidermodder2
	${EXTERN}/nanovg/src/nanovg.c
	${EXTERN}/nanovg/src/nanovg.h

	${EXTERN}/glad/src/glad.c
	${EXTERN}/glad/include/glad/glad.h
	${EXTERN}/glad/include/KHR/khrplatform.h

	${LIBRARY_SRC}

	${SRC}/commands.cpp

	${SRC}/designs.c
	${SRC}/interface.c
	${SRC}/objects.c
	${SRC}/state.c
	${SRC}/translations.c
	${SRC}/icons.c
	${SRC}/xpm.c

	${SRC}/core.h
)

#add_executable(embroidermodder2_x11
#	${SRC}/wrapper.c
#	${SRC}/state.c
#
#	${SRC}/constants.h
#	${SRC}/core.h
#)

include_directories(
	${EXTERN}/libembroidery/src
	${EXTERN}/glfw/include
	${EXTERN}/glad/include
	${EXTERN}/nanovg/src
	${EXTERN}/nanosvg/src
	${OPENGL_INCLUDE_DIRS}
	${SRC}
)

target_link_libraries(embroidermodder2 PRIVATE
	glfw
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	Qt6::Svg
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
