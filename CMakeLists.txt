cmake_minimum_required(VERSION 3.5)

project(embroidermodder VERSION 1.91.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_INCLUDE_CURRENT_DIR OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -O2")

set(EMBROIDERY_BUILD_SHARED_LIBS OFF)

INCLUDE(FindPkgConfig)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# We currently assume Apple has HOME set, which is not always
# true.
if(UNIX)
set(ASSET_DIR $ENV{HOME}/.embroidermodder2)
else()
copyset(ASSET_DIR %HOMEDRIVE%%HOMEPATH%\.embroidermodder2)
endif()

find_package(Qt5Widgets)
find_package(OpenGL)

# Most of Embroidermodder is assets, so we just copy a full
# tree to the user's assets folder. That way in an emergency
# everything is traceable.
#
# Trailing / means the DIRECTORY is emptied into the location
# whereas without it means append the DIRECTORY name.
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${ASSET_DIR})

SET(FREEGLUT_HEADERS
	extern/freeglut3/include/GL/freeglut.h
	extern/freeglut3/include/GL/freeglut_ucall.h
	extern/freeglut3/include/GL/freeglut_ext.h
	extern/freeglut3/include/GL/freeglut_std.h
)

SET(FREEGLUT_SRCS
	${FREEGLUT_HEADERS}
	extern/freeglut3/src/fg_callbacks.c
	extern/freeglut3/src/fg_cursor.c
	extern/freeglut3/src/fg_display.c
	extern/freeglut3/src/fg_ext.c
	extern/freeglut3/src/fg_font_data.c
	extern/freeglut3/src/fg_gamemode.c
	extern/freeglut3/src/fg_geometry.c
	extern/freeglut3/src/fg_gl2.c
	extern/freeglut3/src/fg_gl2.h
	extern/freeglut3/src/fg_init.c
	extern/freeglut3/src/fg_init.h
	extern/freeglut3/src/fg_internal.h
	extern/freeglut3/src/fg_callback_macros.h
	extern/freeglut3/src/fg_input_devices.c
	extern/freeglut3/src/fg_joystick.c
	extern/freeglut3/src/fg_main.c
	extern/freeglut3/src/fg_misc.c
	extern/freeglut3/src/fg_overlay.c
	extern/freeglut3/src/fg_spaceball.c
	extern/freeglut3/src/fg_state.c
	extern/freeglut3/src/fg_stroke_mono_roman.c
	extern/freeglut3/src/fg_stroke_roman.c
	extern/freeglut3/src/fg_structure.c
	extern/freeglut3/src/fg_teapot.c
	extern/freeglut3/src/fg_teapot_data.h
	extern/freeglut3/src/fg_videoresize.c
	extern/freeglut3/src/fg_window.c
	extern/freeglut3/src/fg_font.c
        extern/freeglut3/src/fg_menu.c
)

IF(WIN32)
    LIST(APPEND FREEGLUT_SRCS
	extern/freeglut3/src/mswin/fg_cursor_mswin.c
	extern/freeglut3/src/mswin/fg_display_mswin.c
	extern/freeglut3/src/mswin/fg_ext_mswin.c
	extern/freeglut3/src/mswin/fg_gamemode_mswin.c
	extern/freeglut3/src/mswin/fg_init_mswin.c
    	extern/freeglut3/src/mswin/fg_internal_mswin.h
    	extern/freeglut3/src/mswin/fg_input_devices_mswin.c
    	extern/freeglut3/src/mswin/fg_joystick_mswin.c
    	extern/freeglut3/src/mswin/fg_main_mswin.c
    	extern/freeglut3/src/mswin/fg_menu_mswin.c
    	extern/freeglut3/src/mswin/fg_spaceball_mswin.c
    	extern/freeglut3/src/mswin/fg_state_mswin.c
    	extern/freeglut3/src/mswin/fg_structure_mswin.c
    	extern/freeglut3/src/mswin/fg_window_mswin.c
	${CMAKE_BINARY_DIR}/freeglut.rc # generated below from freeglut.rc.in
    )

ELSEIF(ANDROID OR BLACKBERRY)
    # BlackBerry and Android share some similar design concepts and ideas, as with many mobile devices.
    # As such, some classes can be shared between the two. XXX: Possibly rename shareable classes to
    # a more generic name. *_stub? *_mobile?
    LIST(APPEND FREEGLUT_SRCS
	extern/freeglut3/src/android/fg_cursor_android.c
	extern/freeglut3/src/android/fg_ext_android.c
	extern/freeglut3/src/android/fg_gamemode_android.c
	extern/freeglut3/src/android/fg_joystick_android.c
	extern/freeglut3/src/android/fg_spaceball_android.c
    )
    IF(ANDROID)
	LIST(APPEND FREEGLUT_SRCS
		extern/freeglut3/src/android/native_app_glue/android_native_app_glue.c
		extern/freeglut3/src/android/native_app_glue/android_native_app_glue.h
		extern/freeglut3/src/android/fg_internal_android.h
		extern/freeglut3/src/android/fg_init_android.c
		extern/freeglut3/src/android/fg_input_devices_android.c
		extern/freeglut3/src/android/fg_main_android.c
		extern/freeglut3/src/android/fg_main_android.h
		extern/freeglut3/src/android/fg_runtime_android.c
		extern/freeglut3/src/android/fg_state_android.c
		extern/freeglut3/src/android/fg_structure_android.c
		extern/freeglut3/src/android/fg_window_android.c
	)
    ELSE()
	LIST(APPEND FREEGLUT_SRCS
		extern/freeglut3/src/blackberry/fg_internal_blackberry.h
		extern/freeglut3/src/blackberry/fg_init_blackberry.c
		extern/freeglut3/src/x11/fg_input_devices_x11.c
		extern/freeglut3/src/blackberry/fg_main_blackberry.c
		extern/freeglut3/src/blackberry/fg_state_blackberry.c
		extern/freeglut3/src/blackberry/fg_structure_blackberry.c
		extern/freeglut3/src/blackberry/fg_window_blackberry.c
	)
    ENDIF()

ELSE()
	# UNIX (Wayland)
	IF(FREEGLUT_WAYLAND)
	LIST(APPEND FREEGLUT_SRCS
		extern/freeglut3/src/wayland/fg_cursor_wl.c
		extern/freeglut3/src/wayland/fg_ext_wl.c
		extern/freeglut3/src/wayland/fg_gamemode_wl.c
		extern/freeglut3/src/wayland/fg_init_wl.c
		extern/freeglut3/src/wayland/fg_internal_wl.h
		extern/freeglut3/src/wayland/fg_input_devices_wl.c
		extern/freeglut3/src/wayland/fg_main_wl.c
		extern/freeglut3/src/wayland/fg_state_wl.c
		extern/freeglut3/src/wayland/fg_structure_wl.c
		extern/freeglut3/src/wayland/fg_window_wl.c
	# font, serial port & joystick code are agnostic
		extern/freeglut3/src/x11/fg_glutfont_definitions_x11.c
		extern/freeglut3/src/x11/fg_input_devices_x11.c
		extern/freeglut3/src/x11/fg_joystick_x11.c
	)
	# UNIX (X11)
	ELSE()
	LIST(APPEND FREEGLUT_SRCS
		extern/freeglut3/src/x11/fg_cursor_x11.c
		extern/freeglut3/src/x11/fg_ext_x11.c
		extern/freeglut3/src/x11/fg_gamemode_x11.c
		extern/freeglut3/src/x11/fg_glutfont_definitions_x11.c
		extern/freeglut3/src/x11/fg_init_x11.c
		extern/freeglut3/src/x11/fg_internal_x11.h
		extern/freeglut3/src/x11/fg_input_devices_x11.c
		extern/freeglut3/src/x11/fg_joystick_x11.c
		extern/freeglut3/src/x11/fg_main_x11.c
		extern/freeglut3/src/x11/fg_menu_x11.c
		extern/freeglut3/src/x11/fg_spaceball_x11.c
		extern/freeglut3/src/x11/fg_state_x11.c
		extern/freeglut3/src/x11/fg_structure_x11.c
		extern/freeglut3/src/x11/fg_window_x11.c
		extern/freeglut3/src/x11/fg_xinput_x11.c
		extern/freeglut3/src/x11/fg_internal_x11_glx.h
		extern/freeglut3/src/x11/fg_display_x11_glx.c
		extern/freeglut3/src/x11/fg_state_x11_glx.c
	    	extern/freeglut3/src/x11/fg_state_x11_glx.h
	    	extern/freeglut3/src/x11/fg_window_x11_glx.c
	    	extern/freeglut3/src/x11/fg_window_x11_glx.h
	)
	ENDIF()
ENDIF()

add_executable(embroidermodder
	src/main.cpp
	src/settings.cpp
	src/actions.cpp
	src/gui.c
	src/icons.c
	src/config.c
	src/embroidermodder.h
	src/images.qrc
	extern/cJSON/cJSON.c
	${FREEGLUT_SRCS}
)

target_include_directories(embroidermodder PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/libembroidery
	${CMAKE_CURRENT_SOURCE_DIR}/extern/freeglut3/include
	${CMAKE_CURRENT_SOURCE_DIR}/extern/freeglut3/src
	${CMAKE_CURRENT_SOURCE_DIR}/extern/cJSON
	${CMAKE_CURRENT_SOURCE_DIR}/src
)
add_subdirectory(libembroidery)

target_link_libraries(embroidermodder
	Qt5::Widgets
	GL
	embroidery_static
)

if(LINUX)
target_link_libraries(embroidermodder
	X11
)
endif()

set_target_properties(embroidermodder PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER embroidermodder.embroidermodder.org
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(LINUX)
install(
    TARGETS
       embroidermodder
    LIBRARY DESTINATION $ENV{HOME}/.local/lib
    ARCHIVE DESTINATION $ENV{HOME}/.local/share
    RUNTIME DESTINATION $ENV{HOME}/.local/bin
)
endif()
