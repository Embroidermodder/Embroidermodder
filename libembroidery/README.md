What is libembroidery ?
-----------------------

libembroidery is the underlying library that is used and
developed by [Embroidermodder 2](http://embroidermodder.github.io).
It handles over 45 different embroidery specific formats as well
as several non-embroidery specific vector formats.

WARNING: DO NOT USE this version of libembroidery in your own project,
we have an external version which isn't managed specifically to work with
embroidermodder: see
[our github page](https://github.com/embroidermodder/libembroidery).

Eventually libembroidery will be versioned properly, at which point we'll
switch to using the official current version.

Documentation
-------------

Developer Documentation can be generated using [Doxygen](http://www.doxygen.org)

- For clean docs that only includes documented items (most people):
```
doxygen libembroidery.doxyfile
```
- For verbose docs that includes undocumented items (core developers):
```
doxygen libembroidery-undocumented.doxyfile
```
