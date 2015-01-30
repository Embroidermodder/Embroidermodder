TEMPLATE = lib
CONFIG += console plugin no_plugin_name_prefix
CONFIG -= debug_and_release qt
CONFIG += debug
MOC_DIR = .moc
OBJECTS_DIR = .obj
TARGET = libembroidery
include( ../libembroidery/libembroidery.pri )
QMAKE_CFLAGS_WARN_ON += -Wno-pedantic # NOTE: Disable pedantic for SWIG

DESTDIR = ./csharp/binding
INCLUDEPATH += ./csharp/binding
SOURCES += ./csharp/binding/swig_wrap.c
QMAKE_DISTCLEAN += ./csharp/binding/swig_wrap.c \
                   ./csharp/binding/libembroidery.so \
                   ./csharp/binding/*.cs \

system(swig -csharp -o ./csharp/binding/swig_wrap.c -I../libembroidery swig.i)
