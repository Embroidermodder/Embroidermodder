cmake_minimum_required(VERSION 3.12)

# cmake build-essential gcc-arm-none-eabi

set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/pico-sdk)
set(PIMORONI_PICO_PATH ${CMAKE_SOURCE_DIR}/pimoroni-pico)

set(NAME pet-1.0.0-alpha)

set(EMBROIDERY_BUILD_CLI OFF)
set(EMBROIDERY_BUILD_SHARED_LIBS OFF)

include(pimoroni_pico_import.cmake)
include(pico_sdk_import.cmake)

project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# add_compile_options(
# 	-g
# 	-O2
# 	-Wall
# 	-Wextra
# )

# Initialize the SDK
pico_sdk_init()

add_executable(${NAME}
    src/main.cpp
    src/interface.c
    src/interface.h
)

include(common/pimoroni_i2c)
include(common/pimoroni_bus)
include(libraries/bitmap_fonts/bitmap_fonts)
include(libraries/hershey_fonts/hershey_fonts)
include(libraries/pico_explorer/pico_explorer)

add_subdirectory(${PIMORONI_PICO_PATH}/libraries/tufty2040)
add_subdirectory(libembroidery)

include_directories(
    ${PIMORONI_PICO_PATH}/libraries/tufty2040
    ${CMAKE_SOURCE_DIR}/libembroidery/src
    ${CMAKE_SOURCE_DIR}/libembroidery/src/stb
    ${CMAKE_SOURCE_DIR}/libembroidery/src/nanosvg
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${NAME}
    pico_stdlib
    pimoroni_i2c
    embroidery_static
    tufty2040
    hardware_spi
    pico_graphics
    st7789
)

pico_add_extra_outputs(${NAME})
