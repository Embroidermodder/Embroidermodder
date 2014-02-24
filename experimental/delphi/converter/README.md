This folder contains a port of libembroidery-convert to Delphi/FreePascal.
It should be used as an example if you wish to use libembroidery with Delphi or Lazarus/FreePascal
but should not be considered a replacement for libembroidery-convert and should not be considered stable.

Building on Linux via Terminal
------------------------------
* git clone https://github.com/Embroidermodder/Embroidermodder.git
* cd Embroidermodder/libembroidery
* qmake libembroidery-shared.pro
* make
* cd ../experimental/delphi/converter
* fpc -MDelphi -Scghi -O1 -g -gl -vewnhi -Fu../binding -Fu./ -l libembroidery_convert.dpr
* export LD_LIBRARY_PATH=../../../libembroidery/lib:$LD_LIBRARY_PATH
* ./libembroidery_convert
