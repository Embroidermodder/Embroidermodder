# Libembroidery for Python3

Copyright (c) 2018-2025 The Embroidermodder Team

https://www.libembroidery.org

.. WARNING::
   Currently broken and under construction: this is a test distribution.
   Please wait for the beta release.

libembroidery is a low-level library for reading, writing, 
and altering digital embroidery files in C. `libembroidery` in Python
is a hand-guided translation of the C code for Python applications.

libembroidery is the underlying library that is used by
[Embroidermodder 2](http://www.libembroidery.org)
and is developed by [The Embroidermodder Team](#the-embroidermodder-team).
It handles over 45 different embroidery specific formats as well
as several non-embroidery specific vector formats.

If you want to learn more about building, using and developing this software read the
manual (included as the file [embroidermodder_manual.pdf](embroidermodder_manual.pdf)).

## Usage

Would be something like:

    import embroidery as emb

    p = emb.Pattern()
    p.add_circle([2, 4], 5, color="blue")
    p.save("circle.dst", emb.FORMAT_DST)

## Donate

This library is currently broken: but it is part of a suite of tools that
is part of the Embroidermodder 2 project which are at varying degrees of
completion.

To support this and other Embroidermodder projects see
[https://opencollective.com/embroidermodder](our Open Collective page).

