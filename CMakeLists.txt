cmake_minimum_required(VERSION 3.16)

project(embroidermodder VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport Qml)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

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

set(SRC ${CMAKE_SOURCE_DIR}/embroidermodder2)

add_subdirectory(libembroidery)

# The makers of CMake advise that you do not glob source files.
#
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#glob
#
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
	${SRC}/mainwindow-actions.cpp
	${SRC}/mainwindow-actions.h
	${SRC}/mainwindow-commands.cpp
	${SRC}/mainwindow.cpp
	${SRC}/mainwindow.h
	${SRC}/mainwindow-menus.cpp
	${SRC}/mainwindow-settings.cpp
	${SRC}/mainwindow-toolbars.cpp
	${SRC}/mdiarea.cpp
	${SRC}/mdiarea.h
	${SRC}/mdiwindow.cpp
	${SRC}/mdiwindow.h
	${SRC}/native-scripting.cpp
	${SRC}/native-scripting.h
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
)

# WARNING: This does NOT regenerate when the assets change, so you have to
# rerun cmake to ensure that this retriggers.
#
file(COPY "${SRC}/commands"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC}/images"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC}/icons"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC}/samples"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC}/translations"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC}/help"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SRC}/tips.txt"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/LICENSE.md"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

include_directories(
	${CMAKE_SOURCE_DIR}/embroidermodder2
	${CMAKE_SOURCE_DIR}/embroidermodder2/javascript
	${CMAKE_SOURCE_DIR}/libembroidery
)

target_link_libraries(embroidermodder2 PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	Qt6::Qml
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

if(WIN32)
else(WIN32)
	target_link_libraries(embroidermodder2 PRIVATE m)
endif()
