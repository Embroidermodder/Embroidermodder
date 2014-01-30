TEMPLATE = lib
DESTDIR  = lib
CONFIG  += static warn_on create_prl
CONFIG  -= qt debug_and_release
CONFIG  += release
CONFIG  -= debug

include( ../libembroidery/libembroidery.pri )
include( ../libcgeometry/libcgeometry.pri )

TARGET = embroidery

OBJECTS_DIR = .obj

DEFINES += LIBEMBROIDERY_STATIC

!msvc {
QMAKE_LFLAGS += -static-libgcc
}

win32 {
    #Find out if we are cross-compiling from a Linux machine
    UNAME = $$system(uname -s)
    contains( UNAME, [lL]inux ) {
        QMAKE_POST_LINK = "mv -f lib/embroidery.prl lib/libembroidery.prl" # rename the prl file, the Linux way
    } else {
        QMAKE_POST_LINK = "MOVE /Y lib\embroidery.prl lib\libembroidery.prl >nul 2>&1" # rename the prl file, the Windows way
    }
} else {
    VERSION = 1.9.0 #TODO: update this to 2.0.0 for proper release. Better yet, grep a file with the version number
}

QMAKE_DISTCLEAN += lib/*embroidery.prl lib/*embroidery.a object_script.*
