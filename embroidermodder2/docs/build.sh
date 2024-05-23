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

for project in refman user-manual
do
	cd $project
		mkdocs build
	cd ..
	cp -r $project/site site/$project
done

mv site _site