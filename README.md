# Embroidermodder 2

Embroidermodder is a free machine embroidery software program.
The newest version, Embroidermodder 2 can:

- edit and create embroidery designs
- estimate the amount of thread and machine time needed to stitch a design
- convert embroidery files to a variety of formats
- upscale or downscale designs
- run on Windows, Mac and Linux

For more information, see [our website](http://www.libembroidery.org).

## Dependencies

To build Embroidermodder 2 from source you will need at least:
- [Embroidermodder 2](https://github.com/Embroidermodder/Embroidermodder)
- [Qt](http://www.qt-project.org) (version >= 4.8).

Optionally, you may want to add:

- [KDE](http://www.kde.org) for thumbnailer
- [Valgrind](http://www.valgrind.org) for debugging

Ubuntu repository packages:
(FIXME) The Qt, KDE and Valgrind build dependencies can be installed easily by opening a terminal and issuing this command:

```
sudo apt-get install git build-essential cmake qt6-base-dev qt6-tools-dev qt6-declarative-dev qt6-scxml-dev valgrind
```

Fedora repository packages:
The Qt, KDE and Valgrind build dependencies can be installed easily by opening a terminal and issuing this command:

```
sudo yum install git gdb gcc-c++ qt-devel kdelibs-devel valgrind cmake
```

## Building

We use a CMake build:

```
mkdir build
cd build
cmake ..
cmake --build .
```

To run the build with a specified number of cores you can use:

```
make -j4
```

instead of `cmake --build .` but this will vary based on what your CMake backends
to.

## Running

From your build folder, run the `embroidermodder2` executable. After building this
is just:

```
./embroidermodder2
```

## Installation

During alpha, installation is deactivated and you have to run from your build folder:
so if you wish to run the program with from a globally accessible folder on Debian you
could use:

```
mkdir -p ~/.embroidermodder2
mv build/* ~/.embroidermodder2
```

FIXME: Then use the supplied `embroidermodder2.desktop` in this folder to launch.

### Sample Files

Various sample embroidery design files can be found in
the samples folder.

