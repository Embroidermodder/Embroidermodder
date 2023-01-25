echo "Windows Build"

mkdir build
cd build
cmake -G "MinGW Makefiles" ..
MinGW32-make.exe -j4
cd ..
