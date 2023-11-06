#!/bin/bash

TEST_FILES="samples/spiral/spiral5.csv samples/spiral/spiral6.csv"
TEST_FILES="$TEST_FILES samples/embroidermodder_logo/conflicts/Embroidermodder.DST"
TEST_FILES="$TEST_FILES samples/shamrockin/shamrockin.dst"

#bash build.sh

lcov -z

cd build
gdb -ex=run --ex=quit --args ./embroidermodder2 --cov
cd ..

mkdir lcov
lcov -c -d build -o lcov/embroidermodder.info
cd lcov
genhtml embroidermodder.info
