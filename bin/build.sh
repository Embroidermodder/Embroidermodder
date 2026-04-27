#!/bin/sh

VERSION="embroidermodder2.0.0-alpha"

set -e

mkdir build
cd build

if [ "$1" = "Ubuntu" ]; then

    sudo apt-get update
    sudo apt-get install git build-essential cmake qt6-base-dev qml-qt6 libqt6widgets6 \
        qt6-declarative-dev qt6-scxml-dev linguist-qt6 qt6-tools-dev qt6-tools-dev-tools \
        libqt6printsupport6 libqt6core6 libgl-dev libgl1-mesa-dev libglx-dev

    cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ..

elif [ "$1" = "MacOS" ]; then

    brew install qt6 qwt

    cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" \
        -DCMAKE_C_COMPILER=/usr/bin/gcc-11 \
        -DCMAKE_CXX_COMPILER=/usr/bin/g++-11 ..

elif [ "$1" = "Windows" ]; then

    QT_VERSION="6.5.0"

    python -m pip install -U pip --upgrade pip
    pip install aqtinstall
    python -m aqt install-qt windows desktop "${QT_VERSION}" win64_mingw

    QT_DIR="`pwd`/${QT_VERSION}/mingw_64"
    export PATH="${QT_DIR}/bin:${PATH}"
    export CMAKE_PREFIX_PATH="${QT_PATH}"

    cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ..

fi

cmake --build .

mkdir "${VERSION}"
mv LICENSE.md help icons images samples translations "${VERSION}"

if [ "$1" = "Ubuntu" ]; then
mv embroidermodder2 "${VERSION}"
tar -czf "${VERSION}-linux.tar.gz" "${VERSION}"
elif [ "$1" = "MacOS" ]; then
mv embroidermodder2 "${VERSION}"
#archive "${VERSION}-macos" "${VERSION}"
elif [ "$1" = "Windows" ]; then
mv embroidermodder2.exe "${VERSION}"
#archive "${VERSION}-windows.zip" "${VERSION}"
fi

cd ..
