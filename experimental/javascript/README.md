This folder contains a port of libembroidery-convert to Javascript using Emscripten.
It should be used as an example if you wish to use libembroidery with Javascript
but should not be considered a replacement for libembroidery-convert and should not be considered stable.

Dependencies
------------
```
sudo apt-get install emscripten
```

To make this work on Windows, you can download the pre-built Enscripten binaries for Windows.

http://kripken.github.io/emscripten-site/docs/getting_started/downloads.html

The internals of Enscripten expect "python2" to be on the path - in Windows you can fix this by copying the existing python in \Program Files\Emscripten\python\2.7.5.3_64bit (this path may vary based on what version you installed and where you installed it).

Building on via Terminal
------------------------------
```
./compile-js && ./libembroidery-convert-js
```
