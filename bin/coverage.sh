#!/bin/sh

BUILD_DIR=cov
REPORT_DIR=cov_report

rm -fr $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
cmake .. -DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_CXX_FLAGS_DEBUG="-g -fprofile-arcs -ftest-coverage"
	-DCMAKE_C_FLAGS_DEBUG="-g -fprofile-arcs -ftest-coverage"
make -j4
echo "Zero up lcov counters."
lcov -z
echo "Running testing script on a 2 minute timeout for use in CI."
timeout 120 ./embroidermodder2 -t
cd ..

rm -fr $REPORT_DIR
mkdir $REPORT_DIR
cd $REPORT_DIR
lcov -c -d ../ --no-extern --exclude ../extern/toml11 --exclude ../extern/lua -o em2.info
genhtml --ignore-errors inconsistent --ignore-errors corrupt em2.info
cd ..

lcov --ignore-errors inconsistent --ignore-errors corrupt --summary $REPORT_DIR/em2.info &> summary.txt
cat summary.txt

