#!/bin/sh

cd extern/glew/auto
make
make install
cd ..
make
make install
cd ../..
