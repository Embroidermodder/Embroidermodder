cmake_minimum_required(VERSION 3.5)

project(embroidermodder VERSION 2.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

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

file(GLOB SRC
	${PROJECT_SOURCE_DIR}/src/*.c
	${PROJECT_SOURCE_DIR}/src/libembroidery/src/*.c
)

add_executable(embroidermodder 
	${SRC}
)

target_include_directories(embroidermodder INTERFACE src/stb src src/libembroidery/src)

file(GLOB ASSETS
	${PROJECT_SOURCE_DIR}/assets/*.csv
	${PROJECT_SOURCE_DIR}/assets/**/*.csv
	${PROJECT_SOURCE_DIR}/assets/**/*.ttf
	${PROJECT_SOURCE_DIR}/assets/**/*.txt
	${PROJECT_SOURCE_DIR}/assets/**/*.md
)

if (WIN32)
target_link_libraries(embroidermodder PRIVATE Gdi32)

install(
    FILES embroidermodder.exe
	DESTINATION "C:/Program Files/embroidermodder"
)
install(
    FILES ${ASSETS}
	DESTINATION "C:/ProgramData/embroidermodder"
)
else (WIN32)
target_link_libraries(embroidermodder PRIVATE X11 m)

install(
    FILES embroidermodder
	DESTINATION "/usr/bin"
)
install(
    FILES ${ASSETS}
	DESTINATION "/usr/share/embroidermodder"
)
endif (WIN32)
