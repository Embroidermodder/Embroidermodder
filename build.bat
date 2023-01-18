echo "Windows Build"

git submodule init
git submodule update

mkdir build
cd build
cmake -G "MinGW Makefiles" ..
MinGW32-make.exe -j4
cd ..
copy assets\designs build\
copy assets\docs build\
copy assets\fonts build\
copy assets\icons build\
copy assets\images build\
copy assets\samples build\
copy assets\translations build\
copy assets\config.toml build\
