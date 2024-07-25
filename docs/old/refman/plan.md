# Simplifying codebase

## Removing Embedded Javascript

A large proportion of the code is present to facilitate the Javascript (ECMAScript) interpreter
that allows Embroidermodder to be extensible. This feature is one of the least important to
the average user and our command line program `embroider` is the appropriate way for users
to automate their work. It also is a feature that makes the source far harder to manage and
much larger.

This is a very complex change, a lot of the planned user interaction was highly dependant on
how Javascript works. For example, using a number that is set to `NaN` to establish that a
step in the user interaction is incomplete. Also Javascript is dynamically typed, so there is
a lot of management of strings, ints and real numbers that doesn't work the same in C/C++.

### Managing modal interactions

When the user adds an ellipse, for example, 3 clicks are required to establish the ellipse
and 1 more click to optionally set its rotation. Each of these are associated with an
enum that is incremented when each click callback is used.

### Combining Contexts

To make the `command_data` table simpler, rather than have context-sensitive commands
have 5 callbacks and context-insensitive commands have 1, we make the callback capable of
detecting the context it has been called in.

## Combining Geometry Management

Libembroidery and Embroidermodder 2 were slightly at odds with how they dealt with geometry:
every libembroidery type having a sister class in EM2. For example EmbArc relates to ArcObject.
This was necessary to allow the EM2 geometry to inherit the `QGraphicsPathItem`
class.

However, since so much of the geometry algorithms and structs are within
libembroidery making these classes less object-oriented. This would mean we don't have to
load all of the geometry in the pattern into `QGraphicsPathItem` copies to render and manipulate
and then dump them all back into the pattern to save.

## Rules surrounding C, C++ and Qt features and functions

The main rule here is to reduce the amount of code necessary to achieve what we want,
seperate code from data to allow alterations to happen entirely in data and reduce the amount of
dependancy that creates on libraries. The overview goes like this:

1. Don't use a Qt function when a C++ will do: it makes us work to Qt's intended style and makes code harder to read for non-Qt favouring developers. (Don't use `qMax`, `qCos` etc.)
2. Don't use a C++ function when a C one will do, it makes the interfacing between C and C++ parts of the source more complicated. (Don't use the `std::` copies.)
3. Don't use polymorphism, it slows down compliation, produces long and hard to interpret compiler messages.
4. Use switch/case whenever the if/else chain is based on an enum (or defines that act as an enum).
5. Use C linkage for any function not in a class that only uses C types.
6. Don't create a class for data that isn't dynamically allocated, for functions that don't use the private data.
7. Avoid using set/get for class data when the data can be public.
8. Reduce the amount of dynamically allocated memory, every allocation creates an oppertunity for invalid memory.
9. Manage enums by setting DEFINEs for consistency.

## Compilation Speed

Ideally the compilation of the entire project, including libembroidery should be so fast that
the development cycle doesn't have to account for it. At the time of writing, the following
commands create this output:

```
$ make clean
$ time `make &> build.log`

real    9m14.716s
user    8m39.204s
sys     0m28.895s
$ make clean
$ time `make -j4 &> build.log`

real    4m30.748s
user    15m5.793s
sys     1m3.419s
```

Note that we are not including the `cmake` time since that is only done once
as setup. If you made an alteration in `src/core.h` and wanted to test it, it would
take you a 4m 30s turnaround. Waiting this long makes the development cycle pretty slow,
if we could cut this in half then that would help.

## Bugs

* `icon24` is being interpreted as `rgb`.
* "Oh Yeah!" button causes crash.
* Other than `zoomin` and `zoomout` zoom commands don't work.
