
TEMPLATE = app
DESTDIR = build

INCLUDEPATH += extern/libembroidery/src
INCLUDEPATH += extern/tomlc99

QT += core gui widgets printsupport opengl
HEADERS += src/embroidermodder.h \
    extern/libembroidery/embroidery.h \
    extern/tomlc99/toml.h \
    src/core.h
SOURCES += \
    extern/libembroidery/embroidery.c \
    src/object_data.c \
    src/core.c \
    src/action_table.c \
    src/editor_table.c \
    src/interface.cpp \
    src/objects.cpp \
    src/settings-dialog.cpp \
    src/view.cpp \
    src/mainwindow.cpp

#CONFIG += qt debug application

#win32 {
#}
#unix {
#}

RESOURCES += \
    assets/assets.qrc

DISTFILES += \
    assets/Info.plist \
    assets/commands/dolphin.toml \
    assets/commands/heart4.toml \
    assets/commands/heart5.toml \
    assets/commands/snowflake.toml \
    assets/commands/star.toml \
    assets/commands/treble_clef.toml \
    assets/config.toml \
    assets/embroidermodder2.icns \
    assets/embroidermodder2.ico \
    assets/fonts/SourceCodePro-Regular.ttf \
    assets/fonts/SourceSans3-Regular.ttf \
    assets/fonts/source-code-pro-license.md \
    assets/fonts/source-code-pro-readme.md \
    assets/fonts/source-sans-license.md \
    assets/fonts/source-sans-readme.md \
    assets/icons/default/3dviews.png \
    assets/icons/default/about.png \
    assets/icons/default/aligneddimension.png \
    assets/icons/default/aligntext.png \
    assets/icons/default/aligntextangle.png \
    assets/icons/default/aligntextcenter.png \
    assets/icons/default/aligntexthome.png \
    assets/icons/default/aligntextleft.png \
    assets/icons/default/aligntextright.png \
    assets/icons/default/angulardimension.png \
    assets/icons/default/app.png \
    assets/icons/default/arc.png \
    assets/icons/default/arc3points.png \
    assets/icons/default/arccenterstartangle.png \
    assets/icons/default/arccenterstartend.png \
    assets/icons/default/arccenterstartlength.png \
    assets/icons/default/arccontinue.png \
    assets/icons/default/arcstartcenterangle.png \
    assets/icons/default/arcstartcenterend.png \
    assets/icons/default/arcstartcenterlength.png \
    assets/icons/default/arcstartendangle.png \
    assets/icons/default/arcstartenddirection.png \
    assets/icons/default/arcstartendradius.png \
    assets/icons/default/area.png \
    assets/icons/default/array.png \
    assets/icons/default/backview.png \
    assets/icons/default/baselinedimension.png \
    assets/icons/default/bean.png \
    assets/icons/default/blank.png \
    assets/icons/default/bottomview.png \
    assets/icons/default/boundary.png \
    assets/icons/default/break2points.png \
    assets/icons/default/breakatpoint.png \
    assets/icons/default/browser.png \
    assets/icons/default/camera.png \
    assets/icons/default/centermark.png \
    assets/icons/default/chamfer.png \
    assets/icons/default/changelog.png \
    assets/icons/default/check.png \
    assets/icons/default/circle.png \
    assets/icons/default/circle2points.png \
    assets/icons/default/circle3points.png \
    assets/icons/default/circlecenterdiameter.png \
    assets/icons/default/circlecenterradius.png \
    assets/icons/default/circletantanradius.png \
    assets/icons/default/circletantantan.png \
    assets/icons/default/cloud-2.png \
    assets/icons/default/cloud.png \
    assets/icons/default/colorblue.png \
    assets/icons/default/colorbyblock.png \
    assets/icons/default/colorbylayer.png \
    assets/icons/default/colorcyan.png \
    assets/icons/default/colorgreen.png \
    assets/icons/default/colormagenta.png \
    assets/icons/default/colorother.png \
    assets/icons/default/colorred.png \
    assets/icons/default/colorselector.png \
    assets/icons/default/colorwhite.png \
    assets/icons/default/coloryellow.png \
    assets/icons/default/constructionline.png \
    assets/icons/default/continuedimension.png \
    assets/icons/default/copy.png \
    assets/icons/default/copyobject.png \
    assets/icons/default/customize.png \
    assets/icons/default/customizekeyboard.png \
    assets/icons/default/customizemenus.png \
    assets/icons/default/customizetoolbars.png \
    assets/icons/default/cut.png \
    assets/icons/default/date.png \
    assets/icons/default/day.png \
    assets/icons/default/designdetails.png \
    assets/icons/default/diameterdimension.png \
    assets/icons/default/dimensionedit.png \
    assets/icons/default/dimensionstyle.png \
    assets/icons/default/dimensiontextedit.png \
    assets/icons/default/dimensionupdate.png \
    assets/icons/default/distance.png \
    assets/icons/default/dolphin.png \
    assets/icons/default/donothing.png \
    assets/icons/default/donut-2.png \
    assets/icons/default/donut.png \
    assets/icons/default/drawing.png \
    assets/icons/default/drawing2.png \
    assets/icons/default/ellipse.png \
    assets/icons/default/ellipsearc.png \
    assets/icons/default/ellipseaxisend.png \
    assets/icons/default/ellipsecenter.png \
    assets/icons/default/erase.png \
    assets/icons/default/escape.png \
    assets/icons/default/exit.png \
    assets/icons/default/explode.png \
    assets/icons/default/extend.png \
    assets/icons/default/fillet.png \
    assets/icons/default/findandreplace.png \
    assets/icons/default/freezealllayers.png \
    assets/icons/default/frontview.png \
    assets/icons/default/gridsettings.png \
    assets/icons/default/gridsnapsettings.png \
    assets/icons/default/hatch.png \
    assets/icons/default/heart-2.png \
    assets/icons/default/heart.png \
    assets/icons/default/help-2.png \
    assets/icons/default/help.png \
    assets/icons/default/hex.png \
    assets/icons/default/hidealllayers.png \
    assets/icons/default/histogram.png \
    assets/icons/default/icon128.png \
    assets/icons/default/icon16.png \
    assets/icons/default/icon24.png \
    assets/icons/default/icon32.png \
    assets/icons/default/icon48.png \
    assets/icons/default/icon64.png \
    assets/icons/default/inquiry.png \
    assets/icons/default/insertblock.png \
    assets/icons/default/join.png \
    assets/icons/default/justifytext.png \
    assets/icons/default/layerprevious.png \
    assets/icons/default/layers.png \
    assets/icons/default/layerselector.png \
    assets/icons/default/layertranslate.png \
    assets/icons/default/leftview.png \
    assets/icons/default/lengthen.png \
    assets/icons/default/line.png \
    assets/icons/default/lineardimension.png \
    assets/icons/default/linetypebyblock.png \
    assets/icons/default/linetypebylayer.png \
    assets/icons/default/linetypecenter.png \
    assets/icons/default/linetypecontinuous.png \
    assets/icons/default/linetypehidden.png \
    assets/icons/default/linetypeother.png \
    assets/icons/default/linetypeselector.png \
    assets/icons/default/lineweight01.png \
    assets/icons/default/lineweight02.png \
    assets/icons/default/lineweight03.png \
    assets/icons/default/lineweight04.png \
    assets/icons/default/lineweight05.png \
    assets/icons/default/lineweight06.png \
    assets/icons/default/lineweight07.png \
    assets/icons/default/lineweight08.png \
    assets/icons/default/lineweight09.png \
    assets/icons/default/lineweight10.png \
    assets/icons/default/lineweight11.png \
    assets/icons/default/lineweight12.png \
    assets/icons/default/lineweight13.png \
    assets/icons/default/lineweight14.png \
    assets/icons/default/lineweight15.png \
    assets/icons/default/lineweight16.png \
    assets/icons/default/lineweight17.png \
    assets/icons/default/lineweight18.png \
    assets/icons/default/lineweight19.png \
    assets/icons/default/lineweight20.png \
    assets/icons/default/lineweight21.png \
    assets/icons/default/lineweight22.png \
    assets/icons/default/lineweight23.png \
    assets/icons/default/lineweight24.png \
    assets/icons/default/lineweightbyblock.png \
    assets/icons/default/lineweightbylayer.png \
    assets/icons/default/lineweightdefault.png \
    assets/icons/default/lineweightselector.png \
    assets/icons/default/lineweightsettings.png \
    assets/icons/default/list.png \
    assets/icons/default/locatepoint.png \
    assets/icons/default/locator-snaptoapparentintersection.png \
    assets/icons/default/locator-snaptocenter.png \
    assets/icons/default/locator-snaptoendpoint.png \
    assets/icons/default/locator-snaptoextension.png \
    assets/icons/default/locator-snaptoinsert.png \
    assets/icons/default/locator-snaptointersection.png \
    assets/icons/default/locator-snaptomidpoint.png \
    assets/icons/default/locator-snaptonearest.png \
    assets/icons/default/locator-snaptonode.png \
    assets/icons/default/locator-snaptoparallel.png \
    assets/icons/default/locator-snaptoperpendicular.png \
    assets/icons/default/locator-snaptoquadrant.png \
    assets/icons/default/locator-snaptotangent.png \
    assets/icons/default/lockalllayers.png \
    assets/icons/default/makeblock.png \
    assets/icons/default/makelayercurrent.png \
    assets/icons/default/mass.png \
    assets/icons/default/mirror.png \
    assets/icons/default/move.png \
    assets/icons/default/multiline.png \
    assets/icons/default/multilinetext.png \
    assets/icons/default/namedviews.png \
    assets/icons/default/neisometricview.png \
    assets/icons/default/new.png \
    assets/icons/default/night.png \
    assets/icons/default/nopreview.png \
    assets/icons/default/nwisometricview.png \
    assets/icons/default/obliquedimensions.png \
    assets/icons/default/offset.png \
    assets/icons/default/open.png \
    assets/icons/default/ordinatedimension.png \
    assets/icons/default/orthosettings.png \
    assets/icons/default/pan.png \
    assets/icons/default/pandown.png \
    assets/icons/default/panleft.png \
    assets/icons/default/panpoint.png \
    assets/icons/default/panrealtime.png \
    assets/icons/default/panright.png \
    assets/icons/default/panup.png \
    assets/icons/default/paste.png \
    assets/icons/default/path.png \
    assets/icons/default/pickadd.png \
    assets/icons/default/picknew.png \
    assets/icons/default/plugin.png \
    assets/icons/default/point.png \
    assets/icons/default/pointdivide.png \
    assets/icons/default/pointmeasure.png \
    assets/icons/default/pointmultiple.png \
    assets/icons/default/pointsingle.png \
    assets/icons/default/polarsettings.png \
    assets/icons/default/polygon.png \
    assets/icons/default/polyline.png \
    assets/icons/default/print.png \
    assets/icons/default/pyscript.png \
    assets/icons/default/qsnapsettings.png \
    assets/icons/default/qtracksettings.png \
    assets/icons/default/quickdimension.png \
    assets/icons/default/quickleader.png \
    assets/icons/default/quickselect.png \
    assets/icons/default/radiusdimension.png \
    assets/icons/default/ray.png \
    assets/icons/default/rectangle.png \
    assets/icons/default/redo.png \
    assets/icons/default/region.png \
    assets/icons/default/render.png \
    assets/icons/default/rgb.png \
    assets/icons/default/rightview.png \
    assets/icons/default/rotate.png \
    assets/icons/default/rulersettings.png \
    assets/icons/default/sandbox.png \
    assets/icons/default/satin.png \
    assets/icons/default/save.png \
    assets/icons/default/saveas.png \
    assets/icons/default/scale.png \
    assets/icons/default/seisometricview.png \
    assets/icons/default/settingsdialog-2.png \
    assets/icons/default/settingsdialog.png \
    assets/icons/default/shade.png \
    assets/icons/default/shade2dwireframe.png \
    assets/icons/default/shade3dwireframe.png \
    assets/icons/default/shadeflat.png \
    assets/icons/default/shadeflatedges.png \
    assets/icons/default/shadehidden.png \
    assets/icons/default/shadesmooth.png \
    assets/icons/default/shadesmoothedges.png \
    assets/icons/default/showalllayers.png \
    assets/icons/default/singlelinetext.png \
    assets/icons/default/sketch-2.png \
    assets/icons/default/sketch.png \
    assets/icons/default/snapfrom.png \
    assets/icons/default/snaptoapparentintersection.png \
    assets/icons/default/snaptocenter.png \
    assets/icons/default/snaptoendpoint.png \
    assets/icons/default/snaptoextension.png \
    assets/icons/default/snaptoinsert.png \
    assets/icons/default/snaptointersection.png \
    assets/icons/default/snaptomidpoint.png \
    assets/icons/default/snaptonearest.png \
    assets/icons/default/snaptonode.png \
    assets/icons/default/snaptonone.png \
    assets/icons/default/snaptoparallel.png \
    assets/icons/default/snaptoperpendicular.png \
    assets/icons/default/snaptoquadrant.png \
    assets/icons/default/snaptotangent.png \
    assets/icons/default/snowflake-2.png \
    assets/icons/default/snowflake.png \
    assets/icons/default/solidbox.png \
    assets/icons/default/solidcheck.png \
    assets/icons/default/solidclean.png \
    assets/icons/default/solidcoloredges.png \
    assets/icons/default/solidcolorfaces.png \
    assets/icons/default/solidcone.png \
    assets/icons/default/solidcopyedges.png \
    assets/icons/default/solidcopyfaces.png \
    assets/icons/default/solidcylinder.png \
    assets/icons/default/soliddeletefaces.png \
    assets/icons/default/solidextrude.png \
    assets/icons/default/solidextrudefaces.png \
    assets/icons/default/solidimprint.png \
    assets/icons/default/solidinterfere.png \
    assets/icons/default/solidintersect.png \
    assets/icons/default/solidmovefaces.png \
    assets/icons/default/solidoffsetfaces.png \
    assets/icons/default/solidrevolve.png \
    assets/icons/default/solidrotatefaces.png \
    assets/icons/default/solids.png \
    assets/icons/default/solidsection.png \
    assets/icons/default/solidsediting.png \
    assets/icons/default/solidseparate.png \
    assets/icons/default/solidsetup.png \
    assets/icons/default/solidsetupdrawing.png \
    assets/icons/default/solidsetupprofile.png \
    assets/icons/default/solidsetupview.png \
    assets/icons/default/solidshell.png \
    assets/icons/default/solidslice.png \
    assets/icons/default/solidsphere.png \
    assets/icons/default/solidsubtract.png \
    assets/icons/default/solidtaperfaces.png \
    assets/icons/default/solidtorus.png \
    assets/icons/default/solidunion.png \
    assets/icons/default/solidwedge.png \
    assets/icons/default/spline.png \
    assets/icons/default/star.png \
    assets/icons/default/stretch.png \
    assets/icons/default/stub.png \
    assets/icons/default/surface2dsolid.png \
    assets/icons/default/surface3dface.png \
    assets/icons/default/surface3dmesh.png \
    assets/icons/default/surfacebox.png \
    assets/icons/default/surfacecone.png \
    assets/icons/default/surfacecylinder.png \
    assets/icons/default/surfacedish.png \
    assets/icons/default/surfacedome.png \
    assets/icons/default/surfaceedge.png \
    assets/icons/default/surfaceedgesurface.png \
    assets/icons/default/surfacepyramid.png \
    assets/icons/default/surfacerevolvedsurface.png \
    assets/icons/default/surfaceruledsurface.png \
    assets/icons/default/surfaces.png \
    assets/icons/default/surfacesphere.png \
    assets/icons/default/surfacetabulatedsurface.png \
    assets/icons/default/surfacetorus.png \
    assets/icons/default/surfacewedge.png \
    assets/icons/default/swisometricview.png \
    assets/icons/default/temptrackingpoint.png \
    assets/icons/default/text.png \
    assets/icons/default/textbold.png \
    assets/icons/default/textitalic.png \
    assets/icons/default/textoverline.png \
    assets/icons/default/textstrikeout.png \
    assets/icons/default/textunderline.png \
    assets/icons/default/thawalllayers.png \
    assets/icons/default/theme.png \
    assets/icons/default/tipoftheday-2.png \
    assets/icons/default/tipoftheday.png \
    assets/icons/default/tolerance.png \
    assets/icons/default/topview.png \
    assets/icons/default/trim.png \
    assets/icons/default/undo.png \
    assets/icons/default/units.png \
    assets/icons/default/unlockalllayers.png \
    assets/icons/default/view.png \
    assets/icons/default/whatsthis.png \
    assets/icons/default/wideflange.png \
    assets/icons/default/windowcascade.png \
    assets/icons/default/windowclose.png \
    assets/icons/default/windowcloseall.png \
    assets/icons/default/windownext.png \
    assets/icons/default/windowprevious.png \
    assets/icons/default/windowtile.png \
    assets/icons/default/world.png \
    assets/icons/default/zoom.png \
    assets/icons/default/zoomall.png \
    assets/icons/default/zoomcenter.png \
    assets/icons/default/zoomdynamic.png \
    assets/icons/default/zoomextents.png \
    assets/icons/default/zoomin.png \
    assets/icons/default/zoomout.png \
    assets/icons/default/zoomprevious.png \
    assets/icons/default/zoomrealtime.png \
    assets/icons/default/zoomscale.png \
    assets/icons/default/zoomselected.png \
    assets/icons/default/zoomwindow.png \
    assets/icons/test1/theme.png \
    assets/icons/test2/theme.png \
    assets/images/text_about.png \
    assets/images/texture-canvas.png \
    assets/images/texture-spirals-canvas.png \
    assets/images/texture-spirals.png \
    assets/translations/afrikaans.toml \
    assets/translations/afrikaans/commands_afrikaans.qm \
    assets/translations/afrikaans/commands_afrikaans.ts \
    assets/translations/afrikaans/embroidermodder2_afrikaans.qm \
    assets/translations/afrikaans/embroidermodder2_afrikaans.ts \
    assets/translations/arabic.toml \
    assets/translations/arabic/commands_arabic.ts \
    assets/translations/arabic/embroidermodder2_arabic.ts \
    assets/translations/chinese/commands_chinese.qm \
    assets/translations/chinese/commands_chinese.ts \
    assets/translations/chinese/embroidermodder2_chinese.qm \
    assets/translations/chinese/embroidermodder2_chinese.ts \
    assets/translations/chinese_simplified.toml \
    assets/translations/chinese_traditional.toml \
    assets/translations/czech.toml \
    assets/translations/czech/commands_czech.qm \
    assets/translations/czech/commands_czech.ts \
    assets/translations/czech/embroidermodder2_czech.qm \
    assets/translations/czech/embroidermodder2_czech.ts \
    assets/translations/danish.toml \
    assets/translations/danish/commands_danish.ts \
    assets/translations/danish/embroidermodder2_danish.ts \
    assets/translations/dutch.toml \
    assets/translations/dutch/commands_dutch.qm \
    assets/translations/dutch/commands_dutch.ts \
    assets/translations/dutch/embroidermodder2_dutch.qm \
    assets/translations/dutch/embroidermodder2_dutch.ts \
    assets/translations/english/commands_english.qm \
    assets/translations/english/commands_english.ts \
    assets/translations/english/embroidermodder2_english.qm \
    assets/translations/english/embroidermodder2_english.ts \
    assets/translations/english_gb.toml \
    assets/translations/finnish.toml \
    assets/translations/finnish/commands_finnish.ts \
    assets/translations/finnish/embroidermodder2_finnish.ts \
    assets/translations/french.toml \
    assets/translations/french/commands_french.qm \
    assets/translations/french/commands_french.ts \
    assets/translations/french/embroidermodder2_french.qm \
    assets/translations/french/embroidermodder2_french.ts \
    assets/translations/german.toml \
    assets/translations/german/commands_german.qm \
    assets/translations/german/commands_german.ts \
    assets/translations/german/embroidermodder2_german.qm \
    assets/translations/german/embroidermodder2_german.ts \
    assets/translations/greek.toml \
    assets/translations/greek/commands_greek.ts \
    assets/translations/greek/embroidermodder2_greek.ts \
    assets/translations/hindi.toml \
    assets/translations/italian.toml \
    assets/translations/italian/commands_italian.qm \
    assets/translations/italian/commands_italian.ts \
    assets/translations/italian/embroidermodder2_italian.qm \
    assets/translations/italian/embroidermodder2_italian.ts \
    assets/translations/japanese.toml \
    assets/translations/japanese/commands_japanese.qm \
    assets/translations/japanese/commands_japanese.ts \
    assets/translations/japanese/embroidermodder2_japanese.qm \
    assets/translations/japanese/embroidermodder2_japanese.ts \
    assets/translations/korean.toml \
    assets/translations/korean/commands_korean.qm \
    assets/translations/korean/commands_korean.ts \
    assets/translations/korean/embroidermodder2_korean.qm \
    assets/translations/korean/embroidermodder2_korean.ts \
    assets/translations/latin/commands_latin.qm \
    assets/translations/latin/commands_latin.ts \
    assets/translations/latin/embroidermodder2_latin.qm \
    assets/translations/latin/embroidermodder2_latin.ts \
    assets/translations/polish.toml \
    assets/translations/polish/commands_polish.qm \
    assets/translations/polish/commands_polish.ts \
    assets/translations/polish/embroidermodder2_polish.qm \
    assets/translations/polish/embroidermodder2_polish.ts \
    assets/translations/portuguese.toml \
    assets/translations/portuguese/commands_portuguese.qm \
    assets/translations/portuguese/commands_portuguese.ts \
    assets/translations/portuguese/embroidermodder2_portuguese.qm \
    assets/translations/portuguese/embroidermodder2_portuguese.ts \
    assets/translations/romanian.toml \
    assets/translations/romanian/commands_romanian.ts \
    assets/translations/romanian/embroidermodder2_romanian.ts \
    assets/translations/russian.toml \
    assets/translations/russian/commands_russian.qm \
    assets/translations/russian/commands_russian.ts \
    assets/translations/russian/embroidermodder2_russian.qm \
    assets/translations/russian/embroidermodder2_russian.ts \
    assets/translations/spanish.toml \
    assets/translations/spanish/commands_spanish.qm \
    assets/translations/spanish/commands_spanish.ts \
    assets/translations/spanish/embroidermodder2_spanish.qm \
    assets/translations/spanish/embroidermodder2_spanish.ts \
    assets/translations/swedish.toml \
    assets/translations/swedish/commands_swedish.ts \
    assets/translations/swedish/embroidermodder2_swedish.ts \
    assets/translations/turkish.toml \
    assets/translations/turkish/commands_turkish.qm \
    assets/translations/turkish/commands_turkish.ts \
    assets/translations/turkish/embroidermodder2_turkish.qm \
    assets/translations/turkish/embroidermodder2_turkish.ts

