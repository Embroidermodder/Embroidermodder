What is Embroidermodder?
------------------------

_IN ALPHA: VERY EXPERIMENTAL_

Embroidermodder is a free machine embroidery software program.
The newest version, Embroidermodder 2 can:

- edit and create embroidery designs
- estimate the amount of thread and machine time needed to stitch a design
- convert embroidery files to a variety of formats
- upscale or downscale designs
- run on Windows, Mac and Linux

For more information, see [our website](http://www.libembroidery.org).

Dependencies
------------

To build Embroidermodder 2 from source you will need at least:
- The [Embroidermodder 2](https://github.com/Embroidermodder/Embroidermodder) source code.
- [Qt](http://www.qt-project.org) (version >= 6.0).
- [GLFW](http://www.glfw.org) (version >= 3.4).
- [OpenGL](http://www.opengl.org) (version >= 2.0).
- [Git](http://www.github.com) for version control.

Optionally, you may want to add:

- [KDE](http://www.kde.org) for thumbnailer.
- [Valgrind](http://www.valgrind.org) for debugging.
- [CMake](http://cmake.org) (version >= 2.0) for build (see Building Without CMake below if this is an issue).

Ubuntu repository packages:
The Qt, KDE and Valgrind build dependencies can be installed easily by opening a terminal and issuing this command:

```
sudo apt-get install git build-essential qt6-dev-tools libqt6-opengl-dev \
    kdelibs5-dev valgrind cmake
```

Fedora repository packages:
The Qt, KDE and Valgrind build dependencies can be installed easily by opening a terminal and issuing this command:

```
sudo yum install git gdb gcc-c++ qt-devel kdelibs-devel valgrind
```

Building
--------

Various methods of building can be found in the project-files subfolder.
For most builds, it is as simple as opening a terminal in the
project folder and typing:

```
mkdir build
cd build
cmake ..
make -j4
```

When building for Fedora:
Substitute qmake-qt4 for qmake.

When building for Windows:
If you are using Qt/Mingw, substitute mingw32-make for make.
If you are using Qt/MSVC, substitute nmake for make.
You may need to add the directory where those executables are located to your system path.
It is recommended that when targeting Windows, that you should omit the -j switch completely,
otherwise build errors may occur unless you are building an individual pro file.

On non-KDE environments you might want to omit `thumbnailer-kde4` building. Follow these steps **before** running `qmake && make`:

### Building Without Cmake

We're working on a one-liner for systems without cmake.

```
# FIXME:
$(CC) -o embroidermodder2 \
    src/*.c src/*/*.c src/*.h src/*/*.h \
    -lglfw -lGL -lm
```

Help Files
------------
The help files for Embroidermodder 2 are part of the website and are maintained
as a seperate repository. To get up-to date versions use the commands:

```
git submodule init
git submodule update
```

If you downloaded a zip file of the Embroidermodder source rather than using git,
you will need to:

- Download the [help files](https://github.com/Embroidermodder/Embroidermodder.github.io/archive/master.zip)
- Unzip the archive
- Rename the "Embroidermodder.github.io-master" directory to "help"
- Copy the "help" directory to "Embroidermodder-master/embroidermodder2/help"

Sample Files
------------

Various sample embroidery design files can be found in
the embroidermodder2/samples folder.

Install/Uninstall
-----------------

- Linux:
To install, open a terminal in the
project-files/qmake subfolder and type:

```
sudo make install
```

To uninstall, open a terminal in the
project-files/qmake subfolder and type:

```
sudo make uninstall
```
