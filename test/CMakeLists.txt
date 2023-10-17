# Testing for the C Core functions
#

cmake_minimum_required(VERSION 3.16)
project(test_c_core VERSION 2.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(
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

add_subdirectory(libembroidery)

add_executable(test_c_core WIN32 MACOSX_BUNDLE
    ${CMAKE_SOURCE_DIR}/test_core.c
    ${CMAKE_SOURCE_DIR}/../src/core.c
)

include_directories(
    ${CMAKE_SOURCE_DIR}/libembroidery/src
    ${CMAKE_SOURCE_DIR}/libembroidery/src/stb
    ${CMAKE_SOURCE_DIR}/libembroidery/src/nanosvg
    ${CMAKE_SOURCE_DIR}/../src
)

target_link_libraries(test_c_core PRIVATE embroidery_static)

if (WIN32)
else(WIN32)
target_link_libraries(test_c_core PRIVATE m)
endif()
