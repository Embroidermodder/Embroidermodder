cmake_minimum_required(VERSION 3.5)

project(embroidermodder VERSION 2.0.0 LANGUAGES C)

#set(CMAKE_C_STANDARD 90)
#set(CMAKE_C_STANDARD_REQUIRED True)

option( EMBROIDERY_BUILD_SHARED_LIBS "Build shared library." OFF )
option( EMBROIDERY_BUILD_CLI "Build embroider command line tool." OFF )

add_compile_options(
	-g
	-O2
	-fPIC
	-std=c99
	-pedantic
	-Wall
	-Wextra
#	-Werror
)

add_executable(embroidermodder 
	src/libembroidery/src/main.c
	src/libembroidery/src/array.c
	src/libembroidery/src/embroidery.h
	src/libembroidery/src/formats.c
	src/libembroidery/src/pattern.c
	src/libembroidery/src/image.c
	src/libembroidery/src/fill.c
	src/libembroidery/src/geometry.c
	src/libembroidery/src/compress.c
	src/libembroidery/src/encoding.c
	src/libembroidery/src/thread-color.c
	src/em2.c
	src/actuator.c
	src/compatibility_layer.c
)

target_include_directories(embroidermodder INTERFACE src/stb src src/libembroidery/src)

if (WIN32)
target_link_libraries(embroidermodder PRIVATE Gdi32)
else (WIN32)
target_link_libraries(embroidermodder PRIVATE X11 m)
endif (WIN32)

if(EMBROIDERY_BUILD_SHARED_LIBS)
install(
    FILES
        src/embroidery.h
    DESTINATION /usr/include
)

# Needs some way of installing the assets folder.
install(
    TARGETS
       embroidermodder
    RUNTIME DESTINATION /usr/bin
)
endif()
