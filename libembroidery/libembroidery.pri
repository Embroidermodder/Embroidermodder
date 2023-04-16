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
    $$PWD/format-bro.c \
../libembroidery/compound-file.c \
../libembroidery/compound-file-difat.c \
../libembroidery/compound-file-directory.c \
../libembroidery/compound-file-fat.c \
../libembroidery/compound-file-header.c \
../libembroidery/emb-arc.c \
../libembroidery/emb-circle.c \
../libembroidery/emb-compress.c \
../libembroidery/emb-color.c \
../libembroidery/emb-ellipse.c \
../libembroidery/emb-file.c \
../libembroidery/emb-flag.c \
../libembroidery/emb-format.c \
../libembroidery/emb-hash.c \
../libembroidery/emb-hoop.c \
../libembroidery/emb-layer.c \
../libembroidery/emb-line.c \
../libembroidery/emb-logging.c \
../libembroidery/emb-path.c \
../libembroidery/emb-pattern.c \
../libembroidery/emb-point.c \
../libembroidery/emb-polygon.c \
../libembroidery/emb-polyline.c \
../libembroidery/emb-reader-writer.c \
../libembroidery/emb-rect.c \
../libembroidery/emb-satin-line.c \
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
../libembroidery/format-10o.c \
../libembroidery/format-100.c \
../libembroidery/format-art.c \
../libembroidery/format-bmc.c \
../libembroidery/format-cnd.c \
../libembroidery/format-col.c \
../libembroidery/format-csd.c \
../libembroidery/format-csv.c \
../libembroidery/format-dat.c \
../libembroidery/format-dem.c \
../libembroidery/format-dsb.c \
../libembroidery/format-dst.c \
../libembroidery/format-dsz.c \
../libembroidery/format-dxf.c \
../libembroidery/format-edr.c \
../libembroidery/format-emd.c \
../libembroidery/format-exp.c \
../libembroidery/format-exy.c \
../libembroidery/format-eys.c \
../libembroidery/format-fxy.c \
../libembroidery/format-gc.c  \
../libembroidery/format-gnc.c \
../libembroidery/format-gt.c  \
../libembroidery/format-hus.c \
../libembroidery/format-inb.c \
../libembroidery/format-inf.c \
../libembroidery/format-jef.c \
../libembroidery/format-ksm.c \
../libembroidery/format-max.c \
../libembroidery/format-mit.c \
../libembroidery/format-new.c \
../libembroidery/format-ofm.c \
../libembroidery/format-pcd.c \
../libembroidery/format-pcm.c \
../libembroidery/format-pcq.c \
../libembroidery/format-pcs.c \
../libembroidery/format-pec.c \
../libembroidery/format-pel.c \
../libembroidery/format-pem.c \
../libembroidery/format-pes.c \
../libembroidery/format-phb.c \
../libembroidery/format-phc.c \
../libembroidery/format-plt.c \
../libembroidery/format-rgb.c \
../libembroidery/format-sew.c \
../libembroidery/format-shv.c \
../libembroidery/format-sst.c \
../libembroidery/format-stx.c \
../libembroidery/format-svg.c \
../libembroidery/format-t01.c \
../libembroidery/format-t09.c \
../libembroidery/format-tap.c \
../libembroidery/format-thr.c \
../libembroidery/format-txt.c \
../libembroidery/format-u00.c \
../libembroidery/format-u01.c \
../libembroidery/format-vip.c \
../libembroidery/format-vp3.c \
../libembroidery/format-xxx.c \
../libembroidery/format-zsk.c \

HEADERS += \
    $$PWD/embroidery.h \
../libembroidery/emb-compress.h \
../libembroidery/hashtable.h \

# TODO: merge the computational geometry code into libembroidery structs
SOURCES += \
../libembroidery/geom-arc.c \
../libembroidery/geom-line.c \

HEADERS += \
