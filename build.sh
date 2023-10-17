#!/bin/bash

SRC_DIR="`pwd`"
BUILD_DIR="build"
BUILD_TYPE="Debug"
VERSION="2.0.0-alpha"
GENERATOR="Unix Makefiles"

git submodule init || exit 1
git submodule update || exit 1

rm -fr $BUILD_DIR || exit 1
cp -r assets $BUILD_DIR || exit 1

cmake -S . -B"$BUILD_DIR" -G"$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"  || exit 1
cd $BUILD_DIR || exit 1
cmake --build . &> build.log || exit 1
cat build.log || exit 1
cd .. || exit 1
