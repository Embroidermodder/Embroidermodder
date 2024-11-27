# Libembroidery v1.0-alpha

Copyright (c) 2013-2024 The EmbroiderModder Team

https://www.libembroidery.org

(Under construction, please wait for v1.0 release.)

Libembroidery is a low-level library for reading, writing, 
and altering digital embroidery files in C. It is part of the Embroidermodder Project
for open source machine embroidery.

Libembroidery is the underlying library that is used by [Embroidermodder 2](http://embroidermodder.org)
and is developed by [The Embroidermodder Team](#the-embroidermodder-team).
A full list of contributors to the project is maintained in
[the Embroidermodder 2 github](https://github.com/Embroidermodder/embroidermodder)
in the file `CREDITS.md`.
It handles over 45 different embroidery specific formats as well
as several non-embroidery specific vector formats.

It also includes a CLI called `embroider` that allows for better automation of
changes to embroidery files and will be more up-to date than
the Embroidermodder 2 GUI.

## Documentation

Libembroidery is documented as part of the [Embroidermodder 2.0 manual](https://www.libembroidery.org/embroidermodder_2.0-alpha_manual.pdf). If you need libembroidery for any non-trivial usage or want to contribute to the library we advise you read the appropriate design sections of the manual first. Copies of this manual will be shipped with the packaged version of libembroidery, but to build it we use the Doxyfile in [the Embroidermodder git repository](https://github.com/Embroidermodder/embroidermodder).

For more basic usage, `embroider` should have some in-built help starting with:

```sh
    $ embroider --help
```

### License

Libembroidery is distributed under the permissive zlib licence, see the LICENCE file.

## Demos

We're currently trying out some fill techniques which will be demonstrated here and in the script `qa_test.sh`.

![images/logo-spirals.png](images/logo-spirals.png)

Converts to:

![images/logo_spirals_cross_stitch.png](images/logo_spirals_cross_stitch.png)

### Build

libembroidery and EmbroiderModder 2 use CMake builds
so if you are building the project to use as a library we recommend
you run:

```sh
git clone https://github.com/Embroidermodder/libembroidery
cd libembroidery
cmake .
cmake --build .
cmake --install .
```

This builds both the static and shared versions of the library as well
as the command line program `embroider`.
