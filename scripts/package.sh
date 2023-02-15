#!/bin/sh

git submodule init
git submodule update

mkdir build
cd build
cmake ..
cmake --build .
cd ..

cp -r assets build
cp LICENSE.md CREDITS.md build/assets/docs


