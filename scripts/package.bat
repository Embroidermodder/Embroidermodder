
git submodule init
git submodule update

mkdir build
cd build
cmake -G"MinGW Makefiles" ..
cmake --build .
mkdir assets
cd ..

xcopy assets build\assets\ /E/H
copy LICENSE.md build\
copy README.md build\

