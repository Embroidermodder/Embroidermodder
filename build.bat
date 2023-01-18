echo "Windows Build"

git submodule init
git submodule update

mkdir build
cd build
cmake -G "MinGW Makefiles" ..
MinGW32-make.exe -j4
cd ..
xcopy assets build\ /E
