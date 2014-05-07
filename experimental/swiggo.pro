TEMPLATE = lib
CONFIG += console plugin no_plugin_name_prefix
CONFIG -= debug_and_release qt
CONFIG += debug
MOC_DIR = .moc
OBJECTS_DIR = .obj
TARGET = _libembroidery
include( ../libembroidery/libembroidery.pri )
QMAKE_CFLAGS_WARN_ON += -Wno-pedantic # NOTE: Disable pedantic for SWIG

DESTDIR = ./go/binding
INCLUDEPATH += ./go/binding
SOURCES += ./go/binding/swig_wrap.c
QMAKE_DISTCLEAN += ./go/binding/swig_wrap.c \
                   ./go/binding/libembroidery.go \
                   ./go/binding/libembroidery_gc.c \

system(swig -go -intgosize 32 -o ./go/binding/swig_wrap.c -I../libembroidery swig.i)
