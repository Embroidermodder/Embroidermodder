#!/bin/bash

echo "Build and test libembroidery."

if [ ! -z "${HOST_SYSTEM}" ]; then
    HOST_SYSTEM="linux64"
fi

git clone https://github.com/embroidermodder/libembroidery || exit 1
cd libembroidery || exit 1

mkdir debug || exit 1
cd debug || exit 1
    cmake -DCMAKE_BUILD_TYPE=Debug .. || exit 1
    cmake --build . || exit 1

    if [ $ == "linux64" ]; then
	    sudo cmake --install . || exit 1
    else
        cmake --install . || exit 1
    fi

    ctest --output-junit ${HOST_SYSTEM}_tests.log || exit 1

    pip install junit2html || exit 1

    junit2html ${HOST_SYSTEM}_tests.log || exit 1

    mv ${HOST_SYSTEM}_tests.log.html docs/_static || exit 1
cd ..

mkdir release || exit 1
cd release || exit 1
    cmake -DCMAKE_BUILD_TYPE=Release .. || exit 1
    make || exit 1
    make package || exit 1
    make package source || exit 1
cd .. || exit 1

mkdir analysis
SRC=../embroidery.c

gcc -fdump-tree-all -o embroidery.o -c $SRC
clang $SRC -S -Xclang -dump-tokens &> embroidery.tokens
clang $SRC -S -emit-llvm &> embroidery.llvm
clang $SRC -S -ast-dump &> embroidery.ast-dump
clang $SRC -S -ast-view &> embroidery.ast
clang $SRC -S -mllvm -print-after-all &> embroidery.opt

if [ HOST_SYSTEM = "linux64" ]; then
    tar cf libembroidery-linux64.tar build/libembroidery* build/embroider || exit 1
fi
