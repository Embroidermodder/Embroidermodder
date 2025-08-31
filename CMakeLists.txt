cmake_minimum_required(VERSION 3.16)

project(embroidermodder VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

add_compile_options(
	-g
	-pg
	-O2
	-Wall
	-Wextra
	--coverage
)

add_link_options(
	-pg
	--coverage
)

else()
add_compile_options(
	-g
	-O2
	-Wall
)
endif()

#add_subdirectory(${CMAKE_SOURCE_DIR}/extern/libembroidery)

# The makers of CMake advise that you do not glob source files.
#
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#glob
#
# Since our ASSETS are essential for the program to run, we are going to
# give them the same treatment and list them explicitly here.
#

set(SRC ${CMAKE_SOURCE_DIR}/src)
set(LIBEMB ${CMAKE_SOURCE_DIR}/libembroidery)
set(EXTERN ${CMAKE_SOURCE_DIR}/extern)
set(SRC_ASSETS ${CMAKE_SOURCE_DIR}/assets)

add_subdirectory(extern/libembroidery)

qt_add_executable(embroidermodder2
	${SRC}/application.cpp
	${SRC}/cmdprompt.cpp
	${SRC}/embdetails-dialog.cpp
	${SRC}/imagewidget.cpp
	${SRC}/layer-manager.cpp
	${SRC}/main.cpp
	${SRC}/mainwindow.cpp
	${SRC}/mdiarea.cpp
	${SRC}/mdiwindow.cpp
	${SRC}/object-arc.cpp
	${SRC}/object-base.cpp
	${SRC}/object-circle.cpp
	${SRC}/object-dimleader.cpp
	${SRC}/object-ellipse.cpp
	${SRC}/object-image.cpp
	${SRC}/object-line.cpp
	${SRC}/object-path.cpp
	${SRC}/object-point.cpp
	${SRC}/object-polygon.cpp
	${SRC}/object-polyline.cpp
	${SRC}/object-rect.cpp
	${SRC}/object-save.cpp
	${SRC}/object-textsingle.cpp
	${SRC}/preview-dialog.cpp
	${SRC}/property-editor.cpp
	${SRC}/script.cpp
	${SRC}/selectbox.cpp
	${SRC}/settings-dialog.cpp
	${SRC}/statusbar.cpp
	${SRC}/undo-commands.cpp
	${SRC}/undo-editor.cpp
	${SRC}/view.cpp

	${SRC}/embroidermodder.h

	${SRC}/data/commands.c
	${SRC}/data/settings.c
	${SRC}/data/data.h

	${EXTERN}/lua/src/lapi.c
	${EXTERN}/lua/src/lapi.h
	${EXTERN}/lua/src/lauxlib.c
	${EXTERN}/lua/src/lauxlib.h
	${EXTERN}/lua/src/lbaselib.c
	${EXTERN}/lua/src/lcode.c
	${EXTERN}/lua/src/lcode.h
	${EXTERN}/lua/src/lcorolib.c
	${EXTERN}/lua/src/lctype.c
	${EXTERN}/lua/src/lctype.h
    ${EXTERN}/lua/src/ldblib.c
    ${EXTERN}/lua/src/ldebug.c
    ${EXTERN}/lua/src/ldebug.h
    ${EXTERN}/lua/src/ldo.c
    ${EXTERN}/lua/src/ldo.h
    ${EXTERN}/lua/src/ldump.c
    ${EXTERN}/lua/src/lfunc.c
    ${EXTERN}/lua/src/lfunc.h
    ${EXTERN}/lua/src/lgc.c
    ${EXTERN}/lua/src/lgc.h
    ${EXTERN}/lua/src/linit.c
    ${EXTERN}/lua/src/liolib.c
    ${EXTERN}/lua/src/ljumptab.h
    ${EXTERN}/lua/src/llex.c
    ${EXTERN}/lua/src/llex.h
    ${EXTERN}/lua/src/llimits.h
    ${EXTERN}/lua/src/lmathlib.c
    ${EXTERN}/lua/src/lmem.c
    ${EXTERN}/lua/src/lmem.h
    ${EXTERN}/lua/src/loadlib.c
    ${EXTERN}/lua/src/lobject.c
    ${EXTERN}/lua/src/lobject.h
    ${EXTERN}/lua/src/lopcodes.c
    ${EXTERN}/lua/src/lopcodes.h
    ${EXTERN}/lua/src/lopnames.h
    ${EXTERN}/lua/src/loslib.c
    ${EXTERN}/lua/src/lparser.c
    ${EXTERN}/lua/src/lparser.h
    ${EXTERN}/lua/src/lprefix.h
    ${EXTERN}/lua/src/lstate.c
    ${EXTERN}/lua/src/lstate.h
    ${EXTERN}/lua/src/lstring.c
    ${EXTERN}/lua/src/lstring.h
    ${EXTERN}/lua/src/lstrlib.c
    ${EXTERN}/lua/src/ltable.c
    ${EXTERN}/lua/src/ltable.h
    ${EXTERN}/lua/src/ltablib.c
    ${EXTERN}/lua/src/ltm.c
    ${EXTERN}/lua/src/ltm.h
#    ${EXTERN}/lua/src/lua.c
#    ${EXTERN}/lua/src/luac.c
    ${EXTERN}/lua/src/luaconf.h
    ${EXTERN}/lua/src/lua.h
    ${EXTERN}/lua/src/lua.hpp
    ${EXTERN}/lua/src/lualib.h
    ${EXTERN}/lua/src/lundump.c
    ${EXTERN}/lua/src/lundump.h
    ${EXTERN}/lua/src/lutf8lib.c
    ${EXTERN}/lua/src/lvm.c
    ${EXTERN}/lua/src/lvm.h
    ${EXTERN}/lua/src/lzio.c
    ${EXTERN}/lua/src/lzio.h
)

# WARNING: This does NOT regenerate when the assets change, so you have to
# rerun cmake to ensure that this retriggers.
#
file(COPY "${CMAKE_SOURCE_DIR}/images"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/icons"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/scripts"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/samples"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/translations"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/docs"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/LICENSE.md"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/config.toml"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

include_directories(
	${CMAKE_SOURCE_DIR}/embroidermodder2
	${CMAKE_SOURCE_DIR}/extern/libembroidery/src
)

target_link_libraries(embroidermodder2 PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	embroidery_static
	${OPENGL_LIBRARIES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
target_link_libraries(embroidermodder2 PRIVATE
	gcov
)
endif()

set_target_properties(embroidermodder2 PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)

if (WIN32)
	set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder2.exe)
else(WIN32)
	target_link_libraries(embroidermodder2 PRIVATE m)
	set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder)
endif()
