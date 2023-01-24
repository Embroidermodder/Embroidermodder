# What is Embroidermodder ?

Embroidermodder is a free machine embroidery software program.
The newest version, Embroidermodder 2 can:

- edit and create embroidery designs
- estimate the amount of thread and machine time needed to stitch a design
- convert embroidery files to a variety of formats
- upscale or downscale designs
- run on Windows, Mac and Linux

Embroidermodder and associated projects are built by _The Embroidermodder Team_ (see [CREDITS.md](CREDITS.md)).

## Documentation and Community

For more information, see [our website](http://www.libembroidery.org) where the manuals are hosted. Embroidermodder 2 is
built on top of our own [libembroidery](https://github.com/Embroidermodder/libembroidery) C library: if you need an
automated solution you'll be better served by the command line tools there.

We don't have a forum so if you have any issues with following the build described here, feel free to post it as a github issue.
If this becomes unworkable we'll set one up.

#### Sample Files

Various sample embroidery design files can be found in
the assets/samples folder.

## Dependencies

To build Embroidermodder 2 from source you will need at least to download this repository and [Qt](http://www.qt-project.org) (version >= 6.0).
We're working on moving to the lighter Dear ImGui with GLFW backend, so eventually you'll just need a basic CMake build environment.

All other dependencies are included into the source code in the `extern/` directory.

(Note that this doesn't cover the thumbnailer, which requires KDE.)

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

## Run and Install/Uninstall

### Linux

To run the build without installing enter the `build/` directory and use:

    ./embroidermodder

To install, open a terminal in the `build/` subfolder (after building) and type:

    sudo cmake --install .

To uninstall, open a terminal in the `build/` subfolder and type:

    sudo cmake --uninstall .

Then you'll be able to run `embroidermodder` from the command line with the command:

    embroidermodder

### Windows

Make sure your Qt installation is in your PATH like this:

    echo "Run as administrator."
    setx /m path "%PATH%;C:\Qt\6.4.1\mingw_64\bin"

Then double click the executable to run.

On Windows the install commands are the same, however you need to use a `cmd` terminal with admin priviledges and without `sudo`.

Then you'll be able to run `embroidermodder` from the command line with the command:

    embroidermodder.exe
