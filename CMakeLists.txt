cmake_minimum_required(VERSION 3.16)

project(embroidermodder2 VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport Svg Xml)

if(WIN32)
else(WIN32)
find_package(X11 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
endif(WIN32)

set(TARGET embroidermodder2)

set(SRC ${CMAKE_SOURCE_DIR}/src)
set(EXTERN ${CMAKE_SOURCE_DIR}/extern)
set(LIB ${CMAKE_SOURCE_DIR}/extern/libembroidery)

set(DIALOGS ${CMAKE_SOURCE_DIR}/dialogs)
set(ICONS ${CMAKE_SOURCE_DIR}/icons)
set(IMAGES ${CMAKE_SOURCE_DIR}/images)
set(SAMPLES ${CMAKE_SOURCE_DIR}/samples)
set(HELP ${CMAKE_SOURCE_DIR}/help)
set(TS ${CMAKE_SOURCE_DIR}/translations)

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

add_subdirectory(extern/libembroidery)
add_subdirectory(extern/tinyscheme)

qt_add_executable(${TARGET}
	${SRC}/embroidermodder2.icns
	${SRC}/embroidermodder2.ico
	${SRC}/Info.plist

	${SRC}/application.cpp
	${SRC}/cmdprompt.cpp
	${SRC}/dialog.cpp
	${SRC}/imagewidget.cpp
	${SRC}/layer-manager.cpp
	${SRC}/main.cpp
	${SRC}/mainwindow.cpp
	${SRC}/mainwindow-commands.cpp
	${SRC}/mainwindow-menus.cpp
	${SRC}/mainwindow-settings.cpp
	${SRC}/mainwindow-toolbars.cpp
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
	${SRC}/statusbar-button.cpp
	${SRC}/undo-commands.cpp
	${SRC}/undo-editor.cpp
	${SRC}/view.cpp

	${SRC}/embroidermodder.h
)

qt_add_translations(embroidermodder2 TS_FILES
	${TS}/afrikaans.ts
	${TS}/arabic.ts
	${TS}/chinese.ts
	${TS}/czech.ts
	${TS}/danish.ts
	${TS}/dutch.ts
	${TS}/english.ts
	${TS}/finnish.ts
	${TS}/french.ts
	${TS}/german.ts
	${TS}/greek.ts
	${TS}/italian.ts
	${TS}/japanese.ts
	${TS}/korean.ts
	${TS}/latin.ts
	${TS}/polish.ts
	${TS}/portuguese.ts
	${TS}/romanian.ts
	${TS}/russian.ts
	${TS}/spanish.ts
	${TS}/swedish.ts
	${TS}/turkish.ts
)

# WARNING: This does NOT regenerate when the assets change, so you have to
# rerun cmake to ensure that this retriggers.
#
file(COPY "${DIALOGS}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${ICONS}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${IMAGES}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SAMPLES}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${HELP}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${TS}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/tips.txt"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/load.scm"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

include_directories(
	${CMAKE_SOURCE_DIR}/src
	${LIB}
	${EXTERN}/tinyscheme/include
	${OPENGL_INCLUDE_DIRS}
	${SRC}
)

target_link_libraries(${TARGET} PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	Qt6::Svg
	Qt6::Xml
	embroidery_static
	${OPENGL_LIBRARIES}
)

set_target_properties(${TARGET} PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)

if (WIN32)
else(WIN32)
	target_link_libraries(${TARGET} PRIVATE m)
endif()

