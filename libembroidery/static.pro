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
