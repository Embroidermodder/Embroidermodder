#!/bin/bash

VERSION="embroidermodder2.0.0-alpha"

set -e

mkdir build
cd build

if [ "$1" = "Ubuntu" ]; then

    sudo apt-get update
    sudo apt-get install git build-essential cmake qt6-base-dev qml-qt6 libqt6widgets6 \
        qt6-declarative-dev qt6-scxml-dev linguist-qt6 qt6-tools-dev qt6-tools-dev-tools \
        libqt6printsupport6 libqt6core6 libwayland-dev libxkbcommon-dev libxrandr-dev \
        libxinerama-dev libxcursor-dev libxi-dev libglew-dev libglfw3-dev libgl-dev libgl1-mesa-dev libglx-dev \
        libpython3-dev

    cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ..

elif [ "$1" = "MacOS" ]; then

    brew install qt6 qwt glew

    cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ..

elif [ "$1" = "Windows" ]; then

# This is failing CI because of a bug in aqt.
#    QT_VERSION="6.5.0"
#
#    python -m pip install -U pip --upgrade pip
#    pip install aqtinstall
#    python -m aqt install-qt windows desktop "${QT_VERSION}" win64_mingw
#
#    QT_DIR="`pwd`/${QT_VERSION}/mingw_64"
#    export PATH="${QT_DIR}/bin:${PATH}"
#    export CMAKE_PREFIX_PATH="${QT_PATH}"

    vcpkg install qtbase glew glfw3

    export GLEW_INCLUDE_DIRS="C:/vcpkg/packages/glew_x64-windows/include"
    export GLEW_LIBRARIES="C:/vcpkg/packages/glew_x64-windows/lib"

    cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ..

fi

cmake --build .
rm -fr CMakeFiles* embroidermodder2_autogen extern *.cmake Makefile
cd ..

exit 0

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

