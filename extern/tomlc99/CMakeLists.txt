cmake_minimum_required(VERSION 3.5)

project(toml VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_library(tomlc99 STATIC toml.c toml.h)
