#!/bin/bash

SOURCE_DIR="."
BUILD_DIR="_build"

doxygen
cd docs
sphinx-build -M html $SOURCE_DIR $BUILD_DIR
sphinx-build -M latex $SOURCE_DIR $BUILD_DIR
cd _build/latex
make
cd ../..
cd ..
