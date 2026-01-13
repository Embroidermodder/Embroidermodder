TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS  = \
libembroidery-shared.pro \
libembroidery-static.pro \

QMAKE_DISTCLEAN += object_script.*
