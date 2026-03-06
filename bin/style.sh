#!/bin/bash
#
# This program does applies of our code styling or indicates to the user that
# a change is required.
#

function remove_trailing_space () {
    sed -i -e 's/[[:space:]]*$//' "$1"
}

function tabs_to_spaces () {
    sed -i -e 's/\t/    /g' "$1"
}

function unix_line_ending () {
    sed -i -e 's/\r//g' "$1"
}

function keyword_followed_by_space () {
    sed -i -e 's/if(/if (/g' "$1"
    sed -i -e 's/for(/for (/g' "$1"
}

echo "WHITESPACE"
for file in `ls src/*/*.cpp src/*/*.c src/*/*.h`
do
    echo $file
    remove_trailing_space $file
    tabs_to_spaces $file
    unix_line_ending $file
    keyword_followed_by_space $file
done

# Report the changes made in this process.
git diff --stat
