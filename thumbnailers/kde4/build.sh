#!/bin/sh

echo "Build for POSIX systems."

mkdir build
cd build
cmake ..
cmake --build .
cd ..
