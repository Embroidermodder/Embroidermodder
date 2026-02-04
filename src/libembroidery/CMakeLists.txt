cmake_minimum_required(VERSION 3.5)

project(libembroidery VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED True)

if (WIN32)
#add_compile_options(
#    -O2
#)
else (WIN32)
add_compile_options(
    -g
    -O2
    -fPIC
    -std=c99
    -Wall
    -Wextra
#    -Werror
)
endif (WIN32)

set(LIBRARY_SRC
	compound-file.c
	compound-file-difat.c
	compound-file-directory.c
	compound-file-fat.c
	compound-file-header.c
	emb-arc.c
	emb-circle.c
	emb-color.c
	emb-compress.c
	emb-ellipse.c
	emb-file.c
	emb-flag.c
	emb-format.c
	emb-hoop.c
	emb-layer.c
	emb-line.c
	emb-logging.c
#	emb-outline.c
	emb-path.c
	emb-pattern.c
	emb-point.c
	emb-polygon.c
	emb-polyline.c
	emb-reader-writer.c
	emb-rect.c
	emb-satin-line.c
	emb-settings.c
	emb-spline.c
	emb-stitch.c
	emb-thread.c
	emb-time.c
	emb-vector.c
	format-100.c
	format-10o.c
	format-art.c
	format-bmc.c
	format-bro.c
	format-cnd.c
	format-col.c
	format-csd.c
	format-csv.c
	format-dat.c
	format-dem.c
	format-dsb.c
	format-dst.c
	format-dsz.c
	format-dxf.c
	format-edr.c
	format-emd.c
	format-exp.c
	format-exy.c
	format-eys.c
	format-fxy.c
	format-gc.c
	format-gnc.c
	format-gt.c
	format-hus.c
	format-inb.c
	format-inf.c
	format-jef.c
	format-ksm.c
	format-max.c
	format-mit.c
	format-new.c
	format-ofm.c
	format-pcd.c
	format-pcm.c
	format-pcq.c
	format-pcs.c
	format-pec.c
	format-pel.c
	format-pem.c
	format-pes.c
	format-phb.c
	format-phc.c
	format-plt.c
	format-rgb.c
	format-sew.c
	format-shv.c
	format-sst.c
	format-stx.c
	format-svg.c
	format-t01.c
	format-t09.c
	format-tap.c
	format-thr.c
	format-txt.c
	format-u00.c
	format-u01.c
	format-vip.c
	format-vp3.c
	format-xxx.c
	format-zsk.c
	geom-arc.c
	geom-circle.c
	geom-line.c
	helpers-binary.c
	helpers-misc.c
	thread-color.c

    embroidery.h
)

add_library(embroidery_static STATIC ${LIBRARY_SRC})

add_executable(sew sew.c)
target_link_libraries(sew PRIVATE embroidery_static)
