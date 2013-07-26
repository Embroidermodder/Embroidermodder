TEMPLATE = app
CONFIG -= debug_and_release
CONFIG += debug #This adds -g to CXXFLAGS so valgrind can locate the exact line.

TARGET = embroidermodder2

QT += opengl
QT += script scripttools

#Qt5
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += printsupport widgets
}

OBJECTS_DIR = .obj
MOC_DIR = .moc

INCLUDEPATH += \
$$PWD \
javascript \

#GUI
SOURCES += \
main.cpp \
mainwindow.cpp \
mainwindow-settings.cpp \
mainwindow-menus.cpp \
mainwindow-toolbars.cpp \
mainwindow-commands.cpp \
mainwindow-actions.cpp \
mdiarea.cpp \
mdiwindow.cpp \
view.cpp \
cmdprompt.cpp \
settings-dialog.cpp \
preview-dialog.cpp \
layer-manager.cpp \
statusbar.cpp \
statusbar-button.cpp \
imagewidget.cpp \
property-editor.cpp \
undo-editor.cpp \
undo-commands.cpp \
object-save.cpp \
object-base.cpp \
object-arc.cpp \
object-circle.cpp \
object-dimleader.cpp \
object-ellipse.cpp \
object-image.cpp \
object-line.cpp \
object-path.cpp \
object-point.cpp \
object-polygon.cpp \
object-polyline.cpp \
object-rect.cpp \
object-textsingle.cpp \

HEADERS += \
mainwindow.h \
mainwindow-actions.h \
mdiarea.h \
mdiwindow.h \
view.h \
cmdprompt.h \
settings-dialog.h \
preview-dialog.h \
layer-manager.h \
statusbar.h \
statusbar-button.h \
imagewidget.h \
property-editor.h \
undo-editor.h \
undo-commands.h \
object-data.h \
object-save.h \
object-base.h \
object-arc.h \
object-circle.h \
object-dimleader.h \
object-ellipse.h \
object-image.h \
object-line.h \
object-path.h \
object-point.h \
object-polygon.h \
object-polyline.h \
object-rect.h \
object-textsingle.h \

#SCRIPTING
SOURCES += \
native-scripting.cpp \
javascript/native-javascript.cpp \
javascript/native-javascript-init.cpp \
javascript/native-javascript-include.cpp \

HEADERS += \
native-scripting.h \
javascript/native-javascript.h \

include( ../libembroidery/libembroidery.pri )
include( ../libcgeometry/libcgeometry.pri )

# install *nix
unix {
QMAKE_STRIP    = echo                       #Suppress strip errors "File format not recognized"
QMAKE_DEL_DIR += --ignore-fail-on-non-empty #Suppress rmdir errors "Directory not empty"

embapp.path  = "/usr/share/embroidermodder2"
embapp.files = "embroidermodder2"
embapp.extra = "strip embroidermodder2; cp -f embroidermodder2 /usr/share/embroidermodder2/embroidermodder2" #ensure the binary gets stripped of debug symbols

embcommands.path  = "/usr/share/embroidermodder2"
embcommands.files = "commands"

embhelp.path  = "/usr/share/embroidermodder2"
embhelp.files = "help"

embicons.path  = "/usr/share/embroidermodder2"
embicons.files = "icons"

embimages.path  = "/usr/share/embroidermodder2"
embimages.files = "images"

embsamples.path  = "/usr/share/embroidermodder2"
embsamples.files = "samples"

INSTALLS += embapp      \
            embcommands \
            embhelp     \
            embicons    \
            embimages   \
            embsamples  \

}

win32 {
#Qt5 Deployment
# embroidermodder2.exe
# D3DCompiler_##.dll
# icudt##.dll
# icuin##.dll
# icuuc##.dll
# libEGL.dll
# libgcc_s_sjlj-#.dll
# libGLESv2.dll
# libstdc++-6.dll
# libwinpthread-#.dll
# Qt5Core.dll
# Qt5Gui.dll
# Qt5OpenGL.dll
# Qt5PrintSupport.dll
# Qt5Script.dll
# Qt5ScriptTools.dll
# Qt5Widgets.dll
# platforms/qwindows.dll
}

#Uncomment lines below to hide various warnings
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter 
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-but-set-variable
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-variable
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas
