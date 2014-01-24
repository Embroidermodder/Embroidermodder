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
