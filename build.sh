#!/bin/bash

BUILD_DIR="build"
BUILD_TYPE="Release"
GENERATOR="Unix Makefiles"
SYSTEM="linux"
MAJOR="2"
MINOR="0"
PATCH="0"
TAG="alpha"
VERSION="$MAJOR_$MINOR_$PATCH-$TAG"

function run_cmake () {

	cmake -S . -B"$BUILD_DIR" -G"$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cd $BUILD_DIR
	cmake --build .
	cd ..

}

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
			libqt6printsupport6 libqt6core6 libgl-dev libglx-dev libopengl-dev \
                        libglew-dev
		sudo apt upgrade
	fi

}

# build_emscripten_version
# emcc libembroidery/*.c -o embroidery.wasm

function assemble_release () {

	rm -fr em2

	get_dependancies $1

	run_cmake
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

	run_cmake

	cd $BUILD_DIR

	lcov --directory . --capture --output-file em2.info
	gdb -ex=run --ex=quit --args ./embroidermodder2 --cov $TEST_FILES
	genhtml em2.info

	cd ..

}

# This is not intended to be portable and the requirements are whatever is
# necessary since the program builds and ships without this function.
#
# Current requirements: GNU time, clang-tidy, gnuplot, bash,
# standard UNIX tools, python3 and the module: clang_html
#
# https://www.gnu.org/software/complexity/manual/complexity.html
# https://coccinelle.gitlabpages.inria.fr/website/documentation.html
# https://clang.llvm.org/extra/clang-tidy/
function analysis () {

	BUILD_DIR="debug"
	BUILD_TYPE="Debug"

    run_cmake

    echo "Producing clang-tidy report."
    mkdir -p analysis
    FNAME_TIME="`date -Iseconds`"
    REPORT="analysis/clang-tidy-report-$FNAME_TIME.txt"
    clang-tidy --config-file=clang-tidy.txt -p debug src/* &> $REPORT

    echo "Recording output size"
    SEC=`date +%s`
    LEN=`cat $REPORT | wc -l`
    echo "$SEC, $LEN" >> analysis/history.csv

    echo "Plotting"
    gnuplot -e "set terminal png size 400,300; set output 'analysis/progress.png'; set datafile separator \",\"; plot 'analysis/history.csv' using 1:2 with lines"
    python3 -m clang_html $REPORT -o analysis/clang-report.html
}

function convert_to_xpm () {

        folders="icons/default images"

	for folder in icons/default images
	do
	for file in $folder/*
	do
		echo "Converting $file to xpm..."
		stub=`basename $file`
		outfile="src/$folder/${stub::-4}.xpm"
		cfile="src/$folder/${stub::-4}.c"
		convert $file $outfile
		cat docs/copyright_message.txt $outfile > $cfile
		rm $outfile
	done
	done

	xpm_srcfile="src/xpm.c"

	cat docs/copyright_message.txt > $xpm_srcfile

	echo "#include <stddef.h>" >> $xpm_srcfile

	for folder in icons/default images
	do
	for file in `ls src/$folder/*`
	do
		srcfile=`basename $file`
		echo "#include \"$folder/$srcfile\"" >> $xpm_srcfile
	done
	done

	echo "" >> $xpm_srcfile

	echo "char **xpm_icons[] = {" >> $xpm_srcfile
	for folder in icons/default images
	do
	for file in $folder/*
	do
		stub=`basename $file`
		echo "    ${stub::-4}," >> $xpm_srcfile
	done
	done
	echo "    NULL" >> $xpm_srcfile
	echo "};" >> $xpm_srcfile

	echo "" >> $xpm_srcfile

	echo "char *xpm_icon_labels[] = {" >> $xpm_srcfile
	for folder in icons/default images
	do
	for file in $folder/*
	do
		stub=`basename $file`
		echo "    \"${stub::-4}\"," >> $xpm_srcfile
	done
	done
	echo "    NULL" >> $xpm_srcfile
	echo "};" >> $xpm_srcfile

}

if [[ "$#" -eq 0 ]]; then
	cat docs/short_help.txt
fi

for arg in $@
do
	case "$1" in
	--convert-xpm) convert_to_xpm;;
	-a) analysis;;
	-s | --style) code_style;;
	-G | --generator)
		# GENERATOR="$OPTARG"
		echo "This is currently broken: please change the script variable directly."
		;;
	-b | --build) run_cmake;;
	-d | --debug) build_debug;;
	--gnu-linux | --linux | --ubuntu | --ubuntu-latest | --package-linux) assemble_release "linux";;
	--macos | --macos-latest | --package-macos) assemble_release "macos";;
	--windows | --windows-latest | --package-windows) assemble_release "linux";;
	--package) assemble_release "linux";;
	-h | --help) cat docs/help.txt | less;;
	-c | --clean) rm -fr ${BUILD_DIR};;
	*) cat docs/short_help.txt;;
	esac
done

