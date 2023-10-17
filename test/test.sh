#!/bin/bash

git clone https://github.com/embroidermodder/libembroidery

mkdir build
cd build
cmake ..
make -j4

./test_c_core
