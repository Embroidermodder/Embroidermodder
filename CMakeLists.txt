cmake_minimum_required(VERSION 3.16)

project(embroidermodder2 VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport Svg)

if(WIN32)
else(WIN32)
find_package(X11 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
endif(WIN32)

set(TARGET embroidermodder2)

set(SRC ${CMAKE_SOURCE_DIR}/embroidermodder2)
set(LIB ${CMAKE_SOURCE_DIR}/libembroidery)

set(CMDS ${CMAKE_SOURCE_DIR}/assets/commands)
set(ICONS ${CMAKE_SOURCE_DIR}/assets/icons)
set(IMAGES ${CMAKE_SOURCE_DIR}/assets/images)
set(SAMPLES ${CMAKE_SOURCE_DIR}/assets/samples)
set(HELP ${CMAKE_SOURCE_DIR}/assets/help)
set(TS ${CMAKE_SOURCE_DIR}/assets/translations)

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
	${SRC}/embdetails-dialog.cpp
	${SRC}/embdetails-dialog.h
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
	${SRC}/object-save.h
	${SRC}/mainwindow-actions.cpp
	${SRC}/object-textsingle.cpp
	${SRC}/mainwindow-actions.h
	${SRC}/object-textsingle.h
	${SRC}/mainwindow-commands.cpp
	${SRC}/preview-dialog.cpp
	${SRC}/mainwindow.cpp
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
	${SRC}/native-scripting.cpp
	${SRC}/statusbar-button.h
	${SRC}/native-scripting.h
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

# C CORE
	${SRC}/script.c
	${SRC}/script.h

	${LIB}/api-start.h
	${LIB}/api-stop.h
	${LIB}/compound-file.c
	${LIB}/compound-file-common.h
	${LIB}/compound-file-difat.c
	${LIB}/compound-file-difat.h
	${LIB}/compound-file-directory.c
	${LIB}/compound-file-directory.h
	${LIB}/compound-file-fat.c
	${LIB}/compound-file-fat.h
	${LIB}/compound-file.h
	${LIB}/compound-file-header.c
	${LIB}/compound-file-header.h
	${LIB}/emb-arc.c
	${LIB}/emb-arc.h
	${LIB}/emb-circle.c
	${LIB}/emb-circle.h
	${LIB}/emb-color.c
	${LIB}/emb-color.h
	${LIB}/emb-compress.c
	${LIB}/emb-compress.h
	${LIB}/emb-ellipse.c
	${LIB}/emb-ellipse.h
	${LIB}/emb-file.c
	${LIB}/emb-file.h
	${LIB}/emb-flag.c
	${LIB}/emb-flag.h
	${LIB}/emb-format.c
	${LIB}/emb-format.h
	${LIB}/emb-hash.c
	${LIB}/emb-hash.h
	${LIB}/emb-hoop.c
	${LIB}/emb-hoop.h
	${LIB}/emb-layer.c
	${LIB}/emb-layer.h
	${LIB}/emb-line.c
	${LIB}/emb-line.h
	${LIB}/emb-logging.c
	${LIB}/emb-logging.h
#    ${LIB}/emb-outline.c
#    ${LIB}/emb-outline.h
        ${LIB}/emb-path.c
	${LIB}/emb-path.h
	${LIB}/emb-pattern.c
	${LIB}/emb-pattern.h
	${LIB}/emb-point.c
	${LIB}/emb-point.h
	${LIB}/emb-polygon.c
	${LIB}/emb-polygon.h
	${LIB}/emb-polyline.c
	${LIB}/emb-polyline.h
	${LIB}/emb-reader-writer.c
	${LIB}/emb-reader-writer.h
	${LIB}/emb-rect.c
	${LIB}/emb-rect.h
	${LIB}/emb-satin-line.c
	${LIB}/emb-satin-line.h
	${LIB}/emb-settings.c
	${LIB}/emb-settings.h
	${LIB}/emb-spline.c
	${LIB}/emb-spline.h
	${LIB}/emb-stitch.c
	${LIB}/emb-stitch.h
	${LIB}/emb-thread.c
	${LIB}/emb-thread.h
	${LIB}/emb-time.c
	${LIB}/emb-time.h
	${LIB}/emb-vector.c
	${LIB}/emb-vector.h
	${LIB}/format-100.c
	${LIB}/format-100.h
	${LIB}/format-10o.c
	${LIB}/format-10o.h
	${LIB}/format-art.c
	${LIB}/format-art.h
	${LIB}/format-bmc.c
	${LIB}/format-bmc.h
	${LIB}/format-bro.c
	${LIB}/format-bro.h
	${LIB}/format-cnd.c
	${LIB}/format-cnd.h
	${LIB}/format-col.c
	${LIB}/format-col.h
	${LIB}/format-csd.c
	${LIB}/format-csd.h
	${LIB}/format-csv.c
	${LIB}/format-csv.h
	${LIB}/format-dat.c
	${LIB}/format-dat.h
	${LIB}/format-dem.c
	${LIB}/format-dem.h
	${LIB}/format-dsb.c
	${LIB}/format-dsb.h
	${LIB}/format-dst.c
	${LIB}/format-dst.h
	${LIB}/format-dsz.c
	${LIB}/format-dsz.h
	${LIB}/format-dxf.c
	${LIB}/format-dxf.h
	${LIB}/format-edr.c
	${LIB}/format-edr.h
	${LIB}/format-emd.c
	${LIB}/format-emd.h
	${LIB}/format-exp.c
	${LIB}/format-exp.h
	${LIB}/format-exy.c
	${LIB}/format-exy.h
	${LIB}/format-eys.c
	${LIB}/format-eys.h
	${LIB}/format-fxy.c
	${LIB}/format-fxy.h
	${LIB}/format-gc.c
	${LIB}/format-gc.h
	${LIB}/format-gnc.c
	${LIB}/format-gnc.h
	${LIB}/format-gt.c
	${LIB}/format-gt.h
	${LIB}/format-hus.c
	${LIB}/format-hus.h
	${LIB}/format-inb.c
	${LIB}/format-inb.h
	${LIB}/format-inf.c
	${LIB}/format-inf.h
	${LIB}/format-jef.c
	${LIB}/format-jef.h
	${LIB}/format-ksm.c
	${LIB}/format-ksm.h
	${LIB}/format-max.c
	${LIB}/format-max.h
	${LIB}/format-mit.c
	${LIB}/format-mit.h
	${LIB}/format-new.c
	${LIB}/format-new.h
	${LIB}/format-ofm.c
	${LIB}/format-ofm.h
	${LIB}/format-pcd.c
	${LIB}/format-pcd.h
	${LIB}/format-pcm.c
	${LIB}/format-pcm.h
	${LIB}/format-pcq.c
	${LIB}/format-pcq.h
	${LIB}/format-pcs.c
	${LIB}/format-pcs.h
	${LIB}/format-pec.c
	${LIB}/format-pec.h
	${LIB}/format-pel.c
	${LIB}/format-pel.h
	${LIB}/format-pem.c
	${LIB}/format-pem.h
	${LIB}/format-pes.c
	${LIB}/format-pes.h
	${LIB}/format-phb.c
	${LIB}/format-phb.h
	${LIB}/format-phc.c
	${LIB}/format-phc.h
	${LIB}/format-plt.c
	${LIB}/format-plt.h
	${LIB}/format-rgb.c
	${LIB}/format-rgb.h
	${LIB}/format-sew.c
	${LIB}/format-sew.h
	${LIB}/formats.h
	${LIB}/format-shv.c
	${LIB}/format-shv.h
	${LIB}/format-sst.c
	${LIB}/format-sst.h
	${LIB}/format-stx.c
	${LIB}/format-stx.h
	${LIB}/format-svg.c
	${LIB}/format-svg.h
	${LIB}/format-t01.c
	${LIB}/format-t01.h
	${LIB}/format-t09.c
	${LIB}/format-t09.h
	${LIB}/format-tap.c
	${LIB}/format-tap.h
	${LIB}/format-thr.c
	${LIB}/format-thr.h
	${LIB}/format-txt.c
	${LIB}/format-txt.h
	${LIB}/format-u00.c
	${LIB}/format-u00.h
	${LIB}/format-u01.c
	${LIB}/format-u01.h
	${LIB}/format-vip.c
	${LIB}/format-vip.h
	${LIB}/format-vp3.c
	${LIB}/format-vp3.h
	${LIB}/format-xxx.c
	${LIB}/format-xxx.h
	${LIB}/format-zsk.c
	${LIB}/format-zsk.h
	${LIB}/geom-arc.c
	${LIB}/geom-arc.h
	${LIB}/geom-circle.c
	${LIB}/geom-circle.h
	${LIB}/geom-line.c
	${LIB}/geom-line.h
	${LIB}/hashtable.c
	${LIB}/hashtable.h
	${LIB}/helpers-binary.c
	${LIB}/helpers-binary.h
	${LIB}/helpers-misc.c
	${LIB}/helpers-misc.h
	${LIB}/helpers-unused.h
	${LIB}/thread-color.c
	${LIB}/thread-color.h
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
file(COPY "${ICONS}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${IMAGES}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SAMPLES}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${HELP}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${TS}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets/tips.txt"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

include_directories(
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/libembroidery
	${OPENGL_INCLUDE_DIRS}
	${SRC}
)

target_link_libraries(${TARGET} PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	Qt6::Svg
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

