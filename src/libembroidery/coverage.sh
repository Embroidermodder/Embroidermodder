#!/bin/sh
#
# Requires the following additional packages:
#     sudo apt install xdg-utils lcov

rm -fr build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE="Debug" ..
make -j4
./test_libembroidery
mkdir lcov
lcov --capture --directory . --output-file lcov/coverage.info &> cov.txt
genhtml lcov/coverage.info --output-directory lcov
cd lcov
xdg-open index.html
cd ..

tail -n1 test-result.txt
