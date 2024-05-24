#!/bin/bash
#
# Lighter weight style static site generator for the main pages.
#
# build_emscripten_version
#
# git clone https://github.com/embroidermodder/libembroidery
# cd libembroidery
# emcc embroidery.c -o embroidery.wasm
# mv embroidery.wasm ../downloads
# cd ..

python3 -m pip install --upgrade pip

pip install mkdocs
pip install mkdocs-bibtex
pip install mkdocs-with-pdf
pip install mkdocs-material
pip install mkdocs-table-reader-plugin

rm -fr site

mkdocs build

cd docs
    mkdocs build
    mv site/emrm*.pdf ../site
cd ..

mv site ../_site
