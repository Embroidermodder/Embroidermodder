cmake_minimum_required(VERSION 3.5)

project(embroidermodder VERSION 1.0.0 LANGUAGES C CXX)

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
# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt6 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt6 COMPONENTS Qml REQUIRED)
find_package(Qt6 COMPONENTS PrintSupport REQUIRED)
find_package(Qt6 COMPONENTS OpenGLWidgets REQUIRED)
set(TS_FILES 
		embroidermodder2/translations/afrikaans/commands_afrikaans.ts
		embroidermodder2/translations/afrikaans/embroidermodder2_afrikaans.ts
		embroidermodder2/translations/arabic/commands_arabic.ts
		embroidermodder2/translations/arabic/embroidermodder2_arabic.ts
		embroidermodder2/translations/chinese/commands_chinese.ts
		embroidermodder2/translations/chinese/embroidermodder2_chinese.ts
		embroidermodder2/translations/czech/commands_czech.ts
		embroidermodder2/translations/czech/embroidermodder2_czech.ts
		embroidermodder2/translations/danish/commands_danish.ts
		embroidermodder2/translations/danish/embroidermodder2_danish.ts
		embroidermodder2/translations/dutch/commands_dutch.ts
		embroidermodder2/translations/dutch/embroidermodder2_dutch.ts
		embroidermodder2/translations/english/commands_english.ts
		embroidermodder2/translations/english/embroidermodder2_english.ts
		embroidermodder2/translations/finnish/commands_finnish.ts
		embroidermodder2/translations/finnish/embroidermodder2_finnish.ts
		embroidermodder2/translations/french/commands_french.ts
		embroidermodder2/translations/french/embroidermodder2_french.ts
		embroidermodder2/translations/german/commands_german.ts
		embroidermodder2/translations/german/embroidermodder2_german.ts
		embroidermodder2/translations/greek/commands_greek.ts
		embroidermodder2/translations/greek/embroidermodder2_greek.ts
		embroidermodder2/translations/italian/commands_italian.ts
		embroidermodder2/translations/italian/embroidermodder2_italian.ts
		embroidermodder2/translations/japanese/commands_japanese.ts
		embroidermodder2/translations/japanese/embroidermodder2_japanese.ts
		embroidermodder2/translations/korean/commands_korean.ts
		embroidermodder2/translations/korean/embroidermodder2_korean.ts
		embroidermodder2/translations/latin/commands_latin.ts
		embroidermodder2/translations/latin/embroidermodder2_latin.ts
		embroidermodder2/translations/polish/commands_polish.ts
		embroidermodder2/translations/polish/embroidermodder2_polish.ts
		embroidermodder2/translations/portuguese/commands_portuguese.ts
		embroidermodder2/translations/portuguese/embroidermodder2_portuguese.ts
		embroidermodder2/translations/romanian/commands_romanian.ts
		embroidermodder2/translations/romanian/embroidermodder2_romanian.ts
		embroidermodder2/translations/russian/commands_russian.ts
		embroidermodder2/translations/russian/embroidermodder2_russian.ts
		embroidermodder2/translations/spanish/commands_spanish.ts
		embroidermodder2/translations/spanish/embroidermodder2_spanish.ts
		embroidermodder2/translations/swedish/commands_swedish.ts
		embroidermodder2/translations/swedish/embroidermodder2_swedish.ts
		embroidermodder2/translations/turkish/commands_turkish.ts
		embroidermodder2/translations/turkish/embroidermodder2_turkish.ts
		)

set(PROJECT_SOURCES
		embroidermodder2/application.cpp
		embroidermodder2/cmdprompt.cpp
		embroidermodder2/embdetails-dialog.cpp
		embroidermodder2/imagewidget.cpp
		embroidermodder2/layer-manager.cpp
		embroidermodder2/main.cpp
		embroidermodder2/mainwindow-actions.cpp
		embroidermodder2/mainwindow-commands.cpp
		embroidermodder2/mainwindow.cpp
		embroidermodder2/mainwindow-menus.cpp
		embroidermodder2/mainwindow-settings.cpp
		embroidermodder2/mainwindow-toolbars.cpp
		embroidermodder2/mdiarea.cpp
		embroidermodder2/mdiwindow.cpp
		embroidermodder2/native-scripting.cpp
		embroidermodder2/object-arc.cpp
		embroidermodder2/object-base.cpp
		embroidermodder2/object-circle.cpp
		embroidermodder2/object-dimleader.cpp
		embroidermodder2/object-ellipse.cpp
		embroidermodder2/object-image.cpp
		embroidermodder2/object-line.cpp
		embroidermodder2/object-path.cpp
		embroidermodder2/object-point.cpp
		embroidermodder2/object-polygon.cpp
		embroidermodder2/object-polyline.cpp
		embroidermodder2/object-rect.cpp
		embroidermodder2/object-save.cpp
		embroidermodder2/object-textsingle.cpp
		embroidermodder2/preview-dialog.cpp
		embroidermodder2/property-editor.cpp
		embroidermodder2/selectbox.cpp
		embroidermodder2/settings-dialog.cpp
		embroidermodder2/statusbar-button.cpp
		embroidermodder2/statusbar.cpp
		embroidermodder2/undo-commands.cpp
		embroidermodder2/undo-editor.cpp
		embroidermodder2/view.cpp
                embroidermodder2/javascript/native-javascript.cpp
                embroidermodder2/javascript/native-javascript-include.cpp
                embroidermodder2/javascript/native-javascript-init.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(embroidermodder
		MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        embroidermodder2/embroidermodder.h
        embroidermodder2/javascript/native-javascript.h
        embroidermodder2/images.qrc
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
target_include_directories(embroidermodder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libembroidery ${CMAKE_CURRENT_SOURCE_DIR}/embroidermodder2)
add_subdirectory(libembroidery)

target_link_libraries(embroidermodder PRIVATE Qt6::Qml)
target_link_libraries(embroidermodder PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(embroidermodder PRIVATE Qt6::PrintSupport)
target_link_libraries(embroidermodder PRIVATE Qt6::OpenGLWidgets)
target_link_libraries (embroidermodder LINK_PUBLIC embroidery)

# set_target_properties(embroidermodder2 PROPERTIES
#	MACOSX_BUNDLE_GUI_IDENTIFIER embroidermodder.embroidermodder.com
#	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#)

if(QT_VERSION_MAJOR EQUAL 6)
	qt_finalize_executable(embroidermodder)
endif()

