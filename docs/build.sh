#!/bin/bash

git clone https://github.com/embroidermodder/libembroidery

function build_page () {
pandoc $1 -o stor.html
cat header.html stor.html footer.html > $2
rm stor.html
}

build_page about.md about.html
build_page contact.md contact.html
build_page documentation.md documentation.html
build_page libembroidery/README.md libembroidery.html
build_page index.md index.html
build_page downloads.md downloads.html

pandoc ../README.md \
    embroidermodder_1.90.0_developer_notes.pdf
pandoc documentation.md \
    embroidermodder_1.90.0_user_manual.pdf
pandoc libembroidery/README.md libembroidery_0.1_manual.pdf
