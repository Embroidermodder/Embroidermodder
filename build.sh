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

	cmake -S . -B"$BUILD_DIR" -G"$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  || exit 1
	cd $BUILD_DIR
	cmake --build . || exit 1
	cd ..

}

function get_dependancies () {

	git submodule init
	git submodule update
	cd extern/libembroidery
	git checkout em2
	cd ../..

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
		sudo apt-get update
		sudo apt-get install \
     git build-essential cmake \
     qt6-base-dev libqt6widgets6 \
			libqt6printsupport6 libqt6core6 libgl-dev libgl1-mesa-dev libglx-dev \
    wayland-scanner
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
# Current requirements: GNU time, clang-tidy, bash,
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
    FNAME_TIME="`date -Iseconds`"
    REPORT="clang-tidy-report-$FNAME_TIME.txt"
    clang-tidy --config-file=clang-tidy.txt -p debug src/* &> $REPORT

    python3 -m clang_html $REPORT -o analysis/clang-report.html
}

function file_as_c_string () {
	stub=`basename $1`
	varname=`echo ${stub::-4} | tr - _`
	echo "const char ${varname}_svg[] = \\" >> $2
	sed 's/\"/\\\"/g;s/^/    "/;s/$/" \\/' $1 >> $2
	echo ";" >> $2
	echo "" >> $2
}

function embed_svgs () {

	files=`ls extern/heroicons/src/24/outline/*.svg images/*.svg`
	outf="src/icons.c"
	cat docs/copyright_message.txt > $outf
	cat <<EOF>>$outf

/* This file is generated by the build.sh script: do not edit directly.
 *
 * The following svgs are the from the heroicons library used under the MIT licence.
 * Copyright (c) Tailwind Labs, Inc.
 */

EOF
	for file in extern/heroicons/src/24/outline/*.svg
	do
		file_as_c_string $file $outf
	done

	cat <<EOF>>$outf

/*
 * The following svgs were created for this project.
 */

EOF

	for file in images/*.svg
	do
		file_as_c_string $file $outf
	done

	echo "const char *svgs[] = {" >> $outf
	for file in $files
	do
		stub=`basename $file`
		varname=`echo ${stub::-4} | tr - _`
		echo "    \"${varname}\", ${varname}_svg," >> $outf
	done
	echo "    \"END\"" >> $outf
	echo "};" >> $outf

}

function reduce_colors () {

	for i in images/*.png
	do
		echo "Reducing colors of \"$i\"."
	        convert $i +dither -colors 16 -depth 4 a.png
        	mv a.png $i
	done

}


function convert_to_xpm () {

	# reduce_colors

	files=`ls images/*.png`
	cat docs/copyright_message.txt > src/xpm.c

	for file in $files
	do
		echo "Converting $file to xpm..."
		stub=`basename $file`
		outf="${stub}.xpm"
		convert $file $outf
		cat $outf >> src/xpm.c
		rm $outf
	done

	echo "char **xpm_icons[] = {" >> src/xpm.c
	for file in $files
	do
		stub=`basename $file`
		echo "    ${stub::-4}_png," >> src/xpm.c
	done
	echo "};" >> src/xpm.c
	echo "" >> src/xpm.c

	echo "char *xpm_icon_labels[] = {" >> src/xpm.c
	for file in $files
	do
		stub=`basename $file`
		echo "    \"${stub::-4}\"," >> src/xpm.c
	done
	echo "    \"END\"" >> src/xpm.c
	echo "};" >> src/xpm.c

}

function full_build () {

	git submodule init
	git submodule update
	convert_to_xpm
	embed_svgs
	run_cmake

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
	--svg) embed_svgs;;
	-G | --generator)
		# GENERATOR="$OPTARG"
		echo "This is currently broken: please change the script variable directly."
		;;
	-b | --build) run_cmake;;
	-f | --full) full_build;;
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

