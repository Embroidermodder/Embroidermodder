TEMPLATE = lib
CONFIG += console plugin no_plugin_name_prefix
CONFIG -= debug_and_release qt
CONFIG += debug
MOC_DIR = .moc
OBJECTS_DIR = .obj
TARGET = _libembroidery
include( ../libembroidery/libembroidery.pri )
QMAKE_CFLAGS_WARN_ON += -Wno-pedantic # NOTE: Disable pedantic for SWIG

DESTDIR = ./python/binding
INCLUDEPATH += ./python/binding
INCLUDEPATH += /usr/include/python2.7
LIBS += -L/usr/lib -lpython2.7
SOURCES += ./python/binding/swig_wrap.c
QMAKE_DISTCLEAN += ./python/binding/swig_wrap.c \
                   ./python/binding/_libembroidery.so \
                   ./python/binding/libembroidery.py \
                   ./python/binding/libembroidery.pyc \

system(swig -python -o ./python/binding/swig_wrap.c -I../libembroidery swig.i)
