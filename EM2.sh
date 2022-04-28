#!/bin/bash

function linting () {
    python3 -m pip install pylint
    pylint src/ > rating.txt
    pylint src/ -f json > triage.json
    python3 src/triage.py
    python3 src/make_button.py
}

function build_embroider () {
    git submodule init
    git submodule update
    mkdir build
    cd build
    cmake ../src/libembroidery
    cmake --build .
    cd ..
}

function install_dev_libembroidery () {
    git submodule init
    git submodule update
    cd src/libembroidery
    python3 -m build
    python3 -m pip install -U dist/*.whl --force-reinstall
    cd ../..
}

function install_dev_embroidermodder () {
    python3 -m build
    python3 -m pip install -U dist/*.whl --force-reinstall
}

function normal_install () {
    python3 -m pip install libembroidery embroidermodder
}

function run_embroidermodder () {
    normal_install
    python3 -m embroidermodder
}

function dev_run () {
    build_embroider
    install_dev_libembroidery
    install_dev_embroidermodder

    alias embroider="../build/embroider"
    cd src
    python3 -m embroidermodder
}

function testing () {
    build_embroider
    install_dev_libembroidery
    install_dev_embroidermodder
    cd src
    python3 tests.py
    timeout 10 python3 -m embroidermodder
}

function clean_embroidermodder () {
    rm -fr dist src/embroidermodder.egg-info rating.txt triage.json build
}

function help_embroidermodder () {
    cat <<EOF
Please enter a command for EM2 from this list:
    install
    qa
    lint
    test
    dev_install
    run
    dev_run
    clean
EOF
}

case "$1" in
    "di" | "dev_install")
        install_dev_embroidermodder
        ;;

    "t" | "test")
        testing
        ;;

    "l" | "lint")
        linting
        ;;

    "q" | "qa")
        linting
        testing
        ;;

    "i" | "install")
        normal_install
        ;;

    "d" | "dev_run")
        dev_run
        ;;

    "r" | "run")
        run_embroidermodder
        ;;

    "c" | "clean")
        clean_embroidermodder
        ;;

    *)
        help_embroidermodder
        ;;
esac
