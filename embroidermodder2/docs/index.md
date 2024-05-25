---
title: The Embroidermodder Project
description: Free and Open Source Software for Machine Embroidery.
keywords: machine embroidery, embroidery, dst, pes, jef
---

!!! warning

    ( IN ALPHA DEVELOPMENT: NOT YET READY FOR SERIOUS USE. )

Embroidermodder is a free machine embroidery software program.
The newest version, Embroidermodder 2 can:

- edit and create embroidery designs
- estimate the amount of thread and machine time needed to stitch a design
- convert embroidery files to a variety of formats
- upscale or downscale designs
- run on Windows, Mac and Linux

For more in-depth information, see [our website](http://www.libembroidery.org)
and get the manuals [here](http://www.libembroidery.org/documentation).

To try out the software in alpha see our current
[alpha pre-release](https://github.com/Embroidermodder/Embroidermodder/releases).

Various sample embroidery design files can be found in
the src/samples folder.

Embroidermodder is developed by The Embroidermodder Team which is maintained as a
list on the website under ["Credits"](http://www.libembroidery.org/credits).

## Screenshots

If you use multiple operating systems, it's important to choose software that works on all of them.

Embroidermodder 2 runs on Windows, Linux and Mac OS X. Let's not forget the [Raspberry
Pi](http://www.raspberrypi.org).

![features: platforms 1](images/features-platforms-1.png)

### Realistic Rendering

(This feature is currently broken.)

It is important to be able to visualize what a design will look like when stitched and our
pseudo ``3D'' realistic rendering helps achieve this.

Realistic rendering sample \#1:

![features real render 1](images/features-realrender-1.png)

Realistic rendering sample \#2:

![features real render 2](images/features-realrender-2.png)

Realistic rendering sample \#3:

![features real render 3](images/features-realrender-3.png)

Various grid types and auto-adjusting rulers

Making use of the automatically adjusting ruler in conjunction with the grid will ensure your
design is properly sized and fits within your embroidery hoop area.

Use rectangular, circular or isometric grids to construct your masterpiece!

Multiple grids and rulers in action:

![features grid ruler](images/features-grid-ruler-1.png)

### Many measurement tools

Taking measurements is a critical part of creating great designs. Whether you are designing
mission critical embroidered space suits for NASA or some other far out design for your next
meet-up, you will have precise measurement tools at your command to make it happen. You can
locate individual points or find distances between any 2 points anywhere in the design!

Take quick and accurate measurements:

![features measure 1](images/features-measure-1.png)

### Add text to any design

Need to make company apparel for all of your employees with individual names on them? No sweat.
Just simply add text to your existing design or create one from scratch, quickly and easily.
Didn't get it the right size or made a typo? No problem. Just select the text and update it
with the property editor.

Add text and adjust its properties quickly:

![features text 1](images/features-text-1.png)

### Supports many formats

Embroidery machines all accept different formats. There are so many formats available that it
can sometimes be confusing whether a design will work with your machine.

Embroidermodder 2 supports a wide variety of embroidery formats as well as several vector
formats, such as SVG and DXF. This allows you to worry less about which designs you can use.

### Batch Conversion

(Currently this being ported to the `embroider` command line program.)

Need to send a client several different formats? Just use libembroidery-convert, our command
line utility which supports batch file conversion.

There are a multitude of formats to choose from:

![features formats](images/features-formats-1.png)

### Scripting API

The GUI works by emitting internal text commands, so if you want to alter
or add features to the program that aren't as low level as these commands then you
can chain them together in simple scripts. This allows more control over the program than
the GUI can offer.

A (no longer current) Embroidermodder 2 command excerpt:

![scripting screenshot](images/features-scripting-1.png)

## Dependencies

To build Embroidermodder 2 from source you will need at least
[the Embroidermodder 2 source code itself](https://github.com/Embroidermodder/Embroidermodder),
a build environment including [CMake](https://cmake.org) and [Qt](http://www.qt-project.org) (version >= 6.0). For advice on how to get these,
see the following subsections.

You will also need the git submodules, which can be collected by running these lines
from the embroidermodder source directory:

```
git submodule init
git submodule update
```

### Debian/Ubuntu repository packages

The Qt, KDE and Valgrind build dependencies can be installed easily by
opening a terminal and issuing these commands:

```
sudo apt-get update
sudo apt-get install cmake build-essential qt6-base-dev libqt6gui6 libqt6widgets6 libqt6printsupport6 libqt6core6 libgl-dev libglx-dev libopengl-dev
```

### Fedora repository packages

_TODO: This is outdated advice._

The Qt, KDE and Valgrind build dependencies can be installed easily
by opening a terminal and issuing this command:

```
sudo yum install git gdb gcc-c++ qt-devel kdelibs-devel valgrind
```

### Windows (MSYS2)

After installing [MSYS2](https://www.msys2.org), run this command in a MINGW64 shell:

```sh
pacman -S mingw-w64-clang-x86_64-qt6 cmake gcc make git
```

At the time of writing, this will use around 2Gb of disk space. Then continue to [build](#build).

### Windows (Without MinGW or MSYS2)

If you have a development environment and for some reason want to use that over MSYS2 then ensure you run the installers for:

1. CMake: https://cmake.org/download/#latest
2. Qt: http://www.qt-project.org
3. A Text Editor for Code like Visual Studio Code: https://code.visualstudio.com/
4. A C compiler, like `gcc`, `cl`, `clang` or `tcc`.
5. Git Bash: https://gitforwindows.org/
6. A backend for CMake like Ninja: https://ninja-build.org/

Remember to add these to your `PATH` for scripts to use them.

This would give a similar build experience to standard development on Windows, but we recommend you use MSYS2.

Note that our behind-the-scenes Windows build uses Python to get the Qt libraries
[like this](https://github.com/Embroidermodder/libembroidery/blob/main/bin/build.sh).


## Build

Assuming you have the dependencies for your system, on all systems with Bash, the following should work:

```sh
bash build.sh
```

If your system does not have bash, it may still run as sh.
Failing that, try typing each line in in turn like this:

```sh
git submodule init
git submodule update

cmake -S . -B"build" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug"
cd build
cp -r ../assets/* .
cmake --build .
cat build.log
cd ..
```

### Running the Development Version

After building as above, run your own development copy with:

```sh
cd build
./embroidermodder2
```

### Troubleshooting

If you have no luck with the above advice and still want to
run the development alpha, try reading the `build.log` in your
`build/` folder like this:

```sh
cat build/build.log
```

If, after googling keywords from the errors you're still stuck
post and issue on GitHub here: https://github.com/Embroidermodder/Embroidermodder/issues and supply the `build.log` file. If something
comes up a lot then we can add advice here.

## Development

During the alpha phase we mainly need to focus on getting the C bedrock of this project stable before letting more people
put their creations into it. In Beta, non-programming related contributions will be wecomed to the website and reference manual
repositories.

### Getting Involved

Anyone interested in changing Embroidermodder or becoming a contributor should go read our
[manuals](https://libembroidery.org/documentation), [make issues and submit patches](https://github.com/embroidermodder/refman)
as you find them because the project is very weak here. It will also serve as training for submitting patches to the actual
source code where changes are harder to critique and revise.

As for helping with specific bugs submitting an issue on GitHub along with the `debug-####.txt` file generated during
your test run would be the best approach. For longer term techniques see the next section.

### Bug Hunting

The Embroidermodder Project
===========================

.. toctree::
   :maxdepth: 2
   :caption: Contents
   :name: maintoc

   em_user
   mobile_user
   pet_user
   emrm
   credits

.. warning::

   IN ALPHA DEVELOPMENT: NOT READY FOR SERIOUS USE.

Embroidermodder is a free and open source machine embroidery application.
If our project is successful, it will:

  * edit and create embroidery designs
  * estimate the amount of thread and machine time needed to stitch a design
  * convert embroidery files to a variety of formats
  * upscale or downscale designs
  * run on Windows, Mac and Linux

To try out the software in alpha see our `downloads page <https://libembroidery.org/downloads>`_.

Various sample embroidery design files can be found in
the github samples folder.

Screenshots
-----------

If you use multiple operating systems, it's important to choose
software that works on all of them. Embroidermodder 2 runs on Windows,
Linux and Mac OS X. Let's not forget the `Raspberry Pi <https://www.raspberrypi.org>`_.

# Documentation

For all of these manuals (except the `embroider` manpage),
the source code is maintained as part
of the website [here](https://github.com/Embroidermodder/docs).

## The User Manuals

Note that these URLs are maintained as the permalinks.

For all of these user manuals including the ``embroider`` manpage,
the source code is maintained as part
of the libembroidery project ([https://github.com/Embroidermodder/libembroidery](https://github.com/Embroidermodder/libembroidery)).
The documentation, like the code, is mostly common to all subprojects.

* Embroidermodder, EmbroideryMobile, PET: [https://www.libembroidery.org/user-manual](https://www.libembroidery.org/user-manual) ([PDF](https://www.libembroidery.org/em2_user_manual.pdf))
* embroider: [https://www.libembroidery.org/embroider.txt](https://www.libembroidery.org/embroider.txt)

## The Developer Manual

The Embroidermodder Reference Manual (EMRM) is the main developer 
resource found here: [https://www.libembroidery.org/refman](https://www.libembroidery.org/refman) with the printer friendly version here: [https://www.libembroidery.org/downloads/emrm.pdf](https://www.libembroidery.org/downloads/emrm.pdf).

# The Embroidermodder Project Website

This directory contains most of the broader documentation and automation to
stop minor changes flooding each of Embroidermodder's sub-projects. Including
the documentation as webpages for the
[site itself](https://www.libembroidery.org), each subproject's user manual but
not the reference manual.

This frees the other repositories of the minor-commit heavy mundane tasks of
bundling software and separating a "user" build from a "production" build. It
also means that those projects aren't tasked with keeping production history.

For in-depth information about the software please read some of the PDF manual
included in the top level of the repository. Finishing the manual is the current
top priority in order to fascilitate new developers joining the project.

## Ideas

A testing site that is maintained under testing.libembroidery.org so builds
don't go straight to the main landing page.

If this reaches the cap of storage offered by a github repository then we'll
have to think of something else since the version history of the binaries could
quickly become important if we have any regressions.
