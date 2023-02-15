
git submodule init
git submodule update

mkdir build
cd build
cmake -G"MinGW Makefiles" ..
cmake --build .
cd ..

xcopy assets build\ /E/H
copy LICENSE.md build\
copy README.md build\

