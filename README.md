<h1 align="center">
Embroidermodder 2
</h1>

<h4 align="center">
A graphical user interface for editing machine embroidery files.
</h4>

<p align="center">
<code>v2.0.0-alpha1</code>
<br>
(Under construction, please wait for v2.0.0 release for any serious use.)
<br>
Copyright (c) 2011-2026 Embroidermodder, Jonathan Greig, Josh Varga and all other contributors
<br>
https://www.libembroidery.org
</p>

Embroidermodder is a free machine embroidery software program.
The newest version, Embroidermodder 2 can:

- edit and create embroidery designs
- estimate the amount of thread and machine time needed to stitch a design
- convert embroidery files to a variety of formats
- upscale or downscale designs
- run on Windows, Mac and Linux

For more information, see [our website](http://www.libembroidery.org).

## Building

Generally, you should be able to call the automated build script for a first time build with the
appropriate argument like this:

```bash
# Choose your system out of:
bash bin/build.sh Ubuntu
bash bin/build.sh MacOS
bash bin/build.sh Windows
```

Which will fetch the necessary dependencies. In later builds, calling without an argument is
sufficient:

```bash
bash bin/build.sh
```

For details on how this works on your system keep on reading.

### Dependencies

To build Embroidermodder 2 from source you will need at least:

- [This source code](https://github.com/Embroidermodder/Embroidermodder)
- [Qt](http://www.qt-project.org) (version >= 6.0).
- [CMake](https://cmake.org)

Optionally, you may want to add:

- [KDE](http://www.kde.org) for thumbnailer
- [Valgrind](http://www.valgrind.org) for debugging

#### Ubuntu repository packages

The Qt, KDE and Valgrind build dependencies can be installed easily by opening a terminal and issuing this command:

```bash
sudo apt-get install git build-essential cmake qt6-base-dev libqt6widgets6 \
    libqt6printsupport6 libqt6core6 libgl-dev libgl1-mesa-dev libglx-dev
```

#### Fedora repository packages (UNTESTED)

The Qt, KDE and Valgrind build dependencies can be installed easily by opening a terminal and issuing this command:

```bash
sudo yum install git gdb gcc-c++ qt-devel kdelibs-devel valgrind
```

#### MacOS

```bash
brew install qt6 qwt
```

### Calling CMake

Embroidermodder uses a CMake build exclusively. As of Qt6, CMake is well supported and
your development environement (such as QtCreator) will likely be able to set up the project
from the `CMakeLists.txt`.

A typical build would consist of these commands:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

For an accelerated build, on systems with make you can substitute the last line for:

```bash
make -j4
```

which builds on 4 cores at once.

## Testing

Testing uses the `--test` or `-t` flag: the coverage script has been set up to allow
for testing without human interaction. Call it with:

```bash
# This has only been designed to run on a linux-based system.
bash bin/coverage.sh
```

This automated testing is ran by `test.yml` so we can assure that basic usage doesn't
crash the program. 

## Documentation

See the `docs/` directory. To build run the `make` command.

## Sample Files

Various sample embroidery design files can be found in the `data/samples` folder.

## Install/Uninstall

WARNING: this is untested.

### Linux

To install, open a terminal in the `build/` subfolder and type:

```bash
sudo make install
```

To uninstall, open a terminal in the `build/` subfolder and type:

```bash
sudo make uninstall
```
