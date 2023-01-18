#!/bin/bash

function test_fills () {
echo "$1 test..."

STUB="${1::-4}_stitches"
./build/embroider --fill $1 130 $STUB.csv
./build/embroider --fill $1 130 $STUB.svg
#./build/embroider --render $STUB.csv $STUB.png

STUB="${1::-4}_crosses"
./build/embroider --cross-stitch $1 130 $STUB.csv
./build/embroider --cross-stitch $1 130 $STUB.svg
#./build/embroider --render $STUB.csv $STUB.png
}

echo "Building..."

rm -fr build
mkdir build
cd build
cmake ..
cmake --build .
cd ..

echo "Built in tests..."

./build/embroider --test

echo "Stitch fill tests..."

test_fills demos/donut.png
test_fills demos/logo-spirals.png
test_fills demos/berries.png
test_fills demos/snow-squirrel.png
test_fills demos/great-tit.png
test_fills demos/owl.png
test_fills demos/logo.png
