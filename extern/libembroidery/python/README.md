# Libembroidery v1.0-alpha

Copyright (c) 2013-2022 The EmbroiderModder Team

https://embroidermodder.org

(Under construction, please wait for v1.0 release.)

libembroidery is a low-level library for reading, writing, 
and altering digital embroidery files in C.

libembroidery is the underlying library that is used by [Embroidermodder 2](http://embroidermodder.org)
and is developed by [The Embroidermodder Team](#the-embroidermodder-team).
It handles over 45 different embroidery specific formats as well
as several non-embroidery specific vector formats.

It also includes a CLI called `embroider` that allows for better automation of
changes to embroidery files and will be more up-to date than
the Embroidermodder 2 GUI.

If you want to learn more about building, using and developing this software read the
manual (included as the file [embroidermodder_manual.pdf](embroidermodder_manual.pdf)).

# Libembroidery 1.0 On Python (Alpha)

The Official binding for libembroidery on Python.

This is a stub to save the name on the Python Package Index.

Usage would be something like:

    import libembroidery as emb

    p = emb.createPattern()
    p.add_circle([2, 4], 5, color="blue")
    p.save("circle.dst", emb.FORMAT_DST)

For more see our website: https://embroidermodder.org.

To support this and other Embroidermodder projects see
[https://opencollective.com/embroidermodder](our Open Collective page).

## Installation

We recommend you install the working draft using

    python3 setup.py install --user

from the top level.

In the future we'd recommend that you use the current most
up-to-date version on PyPI using:

    python3 -m pip install libembroidery

or

    py -m pip install libembroidery

on Windows.

## A seperate folder for each language

In theory, for every language that people actively write in a 
well-made binding should be supplied for something as low-level 
as libembroidery. Practically, the Embroidermodder Team will
only know so many languages and for those languages won't necessarily
know about writing bindings.

We can have experts on individual languages take more 
control of the support of this work without it affecting the overall 
support and core library and the levels of completion can be different 
for each language.

While this problem could be solved by the generation of bindings
from a specification like SWIG, we feel that this doesn't account
for the object oriented structure that we want to apply to the
bound functions. Also writing good documentation is helped by
that generation step. Calling conventions may be different between
languages and we'd rather have a human make sure that the documentation
is written with an awareness of the language being written for.

    all:
        swig -Wall -python embroidery.i
        python3 setup.py build
        python3 setup.py install --user
        python3 test.py

    clean:
        rm -fr build dist libembroidery.egg-info __pycache__
        rm -fr libembroidery_core.py embroidery_wrap.c

# Emscripten Version

https://www.sitepoint.com/getting-started-emscripten-transpiling-c-c-javascript-html5/

# Other versions

## Java Version

### Ideas

To support Java perhaps the best approach would be an interface similar
to the command line like a labelled pipe that Java sends instructions
to.

Then when the java program needs to know the current state of the pattern
in RAM it's spit out in a Java byte-code friendly way?

## Swift bindings

Find a source for making the binding.

    # Libembroidery 1.0 binding for Swift
    # Copyright 2022 The Embroidermodder Team
