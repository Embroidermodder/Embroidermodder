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

set(CMDS ${CMAKE_SOURCE_DIR}/commands)
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

qt_add_executable(${TARGET}
	${SRC}/application.cpp
	${SRC}/object-ellipse.h
	${SRC}/application.h
	${SRC}/cmdprompt.cpp
	${SRC}/cmdprompt.h
	${SRC}/object-image.cpp
	${SRC}/object-image.h
	${SRC}/object-line.cpp
	${SRC}/object-line.h
	${SRC}/dialog.cpp
	${SRC}/dialog.h
	${SRC}/object-path.cpp
	${SRC}/object-path.h
	${SRC}/embroidermodder2.icns
	${SRC}/object-point.cpp
	${SRC}/embroidermodder2.ico
	${SRC}/object-point.h
	${SRC}/object-polygon.cpp
	${SRC}/object-polygon.h
	${SRC}/imagewidget.cpp
	${SRC}/object-polyline.cpp
	${SRC}/imagewidget.h
	${SRC}/object-polyline.h
	${SRC}/Info.plist
	${SRC}/object-rect.cpp
	${SRC}/layer-manager.cpp
	${SRC}/object-rect.h
	${SRC}/layer-manager.h
	${SRC}/object-save.cpp
	${SRC}/main.cpp
	${SRC}/mainwindow.cpp
	${SRC}/mainwindow-actions.cpp
	${SRC}/mainwindow-actions.h
	${SRC}/mainwindow-commands.cpp
	${SRC}/object-save.h
	${SRC}/object-textsingle.cpp
	${SRC}/object-textsingle.h
	${SRC}/preview-dialog.cpp
	${SRC}/preview-dialog.h
	${SRC}/mainwindow.h
	${SRC}/property-editor.cpp
	${SRC}/mainwindow-menus.cpp
	${SRC}/property-editor.h
	${SRC}/mainwindow-settings.cpp
	${SRC}/mainwindow-toolbars.cpp
	${SRC}/selectbox.cpp
	${SRC}/mdiarea.cpp
	${SRC}/selectbox.h
	${SRC}/mdiarea.h
	${SRC}/settings-dialog.cpp
	${SRC}/mdiwindow.cpp
	${SRC}/settings-dialog.h
	${SRC}/mdiwindow.h
	${SRC}/statusbar-button.cpp
	${SRC}/statusbar-button.h
	${SRC}/statusbar.cpp
	${SRC}/object-arc.cpp
	${SRC}/statusbar.h
	${SRC}/undo-commands.cpp
	${SRC}/undo-commands.h
	${SRC}/undo-editor.cpp
	${SRC}/object-data.h
	${SRC}/object-arc.h
	${SRC}/object-base.cpp
	${SRC}/object-base.h
	${SRC}/object-circle.cpp
	${SRC}/object-circle.h
	${SRC}/object-dimleader.cpp
	${SRC}/object-dimleader.h
	${SRC}/object-ellipse.cpp
	${SRC}/undo-editor.h
	${SRC}/view.cpp
	${SRC}/view.h
	${SRC}/script.cpp
	${SRC}/script.h

# libembroidery
	${LIB}/compound-file.c
	${LIB}/compound-file-difat.c
	${LIB}/compound-file-directory.c
	${LIB}/compound-file-fat.c
	${LIB}/compound-file-header.c
	${LIB}/emb-arc.c
	${LIB}/emb-circle.c
	${LIB}/emb-color.c
	${LIB}/emb-compress.c
	${LIB}/emb-ellipse.c
	${LIB}/emb-file.c
	${LIB}/emb-flag.c
	${LIB}/emb-format.c
	${LIB}/emb-hash.c
	${LIB}/emb-hoop.c
	${LIB}/emb-layer.c
	${LIB}/emb-line.c
	${LIB}/emb-logging.c
#    ${LIB}/emb-outline.c
        ${LIB}/emb-path.c
	${LIB}/emb-pattern.c
	${LIB}/emb-point.c
	${LIB}/emb-polygon.c
	${LIB}/emb-polyline.c
	${LIB}/emb-reader-writer.c
	${LIB}/emb-rect.c
	${LIB}/emb-satin-line.c
	${LIB}/emb-settings.c
	${LIB}/emb-spline.c
	${LIB}/emb-stitch.c
	${LIB}/emb-thread.c
	${LIB}/emb-time.c
	${LIB}/emb-vector.c
	${LIB}/format-100.c
	${LIB}/format-10o.c
	${LIB}/format-art.c
	${LIB}/format-bmc.c
	${LIB}/format-bro.c
	${LIB}/format-cnd.c
	${LIB}/format-col.c
	${LIB}/format-csd.c
	${LIB}/format-csv.c
	${LIB}/format-dat.c
	${LIB}/format-dem.c
	${LIB}/format-dsb.c
	${LIB}/format-dst.c
	${LIB}/format-dsz.c
	${LIB}/format-dxf.c
	${LIB}/format-edr.c
	${LIB}/format-emd.c
	${LIB}/format-exp.c
	${LIB}/format-exy.c
	${LIB}/format-eys.c
	${LIB}/format-fxy.c
	${LIB}/format-gc.c
	${LIB}/format-gnc.c
	${LIB}/format-gt.c
	${LIB}/format-hus.c
	${LIB}/format-inb.c
	${LIB}/format-inf.c
	${LIB}/format-jef.c
	${LIB}/format-ksm.c
	${LIB}/format-max.c
	${LIB}/format-mit.c
	${LIB}/format-new.c
	${LIB}/format-ofm.c
	${LIB}/format-pcd.c
	${LIB}/format-pcm.c
	${LIB}/format-pcq.c
	${LIB}/format-pcs.c
	${LIB}/format-pec.c
	${LIB}/format-pel.c
	${LIB}/format-pem.c
	${LIB}/format-pes.c
	${LIB}/format-phb.c
	${LIB}/format-phc.c
	${LIB}/format-plt.c
	${LIB}/format-rgb.c
	${LIB}/format-sew.c
	${LIB}/format-shv.c
	${LIB}/format-sst.c
	${LIB}/format-stx.c
	${LIB}/format-svg.c
	${LIB}/format-t01.c
	${LIB}/format-t09.c
	${LIB}/format-tap.c
	${LIB}/format-thr.c
	${LIB}/format-txt.c
	${LIB}/format-u00.c
	${LIB}/format-u01.c
	${LIB}/format-vip.c
	${LIB}/format-vp3.c
	${LIB}/format-xxx.c
	${LIB}/format-zsk.c
	${LIB}/geom-arc.c
	${LIB}/geom-circle.c
	${LIB}/geom-line.c
	${LIB}/hashtable.c
	${LIB}/helpers-binary.c
	${LIB}/helpers-misc.c
	${LIB}/thread-color.c

	${LIB}/embroidery.h
	${LIB}/hashtable.h

	${EXTERN}/tinyscheme/source/scheme.c
	${EXTERN}/tinyscheme/source/dynload.c
	${EXTERN}/tinyscheme/source/mod_json.c
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
file(COPY "${CMDS}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
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

