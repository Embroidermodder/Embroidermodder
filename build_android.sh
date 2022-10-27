#!/bin/bash

cat <<EOF
Assumes that we're building on a unix-like system.

Install Linux (64-bit):

https://developer.android.com/studio#downloads

My download link was

https://redirector.gvt1.com/edgedl/android/studio/ide-zips/2021.2.1.15/android-studio-2021.2.1.15-linux.tar.gz

but I'm not sure if this is stable. Extract and run

bin/android.sh

install the SDK to the path ANDROID_HOME or change the path below

We're using the LibSDL 2 build advice.

https://wiki.libsdl.org/Android
EOF

# for gradle, 'ndk-build', 'android', 'adb'
export EM2_BUILD=~/em2_android_build
export ANDROID_NDK_HOME=$EM2_BUILD/android-ndk-r10e
export ANDROID_HOME=$EM2_BUILD/android-sdk/tools
export ANDROID_PLATFORM_TOOLS=$EM2_BUILD/android-sdk/platform-tools
export PATH=$ANDROID_NDK_HOME:$ANDROID_HOME:$ANDROID_PLATFORM_TOOLS:$PATH

cat <<EOF
If you have not got any of the following libraries please use this install line:
    sudo apt install openjdk-8-jdk ant android-sdk-platform-tools-common

Your path for this session is now:
EOF
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
git submodule init
git submodule update
cp em2.c libembroidery/src/* stb/*.h jni

cd jni
# https://stackoverflow.com/questions/31115218/android-ndk-could-not-find-application-project-directory
$EM2_BUILD/android-ndk-r10e/ndk-build NDK_PROJECT_PATH=`pwd` \
	NDK_APPLICATION_MK=`pwd`/Application.mk \
	APP_BUILD_SCRIPT=`pwd`/Android.mk

