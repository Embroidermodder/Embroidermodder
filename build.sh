#!/bin/bash

BUILD_DIR="build"
BUILD_TYPE="Release"
VERSION="2.0.0-alpha"
GENERATOR="Unix Makefiles"

function help_message () {

cat <<-'EOF'
Usage: ./build.sh [options]
A script to build, debug and package Embroidermodder 2 and its documentation.

Options:
  -D,--docs               Run doxygen and build pdf docs.
  -d,--debug              Build embroidermodder with warnings as errors
                          and run in a debugger.
  -g,--gcov               Compile together gcov output for optimization analysis.
  -b,--dependencies-brew  Install dependencies for Mac OS.
  -a,--dependencies-apt   Install dependencies for systems with the
                          aptitude package manager.
  -h,--help               Describe how to use this script with paging.
     --windows-latest     Prepare GitHub artifact for Windows release.
     --macos-latest       Prepare GitHub artifact for MacOS release.
     --linux-latest       Prepare GitHub artifact for GNU/Linux release.
  -G,--generator          Accepts argument to override the GENERATOR variable.
EOF

}

function long_help_message () {

read -r -d '' OVERVIEW <<-'EOF'

EMBROIDERMODDER 2 DEVELOPER'S SCRIPT

Overview
--------

If you're building the alpha of Embroidermodder in order to try it before the
beta release use:

    ./build.sh --windows
    ./build.sh --macos
    ./build.sh --gnu-linux

depending on your system. Then run without installing using:

    cd embroidermodder2 && ./embroidermodder2

or, on Windows double-click the executable in the embroidermodder2 directory.

If you're building as part of an effort to improve the software, build in the
debug mode then read the gcov output:

    ./build.sh --debug
    # do some user interactions
    ./build.sh --gcov

Then read the .gcov files and debug.txt for information about how your changes
have altered the performance of the software. Note that --debug works without
very GCC specific features but --gcov may not. At some point we'll add specific
callgrind/cachegrind support.

For in-depth information about the software please read some of the PDF manual included
in the top level of the repository. Finishing the manual is the current top priority
in order to fascilitate new developers joining the project.

EOF

    echo "$OVERVIEW" | less

}

function build_release () {

    git submodule init
    git submodule update

    cmake -S . -B"$BUILD_DIR" -G"$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    cd $BUILD_DIR
    cmake --build .
    cd ..

}

function assemble_release () {

#    rm -fr $BUILD_DIR/CMake* $BUILD_DIR/embroidermodder2_autogen $BUILD_DIR/extern
#    rm -fr embroidermodder2/*.cpp embroidermodder2/*.h
#    cp -r $BUILD_DIR/* embroidermodder2
#    mv *manual*pdf embroidermodder2

    cp ../LICENSE.md .

}

function build_docs () {

# Lighter weight style static site generator for the main pages.
#
# build_emscripten_version
#
# git clone https://github.com/embroidermodder/libembroidery
# cd libembroidery
# emcc embroidery.c -o embroidery.wasm
# mv embroidery.wasm ../downloads
# cd ..

    python3 -m pip install --upgrade pip

    pip install mkdocs
    pip install mkdocs-bibtex
    pip install mkdocs-with-pdf
    pip install mkdocs-material
    pip install mkdocs-table-reader-plugin

    cd embroidermodder2

    rm -fr _site

    mkdocs build

    #cd docs
    #    mkdocs build
    #    mv site/emrm*.pdf ../site
    #cd ..

    mv site ../_site

    cd ..
}

function build_debug () {
    TEST_FILES="samples/spiral/spiral5.csv samples/spiral/spiral6.csv"
    TEST_FILES="$TEST_FILES samples/embroidermodder_logo/conflicts/Embroidermodder.DST"
    TEST_FILES="$TEST_FILES samples/shamrockin/shamrockin.dst"

    BUILD_DIR="build/debug"
    BUILD_TYPE="Debug"
    build_release

    cd embroidermodder2

    lcov -z

    gdb -ex=run --ex=quit --args ./embroidermodder2 --cov $TEST_FILES

    mkdir lcov
    lcov -c -d build -o lcov/embroidermodder.info
    cd lcov
    genhtml embroidermodder.info

    cd ..
}

function gcoverage () {

    cd build/debug/CMakeFiles/embroidermodder.dir/

    gcov src/*.gcno

    mkdir results
    mv *.gcov results
    mv results ../../../..

}

# Run packagers with
# curl https://raw.githubusercontent.com/Embroidermodder/Embroidermodder/master/build.sh
# bash build.sh --package-msi
# etc.

function package_msi () {

    git clone https://github.com/embroidermodder/embroidermodder
    cd embroidermodder

    python -m pip install -U pip --upgrade pip
    pip install aqtinstall
    python -m aqt install-qt windows desktop 6.5.0 win64_mingw

    ls 6.5.0/mingw_64
    echo "set (CMAKE_PREFIX_PATH \"6.5.0/mingw_64\")" >> config.cmake

    GENERATOR="MinGW Makefiles" build_release
    cd $BUILD_DIR
    ../6.5.0/mingw_64/bin/windeployqt embroidermodder2.exe
    #cpack -G WIX
    cd ..
    assemble_release
    powershell Compress-Archive embroidermodder2 embroidermodder_2.0.0-alpha_windows.zip
    mv *.zip ..
    cd ..

}

function package_macos () {

    brew install qt6 qwt

    git clone https://github.com/embroidermodder/embroidermodder
    cd embroidermodder
    build_release
    assemble_release
    tar cf embroidermodder_2.0.0-alpha_macos.tar embroidermodder2
    mv *.tar ..
    cd ..

}

function package_linux () {

    sudo apt update
    sudo apt install qt5-base-dev libqt5gui5 libqt5widgets5 libqt5printsupport5 libqt5core5 libgl-dev libglx-dev libopengl-dev
    sudo apt upgrade

    git clone https://github.com/embroidermodder/embroidermodder
    cd embroidermodder
    build_release
    assemble_release
    tar cf embroidermodder_2.0.0-alpha_linux.tar embroidermodder2
    mv *.tar ..
    cd ..

}

if [[ "$#" -eq 0 ]]; then
  help_message
fi

for arg in $@
do
  case "$1" in
    -s | --style) code_style;;
    -G | --generator)
      # GENERATOR="$OPTARG"
      echo "This is currently broken: please change the script variable directly."
      ;;
    -D | --docs) build_docs;;
    -d | --debug) build_debug;;
    -g | --gcov) gcoverage;;
    --gnu-linux | --linux | --ubuntu | --ubuntu-latest) build_release;;
    --macos | --macos-latest) build_release;;
    --windows | --windows-latest) GENERATOR="MinGW Makefiles" build_release;;
    --package-msi) package_msi;;
    --package-macos) package_macos;;
    --package-linux) package_linux;;
    --package) cd build && cpack;;
    -h | --help) long_help_message;;
    -c | --clean) rm -fr build;;
    build.sh) ;;
    *) help_message;;
  esac
done
