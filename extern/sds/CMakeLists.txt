cmake_minimum_required(VERSION 3.11)

project(sds VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_library(sds STATIC sds.c sds.h)
