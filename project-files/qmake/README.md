Master project file
-------------------

This folder contains a file called everything.pro that is used with qmake.
It builds Embroidermodder 2 (gui binary), libembroidery-convert (console binary),
a KDE4 thumbnailer (on linux/unix platforms only).

You will need to refer to the README files in the respective folders for further instructions on usage.

Building
-----------

Open a terminal and type:
```
qmake && make
```

Other files
-----------

This folder also contains linux/unix shell scripts so that after building,
you can easily run them from this folder easily. For example:

To run Embroidermodder 2:
```
./embroidermodder2
```

To run our command line file conversion tool:
```
./libembroidery-convert
```
