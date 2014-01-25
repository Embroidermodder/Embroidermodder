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

win32 {
    TARGET = lib$$qtLibraryTarget($$TARGET) #force name to be libembroidery.dll
    
    #Find out if we are cross-compiling from a Linux machine
    UNAME = $$system(uname -s)
    contains( UNAME, [lL]inux ) {
        QMAKE_POST_LINK = "rm -f lib/liblibembroidery.a" # remove unused temporary file, the Linux way
    } else {
        QMAKE_POST_LINK = "DEL /F lib\liblibembroidery.a >nul 2>&1" # remove unused temporary file, the Windows way
    }
} else {
    VERSION = 1.9.0 #TODO: update this to 2.0.0 for proper release. Better yet, grep a file with the version number
}
