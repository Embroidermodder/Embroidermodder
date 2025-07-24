#!/bin/sh

if [ "$1" = "Ubuntu" ]; then

    sudo apt-get update || exit 11
    sudo apt-get install git build-essential cmake qt6-base-dev libqt6widgets6 \
        libqt6printsupport6 libqt6core6 libgl-dev libgl1-mesa-dev libglx-dev || exit 12

elif [ "$1" = "MacOS" ]; then

    brew install qt6 qwt || exit 13

elif [ "$1" = "Windows" ]; then

    python -m pip install -U pip --upgrade pip || exit 14
    pip install aqtinstall || exit 15
    python -m aqt install-qt windows desktop 6.5.0 win64_mingw || exit 16

    echo "set (CMAKE_PREFIX_PATH \"6.5.0/mingw_64\")" >> config.cmake

fi

git submodule init || exit 2
git submodule update || exit 3

mkdir build || exit 4
cd build
cmake .. || exit 5
cmake --build .
