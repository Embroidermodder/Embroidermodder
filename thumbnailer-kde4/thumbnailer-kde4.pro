TEMPLATE = lib
CONFIG  += plugin shared warn_on
CONFIG  -= debug_and_release
CONFIG  += release
CONFIG  -= debug
CONFIG += silent #Comment this out for verbose output

TARGET = embroidery-thumbnailer-kde4

OBJECTS_DIR = .obj
MOC_DIR = .moc

INCLUDEPATH += $$PWD

HEADERS   = libembroidery-thumbnailer-kde4.h
SOURCES   = libembroidery-thumbnailer-kde4.cpp

include( ../libembroidery/libembroidery.pri )

unix {
QMAKE_STRIP    = echo                       #Suppress strip errors "File format not recognized"
QMAKE_DEL_DIR += --ignore-fail-on-non-empty #Suppress rmdir errors "Directory not empty"

kde4thumblib.path  = "/usr/lib/kde4"
kde4thumblib.files = "libembroidery-thumbnailer-kde4.so"
kde4thumblib.extra = "strip libembroidery-thumbnailer-kde4.so; cp -f libembroidery-thumbnailer-kde4.so /usr/lib/kde4/libembroidery-thumbnailer-kde4.so" #ensure the binary gets stripped of debug symbols

kde4thumbdesk.path  = "/usr/share/kde4/services"
kde4thumbdesk.files = "libembroidery-thumbnailer-kde4.desktop"

kde4thumbpix.path  = "/usr/share/pixmaps"
kde4thumbpix.files = "../project-files/debian/data/usr/share/pixmaps/embroidermodder2.png"

kde4thumbmime.path  = "/usr/share/mime/packages"
kde4thumbmime.files = "x-embroidermodder.xml"
kde4thumbmime.extra = "cp -f x-embroidermodder.xml /usr/share/mime/packages/x-embroidermodder.xml; update-mime-database /usr/share/mime"

kde4rebuildcache.path     = $$PWD
kde4rebuildcache.commands = kbuildsycoca4 2> /dev/null
}

INSTALLS += kde4thumblib \
            kde4thumbdesk \
            kde4thumbpix \
            kde4thumbmime \
            kde4rebuildcache

