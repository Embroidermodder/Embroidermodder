echo "Get internal dependencies."

git submodule init
git submodule update

echo "Build"

mkdir build
cd build
rem cmake -DCMAKE_BUILD_TYPE=Debug ..
rem cmake --build .

rem Just build for now: install needs fixing
rem cmake --install .
rem embroider.exe -test
rem Need to fix the executable name?
