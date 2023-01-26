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

We're working on moving to the lighter Dear ImGui with GLFW backend, so you'll just need a basic CMake build environment which is detailed below if you're new to that build method.

All other dependencies are included into the source code in the `extern/` directory.

(Note that this doesn't cover the thumbnailer, which requires KDE.)

#### Windows

We currently use the following tools, but any equivalent CMake build environment should work:

1. [MinGW Build Environment](https://www.mingw-w64.org/downloads/) for g++, make etc.
2. [Git for Windows](https://gitforwindows.org/) for version control.
3. [CMake](https://cmake.org/download/).
4. A good text edtior. For Windows I'd recommend [Notepad++](https://notepad-plus-plus.org/downloads/) or [Visual Studio Code](https://code.visualstudio.com/).

Make sure all these tools are in your `PATH`, this may be managed by your installers or you can use the `setx` command as an administrator to alter it.

#### MacOS

_We need someone with a MacBook to test this._

Install XCode, CMake and git.

#### Ubuntu/Debian

The build dependencies can be installed easily by opening a terminal and issuing this command:

     sudo apt-get install git build-essential cmake libgl1-mesa-dev valgrind

#### Fedora

The build dependencies can be installed easily by opening a terminal and issuing this command:

    sudo yum install git gdb gcc-c++ valgrind cmake

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

To Embroidermodder 2, enter the `build/` directory and double-click the executable.

On Windows the install commands are the same, however you need to use a `cmd` terminal with admin priviledges and without `sudo`.

Then you'll be able to run `embroidermodder` from the command line with the command:

    embroidermodder.exe
