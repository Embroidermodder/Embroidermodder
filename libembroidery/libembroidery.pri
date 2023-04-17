INCLUDEPATH += ../libembroidery

msvc {
QMAKE_CFLAGS += /Za #equivalent of -pedantic-errors
}

!msvc {
#Ensure anything that does not strictly adhere to C89 is treated as an error
QMAKE_CFLAGS += -std=c89 -pedantic-errors
QMAKE_CFLAGS += -fvisibility=hidden #Check exported symbols using: nm -C -D libembroidery.so | grep ' T '

#Ensure all implicit function declarations are errors rather than warnings
QMAKE_CFLAGS_WARN_ON += -Werror=implicit-function-declaration
}

#Uncomment line below to consider all warnings as errors
#QMAKE_CFLAGS_WARN_ON += -Werror

#Uncomment lines below to hide various warnings
#QMAKE_CFLAGS_WARN_ON += -Wno-unused-parameter
#QMAKE_CFLAGS_WARN_ON += -Wno-unused-but-set-variable
#QMAKE_CFLAGS_WARN_ON += -Wno-unused-variable

SOURCES += \
    $$PWD/formats/format-100.c \
    $$PWD/formats/format-10o.c \
    $$PWD/formats/format-art.c \
    $$PWD/formats/format-bmc.c \
    $$PWD/formats/format-bro.c \
    $$PWD/formats/format-cnd.c \
    $$PWD/formats/format-col.c \
    $$PWD/formats/format-csd.c \
    $$PWD/formats/format-csv.c \
    $$PWD/formats/format-dat.c \
    $$PWD/formats/format-dem.c \
    $$PWD/formats/format-dsb.c \
    $$PWD/formats/format-dst.c \
    $$PWD/formats/format-dsz.c \
    $$PWD/formats/format-dxf.c \
    $$PWD/formats/format-edr.c \
    $$PWD/formats/format-emd.c \
    $$PWD/formats/format-exp.c \
    $$PWD/formats/format-exy.c \
    $$PWD/formats/format-eys.c \
    $$PWD/formats/format-fxy.c \
    $$PWD/formats/format-gc.c \
    $$PWD/formats/format-gnc.c \
    $$PWD/formats/format-gt.c \
    $$PWD/formats/format-hus.c \
    $$PWD/formats/format-inb.c \
    $$PWD/formats/format-inf.c \
    $$PWD/formats/format-jef.c \
    $$PWD/formats/format-ksm.c \
    $$PWD/formats/format-max.c \
    $$PWD/formats/format-mit.c \
    $$PWD/formats/format-new.c \
    $$PWD/formats/format-ofm.c \
    $$PWD/formats/format-pcd.c \
    $$PWD/formats/format-pcm.c \
    $$PWD/formats/format-pcq.c \
    $$PWD/formats/format-pcs.c \
    $$PWD/formats/format-pec.c \
    $$PWD/formats/format-pel.c \
    $$PWD/formats/format-pem.c \
    $$PWD/formats/format-pes.c \
    $$PWD/formats/format-phb.c \
    $$PWD/formats/format-phc.c \
    $$PWD/formats/format-plt.c \
    $$PWD/formats/format-rgb.c \
    $$PWD/formats/format-sew.c \
    $$PWD/formats/format-shv.c \
    $$PWD/formats/format-sst.c \
    $$PWD/formats/format-stx.c \
    $$PWD/formats/format-svg.c \
    $$PWD/formats/format-t01.c \
    $$PWD/formats/format-t09.c \
    $$PWD/formats/format-tap.c \
    $$PWD/formats/format-thr.c \
    $$PWD/formats/format-txt.c \
    $$PWD/formats/format-u00.c \
    $$PWD/formats/format-u01.c \
    $$PWD/formats/format-vip.c \
    $$PWD/formats/format-vp3.c \
    $$PWD/formats/format-xxx.c \
    $$PWD/formats/format-zsk.c \
    $$PWD/geometry/arc.c \
    $$PWD/geometry/circle.c \
    $$PWD/geometry/emb-line.c \
    $$PWD/geometry/emb-polygon.c \
    $$PWD/geometry/emb-polyline.c \
    $$PWD/geometry/emb-rect.c \
    $$PWD/geometry/emb-satin-line.c \
    $$PWD/geometry/geom-line.c \
    ../libembroidery/compound-file.c \
    ../libembroidery/compound-file-difat.c \
    ../libembroidery/compound-file-directory.c \
    ../libembroidery/compound-file-fat.c \
    ../libembroidery/compound-file-header.c \
    ../libembroidery/emb-compress.c \
    ../libembroidery/emb-color.c \
    ../libembroidery/emb-ellipse.c \
    ../libembroidery/emb-file.c \
    ../libembroidery/emb-flag.c \
    ../libembroidery/emb-format.c \
    ../libembroidery/emb-hash.c \
    ../libembroidery/emb-hoop.c \
    ../libembroidery/emb-layer.c \
    ../libembroidery/emb-logging.c \
    ../libembroidery/emb-path.c \
    ../libembroidery/emb-pattern.c \
    ../libembroidery/emb-point.c \
    ../libembroidery/emb-reader-writer.c \
    ../libembroidery/emb-settings.c \
    ../libembroidery/emb-spline.c \
    ../libembroidery/emb-stitch.c \
    ../libembroidery/emb-thread.c \
    ../libembroidery/emb-time.c \
    ../libembroidery/emb-vector.c \
    ../libembroidery/hashtable.c \
    ../libembroidery/helpers-binary.c \
    ../libembroidery/helpers-misc.c \
    ../libembroidery/thread-color.c \

HEADERS += \
    $$PWD/emb-hash.h \
    $$PWD/embroidery-internal.h \
    $$PWD/embroidery.h \
    ../libembroidery/emb-compress.h \
    ../libembroidery/hashtable.h \
