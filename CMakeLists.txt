cmake_minimum_required(VERSION 3.16)

if (WIN32)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

endif()

project(embroidermodder VERSION 2.0.0 LANGUAGES C CXX)

include(config.cmake)

find_package(Qt6 REQUIRED COMPONENTS OpenGL Widgets PrintSupport)

if(LINUX)
else(LINUX)
qt_standard_project_setup()
endif(LINUX)

set(CMAKE_AUTOMOC ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# The makers of CMake advise that you do not glob source files.
#
# https://cmake.org/cmake/help/latest/command/file.html?highlight=glob#glob
#
# Since our ASSETS are essential for the program to run, we are going to
# give them the same treatment and list them explicitly here.
#

set(SRC_ASSETS ${CMAKE_SOURCE_DIR}/src)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_executable(embroidermodder2
    ${SRC_DIR}/cmdprompt.cpp
    ${SRC_DIR}/embdetails-dialog.cpp
    ${SRC_DIR}/embroidermodder.cpp
    ${SRC_DIR}/imagewidget.cpp
    ${SRC_DIR}/interface.cpp
    ${SRC_DIR}/layer-manager.cpp
    ${SRC_DIR}/mainwindow-menus.cpp
    ${SRC_DIR}/mainwindow-toolbars.cpp
    ${SRC_DIR}/mainwindow.cpp
    ${SRC_DIR}/mdiarea.cpp
    ${SRC_DIR}/mdiwindow.cpp
    ${SRC_DIR}/objects.cpp
    ${SRC_DIR}/preview-dialog.cpp
    ${SRC_DIR}/property-editor.cpp
    ${SRC_DIR}/selectbox.cpp
    ${SRC_DIR}/settings-dialog.cpp
    ${SRC_DIR}/statusbar.cpp
    ${SRC_DIR}/undo-commands.cpp
    ${SRC_DIR}/undo-editor.cpp
    ${SRC_DIR}/view.cpp

    ${SRC_DIR}/embroidermodder.h
    ${SRC_DIR}/data.h

    ${CMAKE_SOURCE_DIR}/extern/tomlc99/toml.c
)

add_subdirectory(extern/libembroidery)

include_directories(
	${CMAKE_SOURCE_DIR}/extern/tomlc99
	${CMAKE_SOURCE_DIR}/extern/libembroidery/src
	${CMAKE_SOURCE_DIR}/extern/libembroidery/src/stb
	${CMAKE_SOURCE_DIR}/extern/libembroidery/src/nanosvg
	${SRC_DIR}
)

set_target_properties(embroidermodder2 PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

target_link_libraries(embroidermodder2 PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::OpenGL
    embroidery_static)

if (WIN32)
set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder2.exe)

else(WIN32)
target_link_libraries(embroidermodder2 PRIVATE m)
set(EXEC ${CMAKE_SOURCE_DIR}/build/embroidermodder2)

endif()

set(INSTALL_DIR ".")

install(
	FILES
        ${EXEC}
		${SRC_ASSETS}/em2_config.toml
        ${CMAKE_SOURCE_DIR}/build/D3Dcompiler_47.dll
        ${CMAKE_SOURCE_DIR}/build/Qt6Core.dll
        ${CMAKE_SOURCE_DIR}/build/Qt6Gui.dll
        ${CMAKE_SOURCE_DIR}/build/Qt6Network.dll
        ${CMAKE_SOURCE_DIR}/build/Qt6PrintSupport.dll
        ${CMAKE_SOURCE_DIR}/build/Qt6Svg.dll
        ${CMAKE_SOURCE_DIR}/build/Qt6Widgets.dll
        ${CMAKE_SOURCE_DIR}/build/libgcc_s_seh-1.dll
        ${CMAKE_SOURCE_DIR}/build/libstdc++-6.dll
        ${CMAKE_SOURCE_DIR}/build/libwinpthread-1.dll
        ${CMAKE_SOURCE_DIR}/build/opengl32sw.dll

	DESTINATION
		${INSTALL_DIR}
)

install(
    FILES
        ${CMAKE_SOURCE_DIR}/build/networkinformation/qnetworklistmanager.dll
    DESTINATION
        ${INSTALL_DIR}/networkinformation
)

install(
    FILES
        ${CMAKE_SOURCE_DIR}/build/platforms/qwindows.dll
    DESTINATION
        ${INSTALL_DIR}/platforms
)

install(
    FILES
        ${CMAKE_SOURCE_DIR}/build/styles/qwindowsvistastyle.dll
    DESTINATION
        ${INSTALL_DIR}/styles
)

install(
    FILES
        ${CMAKE_SOURCE_DIR}/build/tls/qcertonlybackend.dll
        ${CMAKE_SOURCE_DIR}/build/tls/qopensslbackend.dll
        ${CMAKE_SOURCE_DIR}/build/tls/qschannelbackend.dll
    DESTINATION
        ${INSTALL_DIR}/tls
)

install(
    FILES
        ${CMAKE_SOURCE_DIR}/build/generic/qtuiotouchplugin.dll
    DESTINATION
        ${INSTALL_DIR}/generic
)

install(
    FILES
        ${CMAKE_SOURCE_DIR}/build/iconengines/qsvgicon.dll
    DESTINATION
        ${INSTALL_DIR}/iconengines
)

install(
    FILES
        ${CMAKE_SOURCE_DIR}/build/imageformats/qgif.dll
        ${CMAKE_SOURCE_DIR}/build/imageformats/qicns.dll
        ${CMAKE_SOURCE_DIR}/build/imageformats/qico.dll
        ${CMAKE_SOURCE_DIR}/build/imageformats/qjpeg.dll
        ${CMAKE_SOURCE_DIR}/build/imageformats/qsvg.dll
        ${CMAKE_SOURCE_DIR}/build/imageformats/qtga.dll
        ${CMAKE_SOURCE_DIR}/build/imageformats/qtiff.dll
        ${CMAKE_SOURCE_DIR}/build/imageformats/qwbmp.dll
        ${CMAKE_SOURCE_DIR}/build/imageformats/qwebp.dll
    DESTINATION
        ${INSTALL_DIR}/imageformats
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
		${INSTALL_DIR}/translations
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
		${INSTALL_DIR}/fonts
)

install(
    FILES
        ${SRC_ASSETS}/commands/about/about.ini
    DESTINATION
        ${INSTALL_DIR}/commands
)

install(
    FILES
        ${SRC_ASSETS}/images/texture-spirals.png
        ${SRC_ASSETS}/images/logo-spirals.png
        ${SRC_ASSETS}/images/did-you-know.png
        ${SRC_ASSETS}/images/logo_embroidermodder_2_0.png
        ${SRC_ASSETS}/images/about_embroidermodder.png
    DESTINATION
        ${INSTALL_DIR}/images
)

install(
    FILES
        ${SRC_ASSETS}/help/html/index.html
    DESTINATION
        ${INSTALL_DIR}/help
)

install(
	FILES
		${SRC_ASSETS}/designs/dolphin.toml
		${SRC_ASSETS}/designs/heart4.toml
		${SRC_ASSETS}/designs/heart5.toml
		${SRC_ASSETS}/designs/snowflake.toml
		${SRC_ASSETS}/designs/star.toml
		${SRC_ASSETS}/designs/treble_clef.toml
	DESTINATION
		${INSTALL_DIR}/shapes
)

install(
	FILES
		${SRC_ASSETS}/icons/default/about.png
		${SRC_ASSETS}/icons/default/aligntextleft.png
		${SRC_ASSETS}/icons/default/aligntextright.png
		${SRC_ASSETS}/icons/default/angulardimension.png
		${SRC_ASSETS}/icons/default/arc3points.png
		${SRC_ASSETS}/icons/default/arc.png
		${SRC_ASSETS}/icons/default/arccenterstartangle.png
		${SRC_ASSETS}/icons/default/arccenterstartend.png
		${SRC_ASSETS}/icons/default/arccenterstartlength.png
		${SRC_ASSETS}/icons/default/arccontinue.png
		${SRC_ASSETS}/icons/default/arcstartcenterangle.png
		${SRC_ASSETS}/icons/default/arcstartcenterend.png
		${SRC_ASSETS}/icons/default/arcstartcenterlength.png
		${SRC_ASSETS}/icons/default/arcstartendangle.png
		${SRC_ASSETS}/icons/default/arcstartenddirection.png
		${SRC_ASSETS}/icons/default/arcstartendradius.png
		${SRC_ASSETS}/icons/default/area.png
		${SRC_ASSETS}/icons/default/array.png
		${SRC_ASSETS}/icons/default/backview.png
		${SRC_ASSETS}/icons/default/baselinedimension.png
		${SRC_ASSETS}/icons/default/bean.png
		${SRC_ASSETS}/icons/default/blank.png
		${SRC_ASSETS}/icons/default/bottomview.png
		${SRC_ASSETS}/icons/default/boundary.png
		${SRC_ASSETS}/icons/default/break2points.png
		${SRC_ASSETS}/icons/default/breakatpoint.png
		${SRC_ASSETS}/icons/default/browser.png
		${SRC_ASSETS}/icons/default/camera.png
		${SRC_ASSETS}/icons/default/centermark.png
		${SRC_ASSETS}/icons/default/chamfer.png
		${SRC_ASSETS}/icons/default/changelog.png
		${SRC_ASSETS}/icons/default/check.png
		${SRC_ASSETS}/icons/default/circle2points.png
		${SRC_ASSETS}/icons/default/circle3points.png
		${SRC_ASSETS}/icons/default/circle.png
		${SRC_ASSETS}/icons/default/circlecenterdiameter.png
		${SRC_ASSETS}/icons/default/circlecenterradius.png
		${SRC_ASSETS}/icons/default/circletantanradius.png
		${SRC_ASSETS}/icons/default/circletantantan.png
		${SRC_ASSETS}/icons/default/cloud-2.png
		${SRC_ASSETS}/icons/default/cloud.png
		${SRC_ASSETS}/icons/default/colorblue.png
		${SRC_ASSETS}/icons/default/colorbyblock.png
		${SRC_ASSETS}/icons/default/colorbylayer.png
		${SRC_ASSETS}/icons/default/colorcyan.png
		${SRC_ASSETS}/icons/default/colorgreen.png
		${SRC_ASSETS}/icons/default/colormagenta.png
		${SRC_ASSETS}/icons/default/colorother.png
		${SRC_ASSETS}/icons/default/colorred.png
		${SRC_ASSETS}/icons/default/colorselector.png
		${SRC_ASSETS}/icons/default/colorwhite.png
		${SRC_ASSETS}/icons/default/coloryellow.png
		${SRC_ASSETS}/icons/default/constructionline.png
		${SRC_ASSETS}/icons/default/continuedimension.png
		${SRC_ASSETS}/icons/default/copy.png
		${SRC_ASSETS}/icons/default/copyobject.png
		${SRC_ASSETS}/icons/default/customize.png
		${SRC_ASSETS}/icons/default/customizekeyboard.png
		${SRC_ASSETS}/icons/default/customizemenus.png
		${SRC_ASSETS}/icons/default/customizetoolbars.png
		${SRC_ASSETS}/icons/default/cut.png
		${SRC_ASSETS}/icons/default/date.png
		${SRC_ASSETS}/icons/default/day.png
		${SRC_ASSETS}/icons/default/designdetails.png
		${SRC_ASSETS}/icons/default/designdetails.png
		${SRC_ASSETS}/icons/default/diameterdimension.png
		${SRC_ASSETS}/icons/default/dimensionedit.png
		${SRC_ASSETS}/icons/default/dimensionstyle.png
		${SRC_ASSETS}/icons/default/dimensiontextedit.png
		${SRC_ASSETS}/icons/default/dimensionupdate.png
		${SRC_ASSETS}/icons/default/distance.png
		${SRC_ASSETS}/icons/default/dolphin.png
		${SRC_ASSETS}/icons/default/donothing.png
		${SRC_ASSETS}/icons/default/donut-2.png
		${SRC_ASSETS}/icons/default/donut.png
		${SRC_ASSETS}/icons/default/drawing2.png
		${SRC_ASSETS}/icons/default/drawing.png
		${SRC_ASSETS}/icons/default/ellipsearc.png
		${SRC_ASSETS}/icons/default/ellipseaxisend.png
		${SRC_ASSETS}/icons/default/ellipse.png
		${SRC_ASSETS}/icons/default/ellipsecenter.png
#		${SRC_ASSETS}/icons/default/embroidermodder2.icns
#		${SRC_ASSETS}/icons/default/embroidermodder2.ico
		${SRC_ASSETS}/icons/default/escape.png
		${SRC_ASSETS}/icons/default/exit.png
		${SRC_ASSETS}/icons/default/explode.png
		${SRC_ASSETS}/icons/default/extend.png
		${SRC_ASSETS}/icons/default/fillet.png
		${SRC_ASSETS}/icons/default/findandreplace.png
		${SRC_ASSETS}/icons/default/freezealllayers.png
		${SRC_ASSETS}/icons/default/frontview.png
		${SRC_ASSETS}/icons/default/gridsettings.png
		${SRC_ASSETS}/icons/default/gridsnapsettings.png
		${SRC_ASSETS}/icons/default/hatch.png
		${SRC_ASSETS}/icons/default/heart-2.png
		${SRC_ASSETS}/icons/default/heart.png
		${SRC_ASSETS}/icons/default/help-2.png
		${SRC_ASSETS}/icons/default/help.png
		${SRC_ASSETS}/icons/default/hex.png
		${SRC_ASSETS}/icons/default/hidealllayers.png
		${SRC_ASSETS}/icons/default/icon128.png
		${SRC_ASSETS}/icons/default/icon16.png
		${SRC_ASSETS}/icons/default/icon24.png
		${SRC_ASSETS}/icons/default/icon32.png
		${SRC_ASSETS}/icons/default/icon48.png
		${SRC_ASSETS}/icons/default/icon64.png
		${SRC_ASSETS}/icons/default/inquiry.png
		${SRC_ASSETS}/icons/default/insertblock.png
		${SRC_ASSETS}/icons/default/join.png
		${SRC_ASSETS}/icons/default/justifytext.png
		${SRC_ASSETS}/icons/default/layerprevious.png
		${SRC_ASSETS}/icons/default/layers.png
		${SRC_ASSETS}/icons/default/layerselector.png
		${SRC_ASSETS}/icons/default/layertranslate.png
		${SRC_ASSETS}/icons/default/leftview.png
		${SRC_ASSETS}/icons/default/lengthen.png
		${SRC_ASSETS}/icons/default/lineardimension.png
		${SRC_ASSETS}/icons/default/line.png
		${SRC_ASSETS}/icons/default/linetypebyblock.png
		${SRC_ASSETS}/icons/default/linetypebylayer.png
		${SRC_ASSETS}/icons/default/linetypecenter.png
		${SRC_ASSETS}/icons/default/linetypecontinuous.png
		${SRC_ASSETS}/icons/default/linetypehidden.png
		${SRC_ASSETS}/icons/default/linetypeother.png
		${SRC_ASSETS}/icons/default/linetypeselector.png
		${SRC_ASSETS}/icons/default/lineweight01.png
		${SRC_ASSETS}/icons/default/lineweight02.png
		${SRC_ASSETS}/icons/default/lineweight03.png
		${SRC_ASSETS}/icons/default/lineweight04.png
		${SRC_ASSETS}/icons/default/lineweight05.png
		${SRC_ASSETS}/icons/default/lineweight06.png
		${SRC_ASSETS}/icons/default/lineweight07.png
		${SRC_ASSETS}/icons/default/lineweight08.png
		${SRC_ASSETS}/icons/default/lineweight09.png
		${SRC_ASSETS}/icons/default/lineweight10.png
		${SRC_ASSETS}/icons/default/lineweight11.png
		${SRC_ASSETS}/icons/default/lineweight12.png
		${SRC_ASSETS}/icons/default/lineweight13.png
		${SRC_ASSETS}/icons/default/lineweight14.png
		${SRC_ASSETS}/icons/default/lineweight15.png
		${SRC_ASSETS}/icons/default/lineweight16.png
		${SRC_ASSETS}/icons/default/lineweight17.png
		${SRC_ASSETS}/icons/default/lineweight18.png
		${SRC_ASSETS}/icons/default/lineweight19.png
		${SRC_ASSETS}/icons/default/lineweight20.png
		${SRC_ASSETS}/icons/default/lineweight21.png
		${SRC_ASSETS}/icons/default/lineweight22.png
		${SRC_ASSETS}/icons/default/lineweight23.png
		${SRC_ASSETS}/icons/default/lineweight24.png
		${SRC_ASSETS}/icons/default/lineweightbyblock.png
		${SRC_ASSETS}/icons/default/lineweightbylayer.png
		${SRC_ASSETS}/icons/default/lineweightdefault.png
		${SRC_ASSETS}/icons/default/lineweightselector.png
		${SRC_ASSETS}/icons/default/lineweightsettings.png
		${SRC_ASSETS}/icons/default/list.png
		${SRC_ASSETS}/icons/default/locatepoint.png
		${SRC_ASSETS}/icons/default/locator-snaptoapparentintersection.png
		${SRC_ASSETS}/icons/default/locator-snaptocenter.png
		${SRC_ASSETS}/icons/default/locator-snaptoendpoint.png
		${SRC_ASSETS}/icons/default/locator-snaptoextension.png
		${SRC_ASSETS}/icons/default/locator-snaptoinsert.png
		${SRC_ASSETS}/icons/default/locator-snaptointersection.png
		${SRC_ASSETS}/icons/default/locator-snaptomidpoint.png
		${SRC_ASSETS}/icons/default/locator-snaptonearest.png
		${SRC_ASSETS}/icons/default/locator-snaptonode.png
		${SRC_ASSETS}/icons/default/locator-snaptoparallel.png
		${SRC_ASSETS}/icons/default/locator-snaptoperpendicular.png
		${SRC_ASSETS}/icons/default/locator-snaptoquadrant.png
		${SRC_ASSETS}/icons/default/locator-snaptotangent.png
		${SRC_ASSETS}/icons/default/lockalllayers.png
		${SRC_ASSETS}/icons/default/makeblock.png
		${SRC_ASSETS}/icons/default/makelayercurrent.png
		${SRC_ASSETS}/icons/default/mass.png
		${SRC_ASSETS}/icons/default/mirror.png
		${SRC_ASSETS}/icons/default/move.png
		${SRC_ASSETS}/icons/default/multiline.png
		${SRC_ASSETS}/icons/default/multilinetext.png
		${SRC_ASSETS}/icons/default/namedviews.png
		${SRC_ASSETS}/icons/default/neisometricview.png
		${SRC_ASSETS}/icons/default/new.png
		${SRC_ASSETS}/icons/default/night.png
		${SRC_ASSETS}/icons/default/nopreview.png
		${SRC_ASSETS}/icons/default/nwisometricview.png
		${SRC_ASSETS}/icons/default/obliquedimensions.png
		${SRC_ASSETS}/icons/default/offset.png
		${SRC_ASSETS}/icons/default/open.png
		${SRC_ASSETS}/icons/default/ordinatedimension.png
		${SRC_ASSETS}/icons/default/orthosettings.png
		${SRC_ASSETS}/icons/default/pan.png
		${SRC_ASSETS}/icons/default/pandown.png
		${SRC_ASSETS}/icons/default/panleft.png
		${SRC_ASSETS}/icons/default/panpoint.png
		${SRC_ASSETS}/icons/default/panrealtime.png
		${SRC_ASSETS}/icons/default/panright.png
		${SRC_ASSETS}/icons/default/panup.png
		${SRC_ASSETS}/icons/default/paste.png
		${SRC_ASSETS}/icons/default/path.png
		${SRC_ASSETS}/icons/default/pickadd.png
		${SRC_ASSETS}/icons/default/picknew.png
		${SRC_ASSETS}/icons/default/plugin.png
		${SRC_ASSETS}/icons/default/point.png
		${SRC_ASSETS}/icons/default/pointdivide.png
		${SRC_ASSETS}/icons/default/pointmeasure.png
		${SRC_ASSETS}/icons/default/pointmultiple.png
		${SRC_ASSETS}/icons/default/pointsingle.png
		${SRC_ASSETS}/icons/default/polarsettings.png
		${SRC_ASSETS}/icons/default/polygon.png
		${SRC_ASSETS}/icons/default/polyline.png
		${SRC_ASSETS}/icons/default/print.png
		${SRC_ASSETS}/icons/default/pyscript.png
		${SRC_ASSETS}/icons/default/qsnapsettings.png
		${SRC_ASSETS}/icons/default/qtracksettings.png
		${SRC_ASSETS}/icons/default/quickdimension.png
		${SRC_ASSETS}/icons/default/quickleader.png
		${SRC_ASSETS}/icons/default/quickselect.png
		${SRC_ASSETS}/icons/default/radiusdimension.png
		${SRC_ASSETS}/icons/default/ray.png
		${SRC_ASSETS}/icons/default/rectangle.png
		${SRC_ASSETS}/icons/default/redo.png
		${SRC_ASSETS}/icons/default/region.png
		${SRC_ASSETS}/icons/default/render.png
		${SRC_ASSETS}/icons/default/rgb.png
		${SRC_ASSETS}/icons/default/rightview.png
		${SRC_ASSETS}/icons/default/rotate.png
		${SRC_ASSETS}/icons/default/rulersettings.png
		${SRC_ASSETS}/icons/default/sandbox.png
		${SRC_ASSETS}/icons/default/satin.png
		${SRC_ASSETS}/icons/default/saveas.png
		${SRC_ASSETS}/icons/default/save.png
		${SRC_ASSETS}/icons/default/scale.png
		${SRC_ASSETS}/icons/default/seisometricview.png
		${SRC_ASSETS}/icons/default/settingsdialog-2.png
		${SRC_ASSETS}/icons/default/settingsdialog.png
		${SRC_ASSETS}/icons/default/shade2dwireframe.png
		${SRC_ASSETS}/icons/default/shade3dwireframe.png
		${SRC_ASSETS}/icons/default/shade.png
		${SRC_ASSETS}/icons/default/shadeflat.png
		${SRC_ASSETS}/icons/default/shadeflatedges.png
		${SRC_ASSETS}/icons/default/shadehidden.png
		${SRC_ASSETS}/icons/default/shadesmooth.png
		${SRC_ASSETS}/icons/default/shadesmoothedges.png
		${SRC_ASSETS}/icons/default/showalllayers.png
		${SRC_ASSETS}/icons/default/singlelinetext.png
		${SRC_ASSETS}/icons/default/sketch-2.png
		${SRC_ASSETS}/icons/default/sketch.png
		${SRC_ASSETS}/icons/default/snapfrom.png
		${SRC_ASSETS}/icons/default/snaptoapparentintersection.png
		${SRC_ASSETS}/icons/default/snaptocenter.png
		${SRC_ASSETS}/icons/default/snaptoendpoint.png
		${SRC_ASSETS}/icons/default/snaptoextension.png
		${SRC_ASSETS}/icons/default/snaptoinsert.png
		${SRC_ASSETS}/icons/default/snaptointersection.png
		${SRC_ASSETS}/icons/default/snaptomidpoint.png
		${SRC_ASSETS}/icons/default/snaptonearest.png
		${SRC_ASSETS}/icons/default/snaptonode.png
		${SRC_ASSETS}/icons/default/snaptonone.png
		${SRC_ASSETS}/icons/default/snaptoparallel.png
		${SRC_ASSETS}/icons/default/snaptoperpendicular.png
		${SRC_ASSETS}/icons/default/snaptoquadrant.png
		${SRC_ASSETS}/icons/default/snaptotangent.png
		${SRC_ASSETS}/icons/default/snowflake-2.png
		${SRC_ASSETS}/icons/default/snowflake.png
		${SRC_ASSETS}/icons/default/solidbox.png
		${SRC_ASSETS}/icons/default/solidcheck.png
		${SRC_ASSETS}/icons/default/solidclean.png
		${SRC_ASSETS}/icons/default/solidcoloredges.png
		${SRC_ASSETS}/icons/default/solidcolorfaces.png
		${SRC_ASSETS}/icons/default/solidcone.png
		${SRC_ASSETS}/icons/default/solidcopyedges.png
		${SRC_ASSETS}/icons/default/solidcopyfaces.png
		${SRC_ASSETS}/icons/default/solidcylinder.png
		${SRC_ASSETS}/icons/default/soliddeletefaces.png
		${SRC_ASSETS}/icons/default/solidextrude.png
		${SRC_ASSETS}/icons/default/solidextrudefaces.png
		${SRC_ASSETS}/icons/default/solidimprint.png
		${SRC_ASSETS}/icons/default/solidinterfere.png
		${SRC_ASSETS}/icons/default/solidintersect.png
		${SRC_ASSETS}/icons/default/solidmovefaces.png
		${SRC_ASSETS}/icons/default/solidoffsetfaces.png
		${SRC_ASSETS}/icons/default/solidrevolve.png
		${SRC_ASSETS}/icons/default/solidrotatefaces.png
		${SRC_ASSETS}/icons/default/solids.png
		${SRC_ASSETS}/icons/default/solidsection.png
		${SRC_ASSETS}/icons/default/solidsediting.png
		${SRC_ASSETS}/icons/default/solidseparate.png
		${SRC_ASSETS}/icons/default/solidsetup.png
		${SRC_ASSETS}/icons/default/solidsetupdrawing.png
		${SRC_ASSETS}/icons/default/solidsetupprofile.png
		${SRC_ASSETS}/icons/default/solidsetupview.png
		${SRC_ASSETS}/icons/default/solidshell.png
		${SRC_ASSETS}/icons/default/solidslice.png
		${SRC_ASSETS}/icons/default/solidsphere.png
		${SRC_ASSETS}/icons/default/solidsubtract.png
		${SRC_ASSETS}/icons/default/solidtaperfaces.png
		${SRC_ASSETS}/icons/default/solidtorus.png
		${SRC_ASSETS}/icons/default/solidunion.png
		${SRC_ASSETS}/icons/default/solidwedge.png
		${SRC_ASSETS}/icons/default/spline.png
		${SRC_ASSETS}/icons/default/star.png
		${SRC_ASSETS}/icons/default/stretch.png
		${SRC_ASSETS}/icons/default/stub.png
		${SRC_ASSETS}/icons/default/surface2dsolid.png
		${SRC_ASSETS}/icons/default/surface3dface.png
		${SRC_ASSETS}/icons/default/surface3dmesh.png
		${SRC_ASSETS}/icons/default/surfacebox.png
		${SRC_ASSETS}/icons/default/surfacecone.png
		${SRC_ASSETS}/icons/default/surfacecylinder.png
		${SRC_ASSETS}/icons/default/surfacedish.png
		${SRC_ASSETS}/icons/default/surfacedome.png
		${SRC_ASSETS}/icons/default/surfaceedge.png
		${SRC_ASSETS}/icons/default/surfaceedgesurface.png
		${SRC_ASSETS}/icons/default/surfacepyramid.png
		${SRC_ASSETS}/icons/default/surfacerevolvedsurface.png
		${SRC_ASSETS}/icons/default/surfaceruledsurface.png
		${SRC_ASSETS}/icons/default/surfaces.png
		${SRC_ASSETS}/icons/default/surfacesphere.png
		${SRC_ASSETS}/icons/default/surfacetabulatedsurface.png
		${SRC_ASSETS}/icons/default/surfacetorus.png
		${SRC_ASSETS}/icons/default/surfacewedge.png
		${SRC_ASSETS}/icons/default/swisometricview.png
		${SRC_ASSETS}/icons/default/temptrackingpoint.png
		${SRC_ASSETS}/icons/default/text.png
		${SRC_ASSETS}/icons/default/textbold.png
		${SRC_ASSETS}/icons/default/textitalic.png
		${SRC_ASSETS}/icons/default/textoverline.png
		${SRC_ASSETS}/icons/default/textstrikeout.png
		${SRC_ASSETS}/icons/default/textunderline.png
		${SRC_ASSETS}/icons/default/thawalllayers.png
		${SRC_ASSETS}/icons/default/theme.png
		${SRC_ASSETS}/icons/default/tipoftheday.png
		${SRC_ASSETS}/icons/default/tolerance.png
		${SRC_ASSETS}/icons/default/topview.png
		${SRC_ASSETS}/icons/default/trim.png
		${SRC_ASSETS}/icons/default/undo.png
		${SRC_ASSETS}/icons/default/units.png
		${SRC_ASSETS}/icons/default/unlockalllayers.png
		${SRC_ASSETS}/icons/default/view.png
		${SRC_ASSETS}/icons/default/whatsthis.png
		${SRC_ASSETS}/icons/default/wideflange.png
		${SRC_ASSETS}/icons/default/windowcascade.png
		${SRC_ASSETS}/icons/default/windowcloseall.png
		${SRC_ASSETS}/icons/default/windowclose.png
		${SRC_ASSETS}/icons/default/windownext.png
		${SRC_ASSETS}/icons/default/windowprevious.png
		${SRC_ASSETS}/icons/default/windowtile.png
		${SRC_ASSETS}/icons/default/world.png
		${SRC_ASSETS}/icons/default/zoomall.png
		${SRC_ASSETS}/icons/default/zoom.png
		${SRC_ASSETS}/icons/default/zoomcenter.png
		${SRC_ASSETS}/icons/default/zoomdynamic.png
		${SRC_ASSETS}/icons/default/zoomextents.png
		${SRC_ASSETS}/icons/default/zoomin.png
		${SRC_ASSETS}/icons/default/zoomout.png
		${SRC_ASSETS}/icons/default/zoomprevious.png
		${SRC_ASSETS}/icons/default/zoomrealtime.png
		${SRC_ASSETS}/icons/default/zoomscale.png
		${SRC_ASSETS}/icons/default/zoomselected.png
		${SRC_ASSETS}/icons/default/zoomwindow.png
	DESTINATION
		${INSTALL_DIR}/icons/default
)

# install(
	# FILES
		# extern/libembroidery/assets/arc_polyester_colors.csv
		# extern/libembroidery/assets/pantone_colors.csv
		# extern/libembroidery/assets/arc_rayon_colors.csv
		# extern/libembroidery/assets/pcm_colors.csv
		# extern/libembroidery/assets/coats_and_clark_rayon_colors.csv
		# extern/libembroidery/assets/pec_colors.csv
		# extern/libembroidery/assets/dxf_color_table.csv
		# extern/libembroidery/assets/robison_anton_colors.csv
		# extern/libembroidery/assets/exquisite_polyester_colors.csv
		# extern/libembroidery/assets/robison_anton_rayon_colors.csv
		# extern/libembroidery/assets/fufu_polyester_colors.csv
		# extern/libembroidery/assets/shv_colors.csv
		# extern/libembroidery/assets/fufu_rayon_colors.csv
		# extern/libembroidery/assets/sigma_polyester_colors.csv
		# extern/libembroidery/assets/header.csv
		# extern/libembroidery/assets/sulky_rayon_colors.csv
		# extern/libembroidery/assets/hemingworth_polyester_colors.csv
		# extern/libembroidery/assets/svg_attribute_table.csv
		# extern/libembroidery/assets/hus_colors.csv
		# extern/libembroidery/assets/svg_colors.csv
		# extern/libembroidery/assets/isacord_polyester_colors.csv
		# extern/libembroidery/assets/svg_element_tokens.csv
		# extern/libembroidery/assets/isafil_rayon_colors.csv
		# extern/libembroidery/assets/svg_media_property_tokens.csv
		# extern/libembroidery/assets/jef_colors.csv
		# extern/libembroidery/assets/svg_property_tokens.csv
		# extern/libembroidery/assets/madeira_polyester_colors.csv
		# extern/libembroidery/assets/thread_art_polyester_colors.csv
		# extern/libembroidery/assets/madeira_rayon_colors.csv
		# extern/libembroidery/assets/thread_art_rayon_colors.csv
		# extern/libembroidery/assets/marathon_polyester_colors.csv
		# extern/libembroidery/assets/threadelight_polyester_colors.csv
		# extern/libembroidery/assets/marathon_rayon_colors.csv
		# extern/libembroidery/assets/z102_isacord_polyester_colors.csv
		# extern/libembroidery/assets/metro_polyester_colors.csv
	# DESTINATION
		# ${INSTALL_DIR}/thread-colors
# )

include(cpack.cmake)
