TEMPLATE = app
CONFIG -= debug_and_release
CONFIG += silent #Comment this out for verbose output
#CONFIG += deploy #Uncomment this to create a release build or by running this from the terminal: qmake CONFIG+=deploy

deploy {
    message("release build")
    CONFIG -= debug
    CONFIG += release
} else {
    message("debug build")
    CONFIG += debug #This adds -g to the compiler flags so valgrind can locate the exact line.
    CONFIG -= release
}

ICON = embroidermodder2.icns

TARGET = embroidermodder2

QT += opengl
QT += script scripttools

#Qt5
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += printsupport widgets
}

!msvc {
    !macx { #TODO: better clang support
        QMAKE_LFLAGS += -static-libgcc -static-libstdc++
    }
}

OBJECTS_DIR = .obj
MOC_DIR = .moc

INCLUDEPATH += \
$$PWD \
javascript \

TRANSLATIONS = translations/afrikaans/embroidermodder2_afrikaans.ts   \ #af
               translations/arabic/embroidermodder2_arabic.ts         \ #ar
               translations/chinese/embroidermodder2_chinese.ts       \ #zh
               translations/czech/embroidermodder2_czech.ts           \ #cs
               translations/danish/embroidermodder2_danish.ts         \ #da
               translations/dutch/embroidermodder2_dutch.ts           \ #nl
               translations/english/embroidermodder2_english.ts       \ #en
               translations/finnish/embroidermodder2_finnish.ts       \ #fi
               translations/french/embroidermodder2_french.ts         \ #fr
               translations/german/embroidermodder2_german.ts         \ #de
               translations/greek/embroidermodder2_greek.ts           \ #el
               translations/italian/embroidermodder2_italian.ts       \ #it
               translations/japanese/embroidermodder2_japanese.ts     \ #ja
               translations/korean/embroidermodder2_korean.ts         \ #ko
               translations/latin/embroidermodder2_latin.ts           \ #la
               translations/polish/embroidermodder2_polish.ts         \ #pl
               translations/portuguese/embroidermodder2_portuguese.ts \ #pt
               translations/romanian/embroidermodder2_romanian.ts     \ #ro
               translations/russian/embroidermodder2_russian.ts       \ #ru
               translations/spanish/embroidermodder2_spanish.ts       \ #es
               translations/swedish/embroidermodder2_swedish.ts       \ #sv
               translations/turkish/embroidermodder2_turkish.ts       \ #tr

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
embdetails-dialog.cpp \
settings-dialog.cpp \
preview-dialog.cpp \
layer-manager.cpp \
selectbox.cpp \
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
embdetails-dialog.h \
settings-dialog.h \
preview-dialog.h \
layer-manager.h \
selectbox.h \
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

#Linux/Unix: make install
unix:!macx {
QMAKE_STRIP    = echo                       #Suppress strip errors "File format not recognized"
QMAKE_DEL_DIR += --ignore-fail-on-non-empty #Suppress rmdir errors "Directory not empty"

# Application Specific

embapp.path  = "/usr/share/embroidermodder2"
embapp.files = "embroidermodder2"
embapp.extra = "strip embroidermodder2; cp -f embroidermodder2 /usr/share/embroidermodder2/embroidermodder2" #ensure the binary gets stripped of debug symbols

embtips.path  = "/usr/share/embroidermodder2"
embtips.files = "tips.txt"
embtips.extra = "cp -f tips.txt /usr/share/embroidermodder2/tips.txt"

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

embtrans.path  = "/usr/share/embroidermodder2"
embtrans.files = "translations"

# Debian Specific

debusrbin.path    = "/usr/bin"
debusrbin.files   = "../project-files/debian/data/usr/bin/embroidermodder2"

deblibmime.path   = "/usr/lib/mime/packages"
deblibmime.files  = "../project-files/debian/data/usr/lib/mime/packages/embroidermodder2"

debappregistry.path   = "/usr/share/application-registry"
debappregistry.files  = "../project-files/debian/data/usr/share/application-registry/embroidermodder2.applications"

debapplications.path   = "/usr/share/applications"
debapplications.files  = "../project-files/debian/data/usr/share/applications/embroidermodder2.desktop"

debdoc.path   = "/usr/share/doc"
debdoc.files  = "../project-files/debian/data/usr/share/doc/embroidermodder2"

debman.path   = "/usr/share/man/man1"
debman.files  = "../project-files/debian/data/usr/share/man/man1/embroidermodder2.1.gz"

debmenu.path   = "/usr/share/menu"
debmenu.files  = "../project-files/debian/data/usr/share/menu/embroidermodder2"

debmimexml.path   = "/usr/share/mime/application"
debmimexml.files  = "../project-files/debian/data/usr/share/mime/application/x-embroidermodder.xml"

debmimeinfo.path   = "/usr/share/mime-info"
debmimeinfo.files  = "../project-files/debian/data/usr/share/mime-info/embroidermodder2.keys" \
                     "../project-files/debian/data/usr/share/mime-info/embroidermodder2.mime"

debpixmaps.path   = "/usr/share/pixmaps"
debpixmaps.files  = "../project-files/debian/data/usr/share/pixmaps/embroidermodder2.png"

INSTALLS += embapp      \
            embtips     \
            embcommands \
            embhelp     \
            embicons    \
            embimages   \
            embsamples  \
            embtrans    \
            debusrbin       \
            deblibmime      \
            debappregistry  \
            debapplications \
            debdoc          \
            debman          \
            debmenu         \
            debmimexml      \
            debmimeinfo     \
            debpixmaps      \

}

#TODO: Mac OSX: make install
macx {
}

#TODO: Windows: make install
win32 {
#Qt4 Deployment
# embroidermodder2.exe
# libgcc_s_dw2-1.dll
# mingwm10.dll
# QtCore4.dll
# QtGui4.dll
# QtOpenGL4.dll
# QtScript4.dll
# QtScriptTools4.dll
}

# TODO: review this for Qt5.2 or greater
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

#TODO: Linux/Unix: make installer
unix:!macx {
installer.commands = ""
installer.depends = ""
}

#TODO: Windows: make installer
win32 {
installer.commands = "makensis ../project-files/nsis/embroidermodder-installer.nsi;"
installer.depends = ""
}

#TODO: Mac OSX: make installer
macx {
installer.commands = ""
installer.depends = ""
}

QMAKE_EXTRA_TARGETS += installer

#Uncomment lines below to hide various warnings
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter 
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-but-set-variable
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-variable
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas
