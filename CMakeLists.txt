cmake_minimum_required(VERSION 3.16)

project(embroidermodder2 VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets PrintSupport OpenGL OpenGLWidgets REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
add_compile_options(
	--coverage
	-g
	-O2
	-Wall
	-Wextra
	-fprofile-arcs
	-ftest-coverage
)

add_link_options(
	-fprofile-arcs
	-ftest-coverage
)
else()
add_compile_options(
	-g
	-O2
	-Wall
)
endif()

set(SRC ${CMAKE_SOURCE_DIR}/src)
set(EXTERN ${CMAKE_SOURCE_DIR}/extern)
set(TRANSLATION ${CMAKE_SOURCE_DIR}/src/translations)

set(C_CORE
	${EXTERN}/libembroidery/embroidery.c
	${EXTERN}/libembroidery/embroidery.h

	${EXTERN}/tomlc99/toml.c
	${EXTERN}/tomlc99/toml.h

    ${TRANSLATION}/afrikaans.c
    ${TRANSLATION}/arabic.c
    ${TRANSLATION}/chinese.c
    ${TRANSLATION}/czech.c
    ${TRANSLATION}/danish.c
    ${TRANSLATION}/dutch.c
    ${TRANSLATION}/finnish.c
    ${TRANSLATION}/french.c
    ${TRANSLATION}/german.c
    ${TRANSLATION}/greek.c
    ${TRANSLATION}/korean.c
    ${TRANSLATION}/italian.c
    ${TRANSLATION}/japanese.c
    ${TRANSLATION}/portuguese.c
    ${TRANSLATION}/romanian.c
    ${TRANSLATION}/polish.c
    ${TRANSLATION}/russian.c
    ${TRANSLATION}/spanish.c
    ${TRANSLATION}/swedish.c
    ${TRANSLATION}/turkish.c

	${SRC}/commands_table.c
	${SRC}/editors_table.c
	${SRC}/settings_table.c
	${SRC}/state.c
	${SRC}/script.c
	${SRC}/objects.c
	${SRC}/commands.c
	${SRC}/geometry.c
	${SRC}/document.c
	${SRC}/properties.c
	${SRC}/xpm.c

	${SRC}/constants.h
	${SRC}/core.h
)

qt_add_executable(embroidermodder2
	${SRC}/widgets.cpp

	${C_CORE}
)

add_executable(emb2_glfw
	${C_CORE}

	${EXTERN}/nanovg/src/nanovg.c
	${EXTERN}/nanovg/src/nanovg.h

	${SRC}/main_glfw.c
)

include_directories(
	${EXTERN}/libembroidery
	${EXTERN}/tomlc99
	${EXTERN}/glfw/include
	${EXTERN}/nanovg/src
#	${GLEW_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	${SRC}
)

target_link_libraries(embroidermodder2 PRIVATE
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
)

target_link_libraries(emb2_glfw PRIVATE
	glfw
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
)

set_target_properties(embroidermodder2 PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)

if (WIN32)
else(WIN32)
target_link_libraries(embroidermodder2 PRIVATE m)
target_link_libraries(emb2_glfw PRIVATE m)
endif()

