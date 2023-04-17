#!/bin/bash

BUILD_DIR="build"
BUILD_TYPE="Release"
GENERATOR="Unix Makefiles"

read -r -d '' FLAG_USAGE <<-'EOF'
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

    ./build/embroidermodder

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


function help_message () {

    echo "$FLAG_USAGE"

}

function long_help_message () {

    echo "$OVERVIEW" | less

}

function build_release () {

    rm -fr $BUILD_DIR

    git submodule init
    git submodule update

    cmake -S . -B"$BUILD_DIR" -G"$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    cmake --build $BUILD_DIR &> build.log

}

function build_docs () {

    rm -fr embroidermodder2/help/latex assets/help/html

    doxygen

    cd embroidermodder2/help/latex

    pdflatex -interaction=nonstopmode refman.tex
    makeindex refman.idx
    bibtex refman.aux
    pdflatex -interaction=nonstopmode refman.tex
    pdflatex -interaction=nonstopmode refman.tex
    pdflatex -interaction=nonstopmode refman.tex

    mv refman.pdf ../../../embroidermodder_2.0.0-alpha_manual.pdf

}

function build_debug () {

    BUILD_DIR="build/debug"
    BUILD_TYPE="Debug"
    build_release

    cd embroidermodder2
    gdb -q -ex r --args ./embroidermodder \
      assets/samples/spiral/spiral5.csv \
      assets/samples/spiral/spiral6.csv \
      assets/samples/embroidermodder_logo/conflicts/Embroidermodder.DST \
      assets/samples/shamrockin/shamrockin.dst

}

function gcoverage () {

    cd build/debug/CMakeFiles/embroidermodder.dir/

    gcov src/*.gcno

    mkdir results
    mv *.gcov results
    mv results ../../../..

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
    -h | --help) long_help_message;;
    build.sh) ;;
    *) help_message;;
  esac
done
