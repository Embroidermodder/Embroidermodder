# What is Embroidermodder ?

(UNDER MAJOR RESTRUCTURING, PLEASE WAIT FOR THE BETA TEST BUILDS.)

[![Build](https://github.com/Embroidermodder/Embroidermodder/actions/workflows/build.yml/badge.svg)](https://github.com/Embroidermodder/Embroidermodder/actions/workflows/build.yml)

http://www.libembroidery.org

Embroidermodder is a free machine embroidery software program.
The newest version, Embroidermodder 2 can:

- edit and create embroidery designs
- estimate the amount of thread and machine time needed to stitch a design
- convert embroidery files to a variety of formats
- upscale or downscale designs
- run on Windows, Mac and Linux

Embroidermodder and associated projects are built by _The Embroidermodder Team_ (see [CREDITS.md](CREDITS.md)).

For more in-depth information read the [manual](https://www.libembroidery.org/embroidermodder_2.0_manual.pdf).

## Screenshots

These screenshots are from the original Qt version.

### Cross Platform

If you use multiple operating systems, it's important to choose software that works on all of them.

Embroidermodder 2 runs on Windows, Linux and Mac OS X. Let's not forget the Raspberry Pi (\url{http://www.raspberrypi.org}).

![features platforms](assets/docs/images/features-platforms-1.png)

### Realistic Rendering

It is important to be able to visualize what a design will look like when stitched and our pseudo ``3D'' realistic rendering helps achieve this.

Realistic rendering sample \#1:

![features real render 1](assets/docs/images/features-realrender-1.png)

Realistic rendering sample \#2:

![features real render 2](assets/docs/images/features-realrender-2.png)

Realistic rendering sample \#3:

![features real render 3](assets/docs/images/features-realrender-3.png)

Various grid types and auto-adjusting rulers

Making use of the automatically adjusting ruler in conjunction with the grid will ensure your design is properly sized and fits within your embroidery hoop area.

Use rectangular, circular or isometric grids to construct your masterpiece!

Multiple grids and rulers in action:

![features grid ruler](assets/docs/images/features-grid-ruler-1.png)

### Many measurement tools

Taking measurements is a critical part of creating great designs. Whether you are designing mission critical embroidered space suits for NASA or some other far out design for your next meet-up, you will have precise measurement tools at your command to make it happen. You can locate individual points or find distances between any 2 points anywhere in the design!

Take quick and accurate measurements:

![features measure 1](assets/docs/images/features-measure-1.png)

### Add text to any design

Need to make company apparel for all of your employees with individual names on them? No sweat. Just simply add text to your existing design or create one from scratch, quickly and easily.
Didn't get it the right size or made a typo? No problem. Just select the text and update it with the property editor.

Add text and adjust its properties quickly:

![text embroidery](assets/docs/images/features-text-1.png)

### Supports many formats

Embroidery machines all accept different formats. There are so many formats available that it can sometimes be confusing whether a design will work with your machine.

Embroidermodder 2 supports a wide variety of embroidery formats as well as several vector formats, such as SVG and DXF. This allows you to worry less about which designs you can use.

### Batch Conversion

Need to send a client several different formats? Just use libembroidery-convert, our command line utility which supports batch file conversion.

There are a multitude of formats to choose from:

![formats](assets/docs/images/features-formats-1.png)

## Current Development Screenshots

We're using ImGui with a GLFW backend for the current build to make a more lightweight build with hopefully more understandable code.
However, the design remains the same, so any features planned for the Qt version should carry over.

### Stitch Simulation

Watch the design you've loaded as a simulation in approximately real time which you can tune to match the speed of your specific model.

![stitch simulation](assets/docs/images/spiral_simulation.gif)

### Auto-Stitch

### Photo-fit

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

## Guide to Contributing to Embroidermodder 2

To find ways to help with the project, [read the manual](https://www.libembroidery.org/embroidermodder_2.0_manual.pdf).

### Code Style

Embroidermodder 2 is written in C++, in a style similar to Python's
[PEP7 style guide](https://peps.python.org/pep-0007/),
which we follow for the C code of `libembroidery`.

### Recieving Credit

Add your name or handle to the end of the [CREDITS.md](CREDITS.md) bulleted list, followed by your GitHub username, a description of your contribution type like "Bug fixes" and an optional quote.
