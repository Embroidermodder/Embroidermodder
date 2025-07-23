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

set(SRC ${CMAKE_SOURCE_DIR}/embroidermodder2)
set(LIBEMB ${CMAKE_SOURCE_DIR}/libembroidery)
set(EXTERN ${CMAKE_SOURCE_DIR}/extern)
set(SRC_ASSETS ${CMAKE_SOURCE_DIR}/assets)

qt_add_executable(embroidermodder
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

	${LIBEMB}/api-start.h
	${LIBEMB}/api-stop.h
	${LIBEMB}/compound-file.c
	${LIBEMB}/compound-file-common.h
	${LIBEMB}/compound-file-difat.c
	${LIBEMB}/compound-file-difat.h
	${LIBEMB}/compound-file-directory.c
	${LIBEMB}/compound-file-directory.h
	${LIBEMB}/compound-file-fat.c
	${LIBEMB}/compound-file-fat.h
	${LIBEMB}/compound-file.h
	${LIBEMB}/compound-file-header.c
	${LIBEMB}/compound-file-header.h
	${LIBEMB}/emb-arc.c
	${LIBEMB}/emb-arc.h
	${LIBEMB}/emb-circle.c
	${LIBEMB}/emb-circle.h
	${LIBEMB}/emb-color.c
	${LIBEMB}/emb-color.h
	${LIBEMB}/emb-compress.c
	${LIBEMB}/emb-compress.h
	${LIBEMB}/emb-ellipse.c
	${LIBEMB}/emb-ellipse.h
	${LIBEMB}/emb-file.c
	${LIBEMB}/emb-file.h
	${LIBEMB}/emb-flag.c
	${LIBEMB}/emb-flag.h
	${LIBEMB}/emb-format.c
	${LIBEMB}/emb-format.h
	${LIBEMB}/emb-hash.c
	${LIBEMB}/emb-hash.h
	${LIBEMB}/emb-hoop.c
	${LIBEMB}/emb-hoop.h
	${LIBEMB}/emb-layer.c
	${LIBEMB}/emb-layer.h
	${LIBEMB}/emb-line.c
	${LIBEMB}/emb-line.h
	${LIBEMB}/emb-logging.c
	${LIBEMB}/emb-logging.h
	${LIBEMB}/emb-path.c
	${LIBEMB}/emb-path.h
	${LIBEMB}/emb-pattern.c
	${LIBEMB}/emb-pattern.h
	${LIBEMB}/emb-point.c
	${LIBEMB}/emb-point.h
	${LIBEMB}/emb-polygon.c
	${LIBEMB}/emb-polygon.h
	${LIBEMB}/emb-polyline.c
	${LIBEMB}/emb-polyline.h
	${LIBEMB}/emb-reader-writer.c
	${LIBEMB}/emb-reader-writer.h
	${LIBEMB}/emb-rect.c
	${LIBEMB}/emb-rect.h
	${LIBEMB}/emb-satin-line.c
	${LIBEMB}/emb-satin-line.h
	${LIBEMB}/emb-settings.c
	${LIBEMB}/emb-settings.h
	${LIBEMB}/emb-spline.c
	${LIBEMB}/emb-spline.h
	${LIBEMB}/emb-stitch.c
	${LIBEMB}/emb-stitch.h
	${LIBEMB}/emb-thread.c
	${LIBEMB}/emb-thread.h
	${LIBEMB}/emb-time.c
	${LIBEMB}/emb-time.h
	${LIBEMB}/emb-vector.c
	${LIBEMB}/emb-vector.h
	${LIBEMB}/format-100.c
	${LIBEMB}/format-100.h
	${LIBEMB}/format-10o.c
	${LIBEMB}/format-10o.h
	${LIBEMB}/format-art.c
	${LIBEMB}/format-art.h
	${LIBEMB}/format-bmc.c
	${LIBEMB}/format-bmc.h
	${LIBEMB}/format-bro.c
	${LIBEMB}/format-bro.h
	${LIBEMB}/format-cnd.c
	${LIBEMB}/format-cnd.h
	${LIBEMB}/format-col.c
	${LIBEMB}/format-col.h
	${LIBEMB}/format-csd.c
	${LIBEMB}/format-csd.h
	${LIBEMB}/format-csv.c
	${LIBEMB}/format-csv.h
	${LIBEMB}/format-dat.c
	${LIBEMB}/format-dat.h
	${LIBEMB}/format-dem.c
	${LIBEMB}/format-dem.h
	${LIBEMB}/format-dsb.c
	${LIBEMB}/format-dsb.h
	${LIBEMB}/format-dst.c
	${LIBEMB}/format-dst.h
	${LIBEMB}/format-dsz.c
	${LIBEMB}/format-dsz.h
	${LIBEMB}/format-dxf.c
	${LIBEMB}/format-dxf.h
	${LIBEMB}/format-edr.c
	${LIBEMB}/format-edr.h
	${LIBEMB}/format-emd.c
	${LIBEMB}/format-emd.h
	${LIBEMB}/format-exp.c
	${LIBEMB}/format-exp.h
	${LIBEMB}/format-exy.c
	${LIBEMB}/format-exy.h
	${LIBEMB}/format-eys.c
	${LIBEMB}/format-eys.h
	${LIBEMB}/format-fxy.c
	${LIBEMB}/format-fxy.h
	${LIBEMB}/format-gc.c
	${LIBEMB}/format-gc.h
	${LIBEMB}/format-gnc.c
	${LIBEMB}/format-gnc.h
	${LIBEMB}/format-gt.c
	${LIBEMB}/format-gt.h
	${LIBEMB}/format-hus.c
	${LIBEMB}/format-hus.h
	${LIBEMB}/format-inb.c
	${LIBEMB}/format-inb.h
	${LIBEMB}/format-inf.c
	${LIBEMB}/format-inf.h
	${LIBEMB}/format-jef.c
	${LIBEMB}/format-jef.h
	${LIBEMB}/format-ksm.c
	${LIBEMB}/format-ksm.h
	${LIBEMB}/format-max.c
	${LIBEMB}/format-max.h
	${LIBEMB}/format-mit.c
	${LIBEMB}/format-mit.h
	${LIBEMB}/format-new.c
	${LIBEMB}/format-new.h
	${LIBEMB}/format-ofm.c
	${LIBEMB}/format-ofm.h
	${LIBEMB}/format-pcd.c
	${LIBEMB}/format-pcd.h
	${LIBEMB}/format-pcm.c
	${LIBEMB}/format-pcm.h
	${LIBEMB}/format-pcq.c
	${LIBEMB}/format-pcq.h
	${LIBEMB}/format-pcs.c
	${LIBEMB}/format-pcs.h
	${LIBEMB}/format-pec.c
	${LIBEMB}/format-pec.h
	${LIBEMB}/format-pel.c
	${LIBEMB}/format-pel.h
	${LIBEMB}/format-pem.c
	${LIBEMB}/format-pem.h
	${LIBEMB}/format-pes.c
	${LIBEMB}/format-pes.h
	${LIBEMB}/format-phb.c
	${LIBEMB}/format-phb.h
	${LIBEMB}/format-phc.c
	${LIBEMB}/format-phc.h
	${LIBEMB}/format-plt.c
	${LIBEMB}/format-plt.h
	${LIBEMB}/format-rgb.c
	${LIBEMB}/format-rgb.h
	${LIBEMB}/format-sew.c
	${LIBEMB}/format-sew.h
	${LIBEMB}/formats.h
	${LIBEMB}/format-shv.c
	${LIBEMB}/format-shv.h
	${LIBEMB}/format-sst.c
	${LIBEMB}/format-sst.h
	${LIBEMB}/format-stx.c
	${LIBEMB}/format-stx.h
	${LIBEMB}/format-svg.c
	${LIBEMB}/format-svg.h
	${LIBEMB}/format-t01.c
	${LIBEMB}/format-t01.h
	${LIBEMB}/format-t09.c
	${LIBEMB}/format-t09.h
	${LIBEMB}/format-tap.c
	${LIBEMB}/format-tap.h
	${LIBEMB}/format-thr.c
	${LIBEMB}/format-thr.h
	${LIBEMB}/format-txt.c
	${LIBEMB}/format-txt.h
	${LIBEMB}/format-u00.c
	${LIBEMB}/format-u00.h
	${LIBEMB}/format-u01.c
	${LIBEMB}/format-u01.h
	${LIBEMB}/format-vip.c
	${LIBEMB}/format-vip.h
	${LIBEMB}/format-vp3.c
	${LIBEMB}/format-vp3.h
	${LIBEMB}/format-xxx.c
	${LIBEMB}/format-xxx.h
	${LIBEMB}/format-zsk.c
	${LIBEMB}/format-zsk.h
	${LIBEMB}/geom-arc.c
	${LIBEMB}/geom-arc.h
	${LIBEMB}/geom-circle.c
	${LIBEMB}/geom-circle.h
	${LIBEMB}/geom-line.c
	${LIBEMB}/geom-line.h
	${LIBEMB}/hashtable.c
	${LIBEMB}/hashtable.h
	${LIBEMB}/helpers-binary.c
	${LIBEMB}/helpers-binary.h
	${LIBEMB}/helpers-misc.c
	${LIBEMB}/helpers-misc.h
	${LIBEMB}/helpers-unused.h
	${LIBEMB}/thread-color.c
	${LIBEMB}/thread-color.h

	${EXTERN}/tinyscheme/source/dynload.c
	${EXTERN}/tinyscheme/source/mod_json.c
	${EXTERN}/tinyscheme/source/scheme.c
)

# WARNING: This does NOT regenerate when the assets change, so you have to
# rerun cmake to ensure that this retriggers.
#
file(COPY "${CMAKE_SOURCE_DIR}/images"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/icons"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/commands"
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
    ${CMAKE_SOURCE_DIR}/libembroidery
	${CMAKE_SOURCE_DIR}/extern/libembroidery/src
	${CMAKE_SOURCE_DIR}/extern/tinyscheme/include
)

target_link_libraries(embroidermodder PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	${OPENGL_LIBRARIES}
)

set_target_properties(embroidermodder PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

if (WIN32)
set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder.exe)

else(WIN32)
target_link_libraries(embroidermodder PRIVATE m)
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

# The necessary licenses are included in installation, should the user
# not retain the code after installation and so this more resembles
# the NSIS style installation.
install(
	FILES
		${SRC_ASSETS}/fonts/source-code-pro-license.md
		${SRC_ASSETS}/fonts/source-code-pro-readme.md
		${SRC_ASSETS}/fonts/SourceCodePro-Regular.ttf
		${SRC_ASSETS}/fonts/source-sans-license.md
		${SRC_ASSETS}/fonts/source-sans-readme.md
		${SRC_ASSETS}/fonts/SourceSans3-Regular.ttf
	DESTINATION
		${ASSETS_DIR}/fonts
)

