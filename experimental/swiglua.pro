TEMPLATE = lib
CONFIG += console plugin no_plugin_name_prefix
CONFIG -= debug_and_release qt
CONFIG += debug
MOC_DIR = .moc
OBJECTS_DIR = .obj
TARGET = libembroidery
include( ../libembroidery/libembroidery.pri )
QMAKE_CFLAGS_WARN_ON += -Wno-pedantic # NOTE: Disable pedantic for SWIG

QMAKE_CFLAGS += -std=c99 # TODO: SWIG 2.0.11 wrapper is not c89 compliant. It inserts C++ style comments, which causes errors.
DESTDIR = ./lua/binding
INCLUDEPATH += ./lua/binding
INCLUDEPATH += /usr/include/lua5.2
LIBS += -L/usr/lib -llua5.2
SOURCES += ./lua/binding/swig_wrap.c
QMAKE_DISTCLEAN += ./lua/binding/swig_wrap.c \
                   ./lua/binding/libembroidery.so \

system(swig -lua -o ./lua/binding/swig_wrap.c -I../libembroidery swig.i)
