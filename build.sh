#!/bin/bash

BUILD_DIR="build"
BUILD_TYPE="Release"
VERSION="2.0.0-alpha"
QT_VERSION="6.5.0"

set -e

function help_msg() {

    echo <<"EOF"
Usage: ./build.sh [OPTIONS]
A script to build, debug and package Embroidermodder 2.
If no options are supplied, print this message.

Options:
  -b, --build            Build the software using cmake.
  -d, --debug            Build the debug version of the software.
  -D, --dependencies     Install dependencies for the argument supplied.
  -h, --help             Display this message.
  -m, --memory           Call a program within valgrind (UNTESTED).
  -t, --test             Run a battery of tests.

EXAMPLES

Install dependencies for MacOS, build the release version:

    $ ./build.sh -D MacOS
    $ ./build.sh -b

Download the latest GH artifacts and assemble the release:

    $ ./build.sh

EOF

}

function get_dependencies() {

    if [ "$1" = "Ubuntu" ]; then

        sudo apt-get update
        sudo apt-get install git build-essential cmake qt6-base-dev qml-qt6 libqt6widgets6 \
            qt6-declarative-dev qt6-scxml-dev linguist-qt6 qt6-tools-dev qt6-tools-dev-tools \
            libqt6printsupport6 libqt6core6 libgl-dev libgl1-mesa-dev libglx-dev \
            lcov

    elif [ "$1" = "MacOS" ]; then

        brew install qt6 qwt

    elif [ "$1" = "Windows" ]; then

        python -m pip install -U pip --upgrade pip
        pip install aqtinstall
        python -m aqt install-qt windows desktop "${QT_VERSION}" win64_mingw

        QT_DIR="`pwd`/${QT_VERSION}/mingw_64"
        export PATH="${QT_DIR}/bin:${PATH}"
        export CMAKE_PREFIX_PATH="${QT_PATH}"

    fi

    git submodule init
    git submodule update

}

function package() {

    cd ${BUILD_DIR}

    rm -fr CMake* cmake* CPack* *_autogen

    if [ "$1" == "Windows" ]; then

        ./${QT_VERSION}/mingw_64/bin/windeployqt6.exe embroidermodder2.exe
        rm -fr ${QT_VERSION}

    fi

    cd ..

}

function package_ci() {

    cd staging
    RUN_ID=`gh run list --workflow build.yml --limit 1 --json databaseId | jq -r .[0].databaseId`
    gh run download $RUN_ID -n Windows -n Ubuntu
    cd ..

}

function build() {

    mkdir -p ${BUILD_DIR}
    cd ${BUILD_DIR}
    cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" ..
    cmake --build .
    cd ..

}

function build_ci() {

    get_dependencies $1

    build

    package $1

}

function coverage() {

    BUILD_DIR="cov"
    REPORT_DIR="cov_report"

    rm -fr $BUILD_DIR
    mkdir $BUILD_DIR
    cd $BUILD_DIR
    cmake .. -DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_CXX_FLAGS_DEBUG="-g -fprofile-arcs -ftest-coverage"
	-DCMAKE_C_FLAGS_DEBUG="-g -fprofile-arcs -ftest-coverage"
    make -j4
    echo "Zero up lcov counters."
    lcov -z
    echo "Running testing script on a 2 minute timeout for use in CI."
    timeout 120 ./embroidermodder2 -t
    cd ..

    rm -fr $REPORT_DIR
    mkdir $REPORT_DIR
    cd $REPORT_DIR
    lcov -c -d ../ --no-extern --exclude ../extern -o em2.info
    genhtml --ignore-errors inconsistent --ignore-errors corrupt em2.info
    cd ..

    lcov --ignore-errors inconsistent --ignore-errors corrupt --summary $REPORT_DIR/em2.info &> summary.txt
    cat summary.txt

}

function collate_lists() {

    OUTPUTFILE="TODO"
    #include the line number in output
    #do not search temporary, object, and moc files
    GREPCMD="grep --line-number --recursive --exclude=*.*~ --exclude=*.o --exclude=moc*.cpp --exclude=*Makefile* --exclude=*TODO* --exclude=*generate-todo* --exclude=*_memleak*"
    DIRECTORY="./src"
    TODOSTRING="TODO"
    BUGSTRING="BUG"
    HACKSTRING="HACK"
    WARNSTRING="WARNING"
    NOTESTRING="NOTE"

    rm -f ${OUTPUTFILE}
    echo "==================================================" >> ${OUTPUTFILE}
    echo "This list was generated on:" >> ${OUTPUTFILE}
    date >> ${OUTPUTFILE}
    echo "==================================================" >> ${OUTPUTFILE}
    echo "" >> ${OUTPUTFILE}

    echo "==================================================" >> ${OUTPUTFILE}
    echo "TODO" >> ${OUTPUTFILE}
    echo "==================================================" >> ${OUTPUTFILE}
    ${GREPCMD} ${TODOSTRING} ${DIRECTORY} >> ${OUTPUTFILE}
    echo "" >> ${OUTPUTFILE}

    echo "==================================================" >> ${OUTPUTFILE}
    echo "BUG" >> ${OUTPUTFILE}
    echo "==================================================" >> ${OUTPUTFILE}
    ${GREPCMD} ${BUGSTRING} ${DIRECTORY} >> ${OUTPUTFILE}
    echo "" >> ${OUTPUTFILE}

    echo "==================================================" >> ${OUTPUTFILE}
    echo "HACK" >> ${OUTPUTFILE}
    echo "==================================================" >> ${OUTPUTFILE}
    ${GREPCMD} ${HACKSTRING} ${DIRECTORY} >> ${OUTPUTFILE}
    echo "" >> ${OUTPUTFILE}

    echo "==================================================" >> ${OUTPUTFILE}
    echo "WARNING" >> ${OUTPUTFILE}
    echo "==================================================" >> ${OUTPUTFILE}
    ${GREPCMD} ${WARNSTRING} ${DIRECTORY} >> ${OUTPUTFILE}
    echo "" >> ${OUTPUTFILE}

    echo "==================================================" >> ${OUTPUTFILE}
    echo "NOTE" >> ${OUTPUTFILE}
    echo "==================================================" >> ${OUTPUTFILE}
    ${GREPCMD} ${NOTESTRING} ${DIRECTORY} >> ${OUTPUTFILE}
    echo "" >> ${OUTPUTFILE}

}

function memory_leaks() {

    # Call as a prefix:
    #     $ bash build.sh --memory ./embroidermodder2 -t
    # Here the testing flag "-t" is passed to embroidermodder2.

    valgrind --leak-check=full --show-reachable=yes --error-limit=no --suppressions=../valgrind-supp/./valgrind-qt.supp --suppressions=../valgrind-supp/./valgrind-misc.supp --gen-suppressions=all --log-file=_memleak.txt -v "$@"
    cat ./_memleak.txt | ../valgrind-supp/./valgrind-create-suppressions.sh > _memleak.supp

    MEMORYLEAKS=$(cat "_memleak.txt" | grep "All heap blocks were freed -- no leaks are possible")

    echo "=============================="
    if [[ -z "$MEMORYLEAKS" ]]; then
        echo $(cat "_memleak.txt" | grep "LEAK SUMMARY:")
        echo $(cat "_memleak.txt" | grep "definitely lost:")
        echo $(cat "_memleak.txt" | grep "indirectly lost:")
        echo $(cat "_memleak.txt" | grep "possibly lost:")
        echo $(cat "_memleak.txt" | grep "still reachable:")
        echo "Review _memleak.txt for more information."
    else
        echo "No memory leaks found :D"
    fi
    echo "=============================="

}


if [[ "$#" -eq 0 ]]; then
  help_msg
fi

for arg in $@
do
  case "$1" in
    -a | --artifact )
      package_ci
      shift
      ;;
    -b | --build )
      build
      shift
      ;;
    -c | --coverage )
      coverage
      shift
      ;;
    --ci )
      build_ci $2
      shift 2
      ;;
    -d | --dependencies )
      get_dependencies $2
      shift 2
      ;;
    -h | --help )
      help_msg
      shift
      ;;
    -l | --lists )
      collate_lists
      ;;
    -m | --memory )
      memory_leaks
      shift
      ;;
    -p | --package )
      package $2
      shift 2
      ;;
    --package-ci )
      package_ci
      shift
      ;;
    *)
      help_msg
      break;;
  esac
done

