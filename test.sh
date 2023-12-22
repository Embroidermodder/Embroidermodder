#!/bin/bash

./build.sh debug $1

cd build
timeout 100 ./embroider --test-suite
timeout 10 ./embroidermodder --test
cd ..
