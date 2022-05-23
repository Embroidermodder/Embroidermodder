# Embroidermodder 2.0.0 Alpha

(UNDER MAJOR RESTRUCTURING, PLEASE WAIT FOR VERSION 2)

http://www.libembroidery.org

Embroidermodder is a free machine embroidery application.
The newest version, Embroidermodder 2 can:

- edit and create embroidery designs
- estimate the amount of thread and machine time needed to stitch a design
- convert embroidery files to a variety of formats
- upscale or downscale designs
- run on Windows, Mac and Linux

Embroidermodder 2 is very much a work in progress since we're doing a ground
up rewrite to an interface in C using the GUI toolkit SDL2.
The reasoning for this is detailed in the issues tab.

For a more in-depth look at what we are developing read
our [website](https://www.libembroidery.org) which includes these docs as well as the up-to date printer-friendly versions.
These discuss recent changes, plans and has user and developer guides for all the Embroidermodder projects.

To see what we're focussing on right now, see the [Open Collective News](https://opencollective.com/embroidermodder).

## Build and Install

### Desktop

We assume you have a basic build environment (bash, CMake, git and a C compiler).
If you do just run:

    $ bash em2.bash build
    
Otherwise, on Debian Linux/GNU use:

    $ bash em2.bash debian-deps build
    
to install the officially supported versions of the libraries and build
the software (requires sudo) use:

    $ bash em2.bash debian-deps build install

Currently this is the 2.0-alpha, which will have a build code of
some kind.

This helper script also has its own help message that you will get
if you run `em2.bash` without arguments.

#### Windows Specific Advice

This is one of many possible ways to build the software on Windows,
this section is to help people who've not got a build environment to start with.

1. Download and install MSYS2 (follow their instructions): https://www.msys2.org/
2. Boot "Mintty" from the Start menu.
3. Use the commands:

```
$ git clone https://github.com/Embroidermodder/Embroidermodder
$ cd Embroidermodder
$ ./em2.bash mingw_deps build
```

### Mobile

These are currently unsupported (see iMobileViewer and Mobileviewer for
iOS and Android respectively), but after the Desktop version is
released we'll work on them.

The Mobile version will share some of the UI and all of the backend,
so development of the Desktop version will help us make both.

Using SDL we don't need to have a different codebase on mobile and desktop,
just conditional operation depending on the platform:

Android advice:  https://wiki.libsdl.org/Android

iOS advice: https://www.lazyfoo.net/tutorials/SDL/52_hello_mobile/ios_mac/index.php

Originally I considered Kivy for the Python version, but that means getting the bindings
to be reliable which is far more work. Instead we will have a seperate Python version of
libembroidery for other people's projects which is hand translated from the C code.

## Development

If you wish to develop with us you can chat via the contact email
on the [website](https://embroidermodder.org) or in the issues tab on the
[github page](https://github.com/Embroidermodder/Embroidermodder/issues).
People have been polite and friendly in these conversations and I (Robin)
have really enjoyed them.
If we do have any arguments please note we have a
[Code of Conduct](CODE_OF_CONDUCT.md) so there is a consistent policy to
enforce when dealing with these arguments.

The first thing you should try is building from source using the [build advice](#build)
above. Then read some of the [manual](https://embroidermodder.org/docs/documentation) to get the general
layout of the source code and what we are currently planning.

### Testing

To find unfixed errors run the tests by launching from the command line with:

    $ embroidermodder --test

then dig through the output. It's currently not worth reporting the errors, since
there are so many but if you can fix anything reported here you can submit a PR.

### Contributing

#### Funding

The easiest way to help is to fund development (see the Donate button above),
since we can't afford to spend a lot of time developing and only have limited
kit to test out libembroidery on.

#### Programming and Engineering

Should you want to get into the code itself:

* Low level C developers are be needed for the base library `libembroidery`.
* Low level assembly programmers are needed for translating some of `libembroidery` to `EmbroiderBot`.
* Hardware Engineers to help design our own kitbashed embroidery machine `EmbroiderBot`, one of the original project aims in 2013.
* Scheme developers and C/SDL developers to help build the GUI.
* Scheme developers to help add designs for generating of custom stitch-filled emblems like the heart or dolphi. Note that this happens in Embroidermodder not libembroidery (which assumes that you already have a function available).

#### Writing

We also need people familiar with the software and the general
machine embroidery ecosystem to contribute to the
[documentation](https://github.com/Embroidermodder/docs).

We need researchers to find references for the documentation: colour tables,
machine specifications etc. The history is murky and often very poorly maintained
so if you know anything from working in the industry that you can share: it'd be
appreciated!
