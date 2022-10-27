cmake_minimum_required(VERSION 3.5)

project(embroidermodder VERSION 1.0.0 LANGUAGES C CXX)

option(EMBROIDERY_BUILD_SHARED_LIBS "Disable unused shared library by default." OFF)
option(EMBROIDERY_BUILD_CLI "Disable embroider by default." OFF)

# These aren't all working.
option(BUILD_SHARED_LIBS "." OFF)
option(SDL2_DISABLE_SDL2MAIN "." ON)
option(SDL_TEST "." OFF)
option(SDL_AUDIO "." OFF)
option(BUILD_SDL "." OFF)

option(DEBUG "." OFF)

if (DEBUG)
# Win32 uses compilers that complain about these flags.
if (WIN32)
else (WIN32)
#add_compile_options(-g -O2 -pedantic -Wall -Wextra -Werror)
add_compile_options(-g -O2 -pedantic)
endif (WIN32)
endif (DEBUG)

file(GLOB INTERNAL_SRCS src/*.c src/*.h)
file(GLOB LIBEMBROIDERY_SRCS extern/libembroidery/src/*.c extern/libembroidery/src/*.h)
file(GLOB ASSETS assets/*.scm)

add_executable(embroidermodder
	${INTERNAL_SRCS}
	${LIBEMBROIDERY_SRCS}
	extern/TinyScheme/src/scheme.c
	extern/TinyScheme/src/api.c
	extern/TinyScheme/src/scheme.h
	extern/TinyScheme/src/scheme-private.h
	extern/TinyScheme/src/dynload.c
)


if (BUILD_SDL)
add_subdirectory(extern/SDL)
add_subdirectory(extern/SDL_image)
add_subdirectory(extern/SDL_ttf)
else()
find_library(embroidermodder SDL2)
find_library(embroidermodder SDL2_image)
find_library(embroidermodder SDL2_ttf)
endif()

if (WIN32)
target_link_libraries(embroidermodder PRIVATE
	SDL2
	SDL2_image
	SDL2_ttf
)
else (WIN32)
target_link_libraries(embroidermodder PRIVATE
	SDL2
	SDL2_image
	SDL2_ttf
	m
	dl
)
endif (WIN32)

# Installation needs further testing

install(
    TARGETS embroidermodder
    DESTINATION /usr/bin
)

install(
    FILES ${ASSETS}
    DESTINATION /usr/share/embroidermodder
)

