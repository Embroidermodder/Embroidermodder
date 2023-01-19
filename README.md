# What is Embroidermodder ?

Embroidermodder is a free machine embroidery software program.
The newest version, Embroidermodder 2 can:

- edit and create embroidery designs
- estimate the amount of thread and machine time needed to stitch a design
- convert embroidery files to a variety of formats
- upscale or downscale designs
- run on Windows, Mac and Linux

For more information, see [our website](http://www.libembroidery.org).

Embroidermodder and associated projects are built by "The Embroidermodder Team" (see [CREDITS.md](CREDITS.md)).

## Dependencies

To build Embroidermodder 2 from source you will need at least:
- [Embroidermodder 2](https://github.com/Embroidermodder/Embroidermodder)
- [Libembroidery](https://github.com/Embroidermodder/libembroidery) (see below)
- [Qt](http://www.qt-project.org) (version >= 6.0).

Optionally, you may want to add:

- [KDE](http://www.kde.org) for thumbnailer
- [Valgrind](http://www.valgrind.org) for debugging

All other dependencies are included into the source code in the `extern/` directory.

### Libembroidery

The core library libembroidery for Embroidermodder 2 is a git submodule, which means that
they reside in a separate repository. It is necessary for compilation so
if you used ```git clone``` to obtain the Embroidermodder source,
you need to run these commands from the toplevel of
the working tree (which is the Embroidermodder directory):

```
git submodule init
git submodule update
```

This is part of the build script already.

### Qt6

#### On Windows

We recommend using the standard installer: [https://www.qt.io/download-qt-installer](https://www.qt.io/download-qt-installer).

#### Ubuntu/debian repository packages

_This subsection is untested._

The Qt, and Valgrind build dependencies can be installed easily by opening a terminal and issuing this command:

     sudo apt-get install git build-essential cmake libgl1-mesa-dev qtcreator qt6-base qt6-base-dev valgrind

#### Fedora repository packages

_This subsection is untested._

The Qt, and Valgrind build dependencies can be installed easily by opening a terminal and issuing this command:

    sudo yum install git gdb gcc-c++ qt-devel valgrind cmake

## Building

Embroidermodder uses a CMake build, we recommend on Windows to use the script:

     .\build.bat

with the dependencies installed (see below). On all other systems use:

     sh build.sh

## Sample Files

Various sample embroidery design files can be found in
the assets/samples folder.

## Running On Windows

Make sure your Qt installation is in your PATH like this:

    echo "Run as administrator."
    setx /m path "%PATH%;C:\Qt\6.4.1\mingw_64\bin"

Then double click the executable.

## Install/Uninstall

- Linux:
To install, open a terminal in the
project-files/qmake subfolder (after building) and type:
```
sudo make install
```

To uninstall, open a terminal in the
project-files/qmake subfolder and type:
```
sudo make uninstall
```

