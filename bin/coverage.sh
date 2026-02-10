#!/bin/sh

BUILD_DIR=cov
REPORT_DIR=cov_report

if [ "$1" = "Ubuntu" ]; then

    sudo apt-get update || exit 11
    sudo apt-get install git build-essential cmake qt6-base-dev qml-qt6 libqt6widgets6 \
        libqt6printsupport6 libqt6core6 libgl-dev libgl1-mesa-dev libglx-dev \
        lcov || exit 12

elif [ "$1" = "MacOS" ]; then

    brew install qt6 qwt || exit 13

elif [ "$1" = "Windows" ]; then

    python -m pip install -U pip --upgrade pip || exit 14
    pip install aqtinstall || exit 15
    python -m aqt install-qt windows desktop 6.5.0 win64_mingw || exit 16

    echo "set (CMAKE_PREFIX_PATH \"6.5.0/mingw_64\")" >> config.cmake

fi

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
lcov -c -d ../ --no-extern --exclude ../extern -o em2.info
genhtml --ignore-errors inconsistent --ignore-errors corrupt em2.info
cd ..

lcov --ignore-errors inconsistent --ignore-errors corrupt --summary $REPORT_DIR/em2.info &> summary.txt
cat summary.txt

