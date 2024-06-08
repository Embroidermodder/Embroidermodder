#!/bin/bash

BUILD_DIR="build"
BUILD_TYPE="Release"
VERSION="2.0.0-alpha"
GENERATOR="Unix Makefiles"
SYSTEM="linux"
VERSION="2_0_0-alpha"

function todo_category () {

	echo "==================================================" >> $2
	echo $1 >> $2
	echo "==================================================" >> $2
	# include the line number in output
	# do not search temporary, object, and moc files
	grep --line-number --recursive --exclude=*.*~ --exclude=*.o \
		--exclude=moc*.cpp --exclude=*Makefile* --exclude=*TODO* \
		--exclude=*generate-todo* --exclude=*_memleak* $1 src >> $2
	echo "" >> $2

}

function todo_report () {

	OUTPUTFILE="TODO"

	rm -f ${OUTPUTFILE}
	echo "==================================================" >> ${OUTPUTFILE}
	echo "This list was generated on:" >> ${OUTPUTFILE}
	date >> ${OUTPUTFILE}
	echo "==================================================" >> ${OUTPUTFILE}
	echo "" >> ${OUTPUTFILE}

	todo_category "TODO" ${OUTPUTFILE}
	todo_category "BUG" ${OUTPUTFILE}
	todo_category "HACK" ${OUTPUTFILE}
	todo_category "WARNING" ${OUTPUTFILE}
	todo_category "NOTE" ${OUTPUTFILE}

}

function valgrind_run () {

	valgrind --leak-check=full --show-reachable=yes --error-limit=no \
		--suppressions=valgrind-supp/valgrind-qt.supp \
		--suppressions=valgrind-supp/valgrind-misc.supp \
		--gen-suppressions=all --log-file=_memleak.txt -v ./embroidermodder2 "$@"
	cat ./_memleak.txt | ./valgrind-supp/valgrind-create-suppressions.sh > _memleak.supp

	MEMORYLEAKS=$(cat "_memleak.txt" | grep "All heap blocks were freed -- no leaks are possible")

	echo "=============================="
	if [[ -z "$MEMORYLEAKS" ]]; then
	    echo $(cat "_memleak.txt" | grep "LEAK SUMMARY:")
	    echo $(cat "_memleak.txt" | grep "definitely lost:")
	    echo $(cat "_memleak.txt" | grep "indirectly lost:")
	    echo $(cat "_memleak.txt" | grep "possibly lost:")
	    echo $(cat "_memleak.txt" | grep "still reachable:")
	    echo "Review _memleak.txt for more information."
	else
	    echo "No memory leaks found :D"
	fi
	echo "=============================="

}

function help_message () {

cat <<-'EOF'
Usage: ./build.sh [options]
A script to build, debug and package Embroidermodder 2 and its documentation.

Options:
  -D,--docs               Run doxygen and build pdf docs.
  -d,--debug              Build embroidermodder with warnings as errors
                          and run in a debugger.
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

function run_cmake () {

	git submodule init
	git submodule update

	cmake -S . -B"$BUILD_DIR" -G"$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
	cd $BUILD_DIR
	cmake --build .
	cd ..

}

# Run packagers with
# curl https://raw.githubusercontent.com/Embroidermodder/Embroidermodder/main/build.sh
# bash build.sh --package-windows
# etc.

function get_dependancies () {

	if [[ $1 = "windows" ]]; then
		python -m pip install -U pip --upgrade pip
	 	pip install aqtinstall
		python -m aqt install-qt windows desktop 6.5.0 win64_mingw

		echo "set (CMAKE_PREFIX_PATH \"6.5.0/mingw_64\")" >> config.cmake
	fi

	if [[ $1 = "macos" ]]; then
		brew install qt6 qwt
	fi

	if [[ $1 = "linux" ]]; then
		sudo apt update
		sudo apt install build-essential cmake qt6-base-dev libqt6gui6 libqt6widgets6 \
			libqt6printsupport6 libqt6core6 libgl-dev libglx-dev libopengl-dev
		sudo apt upgrade
	fi

}

# Lighter weight style static site generator for the main pages.
#
# build_emscripten_version
#
# git clone https://github.com/embroidermodder/libembroidery
# cd libembroidery
# emcc embroidery.c -o embroidery.wasm
# mv embroidery.wasm ../downloads
# cd ..
function build_docs () {

	python3 -m pip install --upgrade pip

	pip install mkdocs
	pip install mkdocs-bibtex
	pip install mkdocs-with-pdf
	pip install mkdocs-material
	pip install mkdocs-table-reader-plugin

	rm -fr _site

	mkdocs build

	#cd docs
	#    mkdocs build
	#    mv site/emrm*.pdf ../site
	#cd ..

	mv site ../_site

}

function assemble_release () {

	rm -fr em2

	get_dependancies $1

	run_cmake
	build_docs
	mkdir em2
	cp $BUILD_DIR/embroidermodder2 em2
	cp LICENSE.md em2
	cp -r docs/ em2
	if [[ $1 = "windows" ]]; then
		GENERATOR="MinGW Makefiles"
		cd em2
		../6.5.0/mingw_64/bin/windeployqt embroidermodder2.exe
		cd ..
		powershell Compress-Archive em2 embroidermodder_${VERSION}_windows.zip
	else
		tar cf embroidermodder_${VERSION}_$1.tar em2
	fi

}

function build_debug () {

	TEST_FILES="samples/spiral/spiral5.csv samples/spiral/spiral6.csv"
	TEST_FILES="$TEST_FILES samples/embroidermodder_logo/conflicts/Embroidermodder.DST"
	TEST_FILES="$TEST_FILES samples/shamrockin/shamrockin.dst"

	BUILD_DIR="debug"
	BUILD_TYPE="Debug"

	git submodule init
	git submodule update

	run_cmake

	cd $BUILD_DIR
	cp -R ../assets/* .

	lcov --directory . --capture --output-file em2.info
	gdb -ex=run --ex=quit --args ./embroidermodder2 --cov $TEST_FILES
	genhtml em2.info

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
    -b | --build) run_cmake;;
    -D | --docs) build_docs;;
    -d | --debug) build_debug;;
    --gnu-linux | --linux | --ubuntu | --ubuntu-latest | --package-linux) assemble_release "linux";;
    --macos | --macos-latest | --package-macos) assemble_release "macos";;
    --windows | --windows-latest | --package-windows) assemble_release "linux";;
    --package) assemble_release "linux";;
    -h | --help) long_help_message;;
    -c | --clean) rm -fr ${BUILD_DIR};;
    build.sh) ;;
    *) help_message;;
  esac
done
