#!/bin/bash

SRC_DIR="`pwd`"

bash build.sh

cd build
gdb -q -ex=r ./embroidermodder2
# \
#    samples/spiral/spiral5.csv \
#    samples/spiral/spiral6.csv \
#    samples/embroidermodder_logo/conflicts/Embroidermodder.DST \
#    samples/shamrockin/shamrockin.dst

mkdir gcov
cd gcov
for code_file in `ls $SRC_DIR/src/*.cpp | xargs -n 1 basename`
do
    gcov -b $SRC_DIR/src/$code_file -o $SRC_DIR/build/CMakeFiles/embroidermodder2.dir/src/$code_file.o
done
