#!/bin/bash

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

    ./bin/mkdocs build
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
    build_website_docs
    build_print_docs
    exit
fi

# For running individual build steps when targeting an issue
# effecting one step.
while [ $# -gt 0 ]; do
    case $1 in
    --site) build_static_site;;
    --web) build_website_docs;;
    --print) build_print_docs;;
    *) echo "Unrecognised option $1.";;
    esac
    shift
done
