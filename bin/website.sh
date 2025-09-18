#!/bin/bash

# Conditionally install a package: if the package exists, don't invoke the
# sudo call so we can run this entirely in user mode on systems with the packages
# already installed.
#
# WARNING: This assumes we are running on Debian or a derivative.
function cond_install () {

    if dpkg -s $1 &>/dev/null; then
        echo "$1 already installed"
    else
        sudo apt install $1
    fi

}

function build_static_site () {

    echo "Generating static site..."
    rm -fr site
    mkdir site

    python3 -m pip install --upgrade pip
    python3 -m venv .
    source bin/activate

    ./bin/pip install mkdocs
    ./bin/pip install mkdocs-material

    # Documentation framework
    ./bin/pip install sphinx
    # Sphinx style
    ./bin/pip install furo
    # C docstring extraction
    ./bin/pip install breathe

    ./bin/mkdocs build
    echo "Done."

}

function get_docstrings () {

    echo "Getting documentation strings from code..."
    cond_install python3-clang
    cond_install git
    cond_install doxygen

    git submodule init
    git submodule update

    rm -fr docs/manual/xml
    doxygen
    mv docs/xml docs/manual
    echo "Done."

}

function build_website_docs () {

    echo "Building HTML documentation..."
    cd docs/manual
        make html
        mkdir -p ../../site/docs
        mv _build/html/* ../../site/docs
    cd ../..
    echo "Done."

}

function build_print_docs () {

    echo "Building printable documentation..."
    cd docs/manual
        make latexpdf
        mv _build/latex/embroidermodder.pdf ../../site/docs/emrm_current.pdf
    cd ../..
    echo "Done."

}

if [ $# -lt 1 ]; then
    build_static_site
    get_docstrings
    build_website_docs
    build_print_docs
    exit
fi

# For running individual build steps when targeting an issue
# effecting one step.
while [ $# -gt 0 ]; do
    case $1 in
    --site) build_static_site;;
    --docstr) get_docstrings;;
    --web) build_website_docs;;
    --print) build_print_docs;;
    *) echo "Unrecognised option $1.";;
    esac
    shift
done
