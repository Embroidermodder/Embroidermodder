TEMPLATE = app
CONFIG -= debug_and_release qt
CONFIG += console debug
#CONFIG += console release
CONFIG -= app_bundle

TARGET = libembroidery-convert

OBJECTS_DIR = .obj
MOC_DIR = .moc

INCLUDEPATH += \
../libembroidery \
../libcgeometry \
$$PWD \

QMAKE_CFLAGS += -std=c89

#Ensure all implicit function declarations are errors rather than warnings
QMAKE_CFLAGS_WARN_ON += -Werror=implicit-function-declaration

#Uncomment line below to consider all warnings as errors
#QMAKE_CFLAGS_WARN_ON += -Werror

#Uncomment lines below to hide various warnings
#QMAKE_CFLAGS_WARN_ON += -Wno-unused-parameter 
#QMAKE_CFLAGS_WARN_ON += -Wno-unused-but-set-variable
#QMAKE_CFLAGS_WARN_ON += -Wno-unused-variable
#QMAKE_CFLAGS_WARN_ON += -Wno-unknown-pragmas

SOURCES += libembroidery-convert-main.c

include( ../libembroidery/libembroidery.pri )
include( ../libcgeometry/libcgeometry.pri )

