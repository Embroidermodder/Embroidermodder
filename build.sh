#!/bin/sh

echo "Build for POSIX systems."

git submodule init
git submodule update

mkdir build
cd build
cmake ..
cmake --build .
cd ..
