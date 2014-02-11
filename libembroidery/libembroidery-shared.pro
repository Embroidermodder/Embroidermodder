TEMPLATE = lib
DESTDIR  = lib
CONFIG  += shared warn_on
CONFIG  -= qt debug_and_release
CONFIG  += release
CONFIG  -= debug

include( ../libembroidery/libembroidery.pri )
include( ../libcgeometry/libcgeometry.pri )

TARGET = embroidery

OBJECTS_DIR = .obj

DEFINES += LIBEMBROIDERY_SHARED

!msvc {
    !macx { #TODO: better clang support
        QMAKE_LFLAGS += -static-libgcc
    }
}

win32 {
    TARGET = lib$$qtLibraryTarget($$TARGET) #force name to be libembroidery.dll
    
    #Find out if we are cross-compiling from a Linux machine
    UNAME = $$system(uname -s)
    contains( UNAME, [lL]inux ) {
        QMAKE_POST_LINK = "mv -f lib/liblibembroidery.a lib/libembroidery.dll.a" # rename the stub, the Linux way
    } else {
        QMAKE_POST_LINK = "MOVE /Y lib\liblibembroidery.a lib\libembroidery.dll.a >nul 2>&1" # rename the stub, the Windows way
    }
} else {
    VERSION = 1.9.0 #TODO: update this to 2.0.0 for proper release. Better yet, grep a file with the version number
}

QMAKE_DISTCLEAN += lib/*embroidery.dll* object_script.*
