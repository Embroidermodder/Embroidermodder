cmake_minimum_required(VERSION 3.16)

project(embroidermodder2 VERSION 2.0.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets PrintSupport REQUIRED)

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

add_subdirectory(extern/libembroidery)

include_directories(extern/glfw)

qt_add_executable(embroidermodder2
	${SRC}/widgets.cpp

	${EXTERN}/nanovg/src/nanovg.c
	${EXTERN}/nanovg/src/nanovg.h

	${EXTERN}/glad/src/glad.c
	${EXTERN}/glad/include/glad/glad.h
	${EXTERN}/glad/include/KHR/khrplatform.h

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
	${SRC}/settings_table.c
	${SRC}/editors_table.c
	${SRC}/state.c

	${SRC}/commands.c
	${SRC}/document.c
	${SRC}/interface.c
	${SRC}/geometry.c
	${SRC}/grip.c
	${SRC}/objects.c
	${SRC}/properties.c
	${SRC}/prompt.c
	${SRC}/settings.c
	${SRC}/script.c
	${SRC}/xpm.c

	${SRC}/constants.h
	${SRC}/core.h
)

include_directories(
	${EXTERN}/libembroidery/src
	${EXTERN}/glfw/include
	${EXTERN}/glad/include
	${EXTERN}/nanovg/src
	${OPENGL_INCLUDE_DIRS}
	${SRC}
)

target_link_libraries(embroidermodder2 PRIVATE
	glfw
	embroidery_static
	Qt6::Core
	Qt6::Widgets
	Qt6::PrintSupport
	${OPENGL_LIBRARIES}
)

set_target_properties(embroidermodder2 PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)

if (WIN32)
else(WIN32)
target_link_libraries(embroidermodder2 PRIVATE m)
endif()
