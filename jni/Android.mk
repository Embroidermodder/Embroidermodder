LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := embroidermodder
LOCAL_SRC_FILES := em2.c embroidery.h  \
	api.c encoding.c nanosvg.h nanosvgrast.h fill.c \
	opdefines.h array.c pattern.c formats.c scheme.c \
    geometry.c scheme.h compress.c scheme-private.h \
    config.h image.c stb_image.h stb_image_write.h \
    dynload.c thread-color.c dynload.h main.c
LOCAL_CFLAGS := -DANDROID_VERSION

APP_PLATFORM := android-19

include $(BUILD_SHARED_LIBRARY)
