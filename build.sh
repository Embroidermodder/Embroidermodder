#!/bin/bash

SRC_DIR="`pwd`"
BUILD_DIR="build"
BUILD_TYPE="Debug"
VERSION="2.0.0-alpha"
GENERATOR="Unix Makefiles"

rm -fr build

git submodule init
git submodule update

cmake -S . -B"$BUILD_DIR" -G"$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cd $BUILD_DIR
cp ../ZLIB-LICENSE.txt .
cp -r ../src/* .
rm -f *.cpp *.h
cmake --build . &> build.log
cat build.log
cd ..
