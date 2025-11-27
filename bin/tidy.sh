#!/bin/bash

echo "Embroidermodder 2"
echo ""
echo "Tidying code using standard system utilities. Note that we assume the presence of"
echo "GNU sed."

sed '/COMMANDS START \*\//,$d' src/core.h > commands.txt
echo "/* COMMANDS START */" >> commands.txt
sed '1,/COMMANDS START \*\//d;/COMMANDS END \*\//,$d' src/core.h | sort | uniq >> commands.txt
echo "/* COMMANDS END */" >> commands.txt
sed '1,/COMMANDS END \*\//d' src/core.h >> commands.txt

diff src/core.h commands.txt

read -e -p "Do you accept the proposed changes to src/core.h [y/n]?" yes_or_no
if [[ $yes_or_no == [Yy]* ]]; then
    mv commands.txt src/core.h
else
    echo "Ok, skipping."
    rm commands.txt
fi
