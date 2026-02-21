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

for file in `ls src/*/*.cpp src/*/*.c src/*/*.h`
do
    echo $file
    remove_trailing_space $file
    tabs_to_spaces $file
done

# Report the changes made in this process.
git diff --stat
