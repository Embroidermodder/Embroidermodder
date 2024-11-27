#!/bin/bash

if [ "$1" = "tables" ]; then

echo "CSV table to C file converter."
echo "Only run this tool when you alter the tables in data/."
echo "This file is part of libembroidery."

for file in data/*.csv
do
	python3 bin/thread_tables.py $file > src/${file::-4}.c
done
exit

fi


mkdir build
cd build
cmake ..
cmake --build .

