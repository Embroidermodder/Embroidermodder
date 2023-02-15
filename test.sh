#!/bin/sh

sh build.sh

cd build
./embroidermodder \
  ../assets/samples/embroidermodder_logo/conflicts/Embroidermodder.DST \
  ../assets/samples/shamrockin/shamrockin.dst \
  ../assets/samples/spiral/spiral5.csv \
  ../assets/samples/spiral/spiral6.csv
