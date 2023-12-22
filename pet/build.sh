#!/bin/bash

export PICO_SDK_PATH="`pwd`/pico-sdk"
export PIMORONI_PICO_PATH="`pwd`/pimoroni-pico"

git submodule update --init --recursive

mkdir build
cd build
cmake ..
cmake --build .
cd ..
