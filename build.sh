#!/bin/bash

SRC_DIR="`pwd`"
BUILD_DIR="$SRC_DIR/build"
BUILD_TYPE="Release"
VERSION="2.0.0-alpha"
GENERATOR="Unix Makefiles"
PLATFORM="gnu-linux"

if [ "$1" = "debug" ]; then
  BUILD_TYPE="Debug"
  shift
fi

if [ "$1" = "ubuntu-latest" ]; then
  sudo apt update || exit 1
  sudo apt install qt6-base-dev libqt6gui6 libqt6widgets6 libqt6printsupport6 libqt6core6 libgl-dev libglx-dev libopengl-dev || exit 1
  sudo apt upgrade || exit 1
  PLATFORM="$1"
fi

if [ "$1" = "macos-latest" ]; then
  brew install qt6 qwt || exit 1
  PLATFORM="$1"
fi

if [ "$1" = "windows-latest" ]; then
  GENERATOR="MinGW Makefiles"

  python -m pip install -U pip --upgrade pip || exit 1
  pip install aqtinstall || exit 1
  python -m aqt install-qt windows desktop 6.5.0 win64_mingw || exit 1

  ls 6.5.0/mingw_64 || exit 1
  echo "set (CMAKE_PREFIX_PATH \"6.5.0/mingw_64\")" >> config.cmake || exit 1
  PLATFORM="$1"
fi

if [ "$1" = "update" ]; then
  git submodule update --init --update --recursive || exit 1
fi

cd $BUILD_DIR || exit 1
cp -r $SRC_DIR/assets/* . || exit 1
cp $SRC_DIR/ZLIB-LICENSE.txt . || exit 1

cmake -S"$SRC_DIR" -B"$BUILD_DIR" -G"$GENERATOR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"  || exit 1
cmake --build . &> build.log || exit 1
cat build.log || exit 1
rm -fr CMake* src_autogen extern || exit 1
#../6.5.0/mingw_64/bin/windeployqt embroidermodder2.exe || exit 1
#cpack -G WIX

if [ "$1" = "package" ]; then
  if [ "$PLATFORM" = "windows-latest" ]; then
    powershell Compress-Archive $BUILD_DIR $SRC_DIR/embroidermodder_2.0.0-alpha_$2.zip || exit 1
  else
    tar cf $SRC_DIR/embroidermodder_2.0.0-alpha_$2.tar $BUILD_DIR || exit 1
  fi
fi

cd $SRC_DIR || exit 1
