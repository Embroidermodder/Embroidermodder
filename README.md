What is Embroidermodder?
------------------------

_IN ALPHA: VERY EXPERIMENTAL_

[![Build Windows](https://github.com/Embroidermodder/Embroidermodder/actions/workflows/build-windows.yml/badge.svg)](https://github.com/Embroidermodder/Embroidermodder/actions/workflows/build-windows.yml) [![Build MacOS](https://github.com/Embroidermodder/Embroidermodder/actions/workflows/build-macos.yml/badge.svg)](https://github.com/Embroidermodder/Embroidermodder/actions/workflows/build-macos.yml) [![Build Linux](https://github.com/Embroidermodder/Embroidermodder/actions/workflows/build-linux.yml/badge.svg)](https://github.com/Embroidermodder/Embroidermodder/actions/workflows/build-linux.yml)

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
- [OpenGL](http://www.opengl.org) (version >= 2.0).

Optionally, you may want to add:

- [KDE](http://www.kde.org) for thumbnailer.
- [Valgrind](http://www.valgrind.org) for debugging.
- [CMake](http://cmake.org) (version >= 2.0) for build (see Building Without CMake below if this is an issue).

Ubuntu repository packages:
The Qt, KDE and Valgrind build dependencies can be installed easily by opening a terminal and issuing this command:

```
sudo apt-get install git build-essential qt6-dev-tools libqt6-opengl-dev \ 
    valgrind cmake
```

Fedora repository packages:
The Qt, KDE and Valgrind build dependencies can be installed easily by opening a terminal and issuing this command:

```
sudo yum install git gdb gcc-c++ qt-devel kdelibs-devel valgrind
```

### Included

Some of the dependencies have been included and CMake will default to these versions.

- [GLFW](http://www.glfw.org)

Building
--------

Various methods of building can be found in the project-files subfolder.
For most builds, it is as simple as opening a terminal in the
project folder and typing:

```
bash build.sh -b
```

### Building Without Bash

```sh
git clone https://github.com/embroidermodder/embroidermodder
cd embroidermodder

mkdir build
cd build
cmake ..
cmake --build .
```

Then run with

```sh
./embroidermodder2
```

### Building Without Cmake

We're working on a one-liner for systems without cmake.

```sh
# FIXME:
$(CC) -o embroidermodder2 \
    src/*.c src/*/*.c src/*.h src/*/*.h \
    -lglfw -lGL -lm
```

### Testing One-liner (Bash-only)

Sometimes an issue you're having is fixed in the current version: it is recommended that you use a fresh build to ensure that the problem is still present before reporting this. This one-liner helps with this: place it in your `.bashrc` for future use.

```sh
alias run_current_em2="rm -f build.sh && wget https://raw.githubusercontent.com/Embroidermodder/Embroidermodder/refs/heads/main/build.sh && bash build.sh --current"
```

Then, if you have issues, you can capture the terminal output like this.

```sh
run_current_em2 &> build.log
```

If there is a new issue arising from this test script itself, this is also valuable to create an [issue](https://github.com/embroidermodder/embroidermodder/issues) for.

## Help Files

A copy of the reference manual and user manual will be included in the source code in
`docs/` folder. Currently these are works in progress and aren't usable. Some notes that
may be helpful are in markdown files in this directory.

## Sample Files

Various sample embroidery design files can be found in
the `embroidermodder2/docs/samples` folder.

## Install/Uninstall

Embroidermodder 2 is self contained, so it does not need to be installed. If you move
the `embroidermodder2` executable to any other folder it should run exactly the same.
