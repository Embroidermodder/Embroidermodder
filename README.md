What is Embroidermodder ?
-------------------------

Embroidermodder is a free machine embroidery software program.
The newest version, Embroidermodder 2 can:

- edit and create embroidery designs
- estimate the amount of thread and machine time needed to stitch a design
- convert embroidery files to a variety of formats
- upscale or downscale designs
- run on Windows, Mac and Linux

For more information, see [the sourceforge project page](http://www.embroidermodder.sourceforge.net).

Dependencies
------------

To build Embroidermodder 2 from source you will need at least:
- [Embroidermodder 2](https://github.com/Embroidermodder/Embroidermodder)
- [Qt](http://www.qt-project.org) (version >= 4.8).

Optionally, you may want to add:

- [KDE](http://www.kde.org) for thumbnailer
- [Valgrind](http://www.valgrind.org) for debugging

Building
--------

Various methods of building can be found in the project-files subfolder.
For most builds, it is as simple as opening a terminal in the
project-files/qmake subfolder and typing:
qmake && make

Build time can be considerably decreased on multi-core machines, for example:

- Dual Core: qmake && make -j2
- Quad Core: qmake && make -j4

Sample Files
------------

Various sample embroidery design files can be found in
the embroidermodder2/samples folder.
 
