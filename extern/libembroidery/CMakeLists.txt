cmake_minimum_required(VERSION 3.5)

project(libembroidery VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED True)

option( EMBROIDERY_BUILD_SHARED_LIBS "Build shared library." ON )
option( EMBROIDERY_BUILD_CLI "Build embroider command line tool." ON )
option( EMBROIDERY_TESTING "Run tests." ON )

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
#    -pedantic
#    -Wall
#    -Wextra
#    -Werror
)
endif (WIN32)

set(LIBRARY_SRC
	src/pattern.c
	src/compress.c
	src/formats.c
	src/geometry.c
	src/script.c
	src/testing.c
	src/internal.h
	src/embroidery.h
	src/data/arc_polyester_colors.c
	src/data/arc_rayon_colors.c
	src/data/coats_and_clark_rayon_colors.c
	src/data/dxf_colors.c
	src/data/exquisite_polyester_colors.c
	src/data/fufu_polyester_colors.c
	src/data/fufu_rayon_colors.c
	# src/data/hemingworth_polyester_colors.c
	src/data/hus_colors.c
	# src/data/isacord_polyester_colors.c
	# src/data/isafil_rayon_colors.c
	src/data/jef_colors.c
	# src/data/madeira_polyester_colors.c
	# src/data/madeira_rayon_colors.c
	# src/data/marathon_polyester_colors.c
	# src/data/marathon_rayon_colors.c
	# src/data/metro_polyester_colors.c
	# src/data/pantone_colors.c
	src/data/pcm_colors.c
	src/data/pec_colors.c
	# src/data/robison_anton_polyester_colors.c
	# src/data/robison_anton_rayon_colors.c
	src/data/shv_colors.c
	# src/data/sigma_polyester_colors.c
	# src/data/sulky_rayon_colors.c
	src/data/svg_colors.c
	# src/data/thread_art_polyester_colors.c
	# src/data/thread_art_rayon_colors.c
	# src/data/threadelight_polyester_colors.c
	# src/data/z102_isacord_polyester_colors.c
)

add_library(embroidery_static STATIC ${LIBRARY_SRC})

if(EMBROIDERY_BUILD_SHARED_LIBS)
add_library(embroidery SHARED ${LIBRARY_SRC})

target_compile_definitions(embroidery PUBLIC LIBEMBROIDERY_SHARED)
endif()

if(EMBROIDERY_BUILD_CLI)
add_executable(embroider src/main.c)

target_link_libraries(embroider PRIVATE embroidery_static)

if (WIN32)
else (WIN32)
target_link_libraries(embroider PRIVATE m)
endif (WIN32)

if(EMBROIDERY_TESTING)
include(CTest)

add_test(NAME test0 COMMAND embroider --test 0)
add_test(NAME test1 COMMAND embroider --test 1)
add_test(NAME test2 COMMAND embroider --test 2)
endif()
endif()

if(EMBROIDERY_BUILD_SHARED_LIBS)
if(WIN32)

# TODO: Test this.
# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Install.html

install(
    FILES src/embroidery.h
    DESTINATION "<SystemDrive>/Program Files/<Project Name>"
)

install(
    TARGETS
       embroidery
       embroidery_static
       embroider
    EXPORT embroidery
    LIBRARY DESTINATION "<SystemDrive>/Program Files/<Project Name>"
    ARCHIVE DESTINATION "<SystemDrive>/Program Files/<Project Name>"
    RUNTIME DESTINATION "<SystemDrive>/Program Files/<Project Name>"
)

else(WIN32)

include(GNUInstallDirs)

install(
    FILES src/embroidery.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    TARGETS
       embroidery
       embroidery_static
       embroider
    EXPORT embroidery
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
endif(WIN32)
endif()

# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html

if(WIN32)
set(CPACK_GENERATOR "WIX")
set(CPACK_SOURCE_GENERATOR "WIX")
else(WIN32)
set(CPACK_GENERATOR "DEB")
set(CPACK_SOURCE_GENERATOR "DEB")
endif(WIN32)
set(CPACK_PACKAGE_DIRECTORY build)
set(CPACK_PACKAGE_VENDOR "The Embroidermodder Team")
set(CPACK_SOURCE_IGNORE_FILES .git .github)

include(CPack)
