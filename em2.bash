#!/bin/bash

function em2_help () {
cat <<EOF
Embroidermodder helper script.
------------------------------

Commands:
   help
   mingw-deps
   debian-deps
   build-deps
   build
   install
EOF
}

function build_pc () {
git submodule init
git submodule update

mkdir build
cd build
cmake ..
cmake --build .
ln -s `pwd`/../assets assets
}

function mingw_deps () {
pacman -S gcc cmake git bash \
    mingw-w64-SDL2 mingw-w64-SDL2_image mingw-w64-SDL2_ttf
}

function debian_deps () {
sudo apt install git clang build-essential \
  libgl1-mesa-dev libglu1-mesa-dev \
  libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
}

function build_em2_android () {
#
# Install Linux (64-bit):
#
# https://developer.android.com/studio#downloads
#
# My download link was
#
# https://redirector.gvt1.com/edgedl/android/studio/ide-zips/2021.2.1.15/android-studio-2021.2.1.15-linux.tar.gz
#
# but I'm not sure if this is stable. Extract and run
#
# bin/android.sh
#
# install the SDK to the path ANDROID_HOME or change the path below
#
# We're using the LibSDL 2 build advice.
#
# https://wiki.libsdl.org/Android

# for gradle, 'ndk-build', 'android', 'adb'
export EM2_BUILD=~/em2_android_build
export ANDROID_NDK_HOME=$EM2_BUILD/android-ndk-r10e
export ANDROID_HOME=$EM2_BUILD/android-sdk/tools
export ANDROID_PLATFORM_TOOLS=$EM2_BUILD/android-sdk/platform-tools
export PATH=$ANDROID_NDK_HOME:$ANDROID_HOME:$ANDROID_PLATFORM_TOOLS:$PATH

echo "If you have not got any of the following libraries please use this install line:"
echo "sudo apt install openjdk-8-jdk ant android-sdk-platform-tools-common"
echo ""
echo "Your path for this session is now:"
echo $PATH

if [ ! -d $EM2_BUILD ]; then
mkdir -p $EM2_BUILD
cd $EM2_BUILD
curl \
https://dl.google.com/android/repository/android-ndk-r10e-linux-x86_64.zip \
--output android-ndk.zip
unzip android-ndk.zip

$ANDROID_HOME/bin/sdkmanager
fi

echo "Building Embroidermodder 2"
cp $EM2_BUILD/SDL2/include/SDL.h .
cp $EM2_BUILD/SDL2_image/SDL_image.h .
cp $EM2_BUILD/SDL2_ttf/SDL_ttf.h .
mv libembroidery/* .
SRC=*.c *.h
$EM2_BUILD/SDL2/build-scripts/androidbuild.sh org.embroidermodder $SRC
}

function build_em2_ios () {
echo "iOS Build"
}

function install_em2 () {
# So we can use relative paths for assets
mkdir -p ~/.embroidermodder2
cp -R assets/ build/embroidermodder ~/.embroidermodder2

# Set an alias so we can try the software.
# Long term users will already have this set
# in their "".bashrc".
alias embroidermodder="cd ~/.embroidermodder2; ./embroidermodder"
}

POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
  case $1 in
    help)
      em2_help
      shift
      ;;
    mingw-deps)
      mingw_deps
      shift
      ;;
    debian-deps)
      debian_deps
      shift
      ;;
    build)
      build_pc
      shift
      ;;
    android)
      build_em2_android
      shift
      ;;
    ios)
      build_em2_ios
      shift
      ;;
    install)
      install_em2
      shift
      ;;
    *)
      # Store positional args
      POSITIONAL_ARGS+=("$1")
      shift
      ;;
  esac
done

