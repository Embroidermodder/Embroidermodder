TEMPLATE = app
CONFIG -= debug_and_release qt
CONFIG += debug
#CONFIG += release

TARGET = libembroidery-convert

OBJECTS_DIR = .obj
MOC_DIR = .moc

INCLUDEPATH += \
../libembroidery \
../libcgeometry \
$$PWD \

QMAKE_CFLAGS += -std=c89

#Uncomment lines below to hide various warnings
#QMAKE_CFLAGS_WARN_ON += -Wno-unused-parameter 
#QMAKE_CFLAGS_WARN_ON += -Wno-unused-but-set-variable
#QMAKE_CFLAGS_WARN_ON += -Wno-unused-variable
#QMAKE_CFLAGS_WARN_ON += -Wno-unknown-pragmas

SOURCES += main.c

include( ../libembroidery/libembroidery.pri )
include( ../libcgeometry/libcgeometry.pri )

