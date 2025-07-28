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
	${SRC}/application.h
	${SRC}/cmdprompt.cpp
	${SRC}/cmdprompt.h
	${SRC}/embdetails-dialog.cpp
	${SRC}/embdetails-dialog.h
	${SRC}/imagewidget.cpp
	${SRC}/imagewidget.h
	${SRC}/layer-manager.cpp
	${SRC}/layer-manager.h
	${SRC}/main.cpp
	${SRC}/mainwindow.cpp
	${SRC}/mainwindow.h
	${SRC}/mainwindow-menus.cpp
	${SRC}/mainwindow-settings.cpp
	${SRC}/mainwindow-toolbars.cpp
	${SRC}/mdiarea.cpp
	${SRC}/mdiarea.h
	${SRC}/mdiwindow.cpp
	${SRC}/mdiwindow.h
	${SRC}/object-arc.cpp
	${SRC}/object-arc.h
	${SRC}/object-base.cpp
	${SRC}/object-base.h
	${SRC}/object-circle.cpp
	${SRC}/object-circle.h
	${SRC}/object-data.h
	${SRC}/object-dimleader.cpp
	${SRC}/object-dimleader.h
	${SRC}/object-ellipse.cpp
	${SRC}/object-ellipse.h
	${SRC}/object-image.cpp
	${SRC}/object-image.h
	${SRC}/object-line.cpp
	${SRC}/object-line.h
	${SRC}/object-path.cpp
	${SRC}/object-path.h
	${SRC}/object-point.cpp
	${SRC}/object-point.h
	${SRC}/object-polygon.cpp
	${SRC}/object-polygon.h
	${SRC}/object-polyline.cpp
	${SRC}/object-polyline.h
	${SRC}/object-rect.cpp
	${SRC}/object-rect.h
	${SRC}/object-save.cpp
	${SRC}/object-save.h
	${SRC}/object-textsingle.cpp
	${SRC}/object-textsingle.h
	${SRC}/preview-dialog.cpp
	${SRC}/preview-dialog.h
	${SRC}/property-editor.cpp
	${SRC}/property-editor.h
	${SRC}/script.cpp
	${SRC}/script.h
	${SRC}/selectbox.cpp
	${SRC}/selectbox.h
	${SRC}/settings-dialog.cpp
	${SRC}/settings-dialog.h
	${SRC}/statusbar-button.cpp
	${SRC}/statusbar-button.h
	${SRC}/statusbar.cpp
	${SRC}/statusbar.h
	${SRC}/undo-commands.cpp
	${SRC}/undo-commands.h
	${SRC}/undo-editor.cpp
	${SRC}/undo-editor.h
	${SRC}/view.cpp
	${SRC}/view.h

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

include_directories(
	${CMAKE_SOURCE_DIR}/embroidermodder2
	${CMAKE_SOURCE_DIR}/extern/libembroidery/src
	${CMAKE_SOURCE_DIR}/extern/tinyscheme/include
)

target_link_libraries(embroidermodder2 PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	embroidery_static
	${OPENGL_LIBRARIES}
)

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
