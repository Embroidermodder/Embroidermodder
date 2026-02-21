# Contributing

## Table of Contents

* [Source Code Overview](#source-code-overview)
* [Coding Standards](#coding-standards)
* [Progress](#progress)

## Source Code Overview

Throughout the source code we use keywords that are specific to our model of computer
aided design (CAD) so it would help to keep these at hand.

| Term            | Definition |
| --------------- | ---------- |
| _command_       | A CAD instruction issued by the user. All user interaction should break into some sequence of commands.                     |
| _configuration_ | Data that is established early at runtime that defines the behaviour of the program. If data is part of configuration we do not expect to reload it during runtime after this boot process.  |
| _core_          | All processing that is not specific to file input/output or GUI toolkit tasks. |
| _design_        | A command that creates a template in the vector layer. |
| _generator_     | A command that creates a stitch list in the current pattern. |
| _pattern_       | All data associated with a given run of an embroidery machine: this includes all the stitch lists, geometric primatives, designs and fills. On export, a lot of this will be flattened into just stitch lists in most formats. |
| _state_         | A specific memory structure that allows access to all of the accessible data within the core. This does not include GUI toolkit specific data structures. |

### What Belongs Where

Between the GUI code, libembroidery, commands, generators, fills, designs and the C core
it can be unclear what goes where and what to prioritize in the case of a conflict. It
should help to realise that some parts of the source code are concessions to
interoperability:

1. C++ is to allow us to work with Qt,
2. libembroidery is to allow others to take the smallest chunk out for embedded systems.

Refer to the glossary table above for advice on which folder to place your new source
code in.

### Core

The core is for C functions that are in the heart of Embroidermodder that aren't
specifically to do with machine embroidery file I/O and aren't a kind of command.

The main benefits of having the C core are:

1. Maintaining a common global state and associated state manipulation functions that
   are compatible with different embroidermodder frontends. This allows us to have
   alternatives to Qt and share code between the thumbnailer and GUI.
2. Easier debugging due to lack of polymorphism and clearer memory structures.
3. More powerful static code analysis will run effectively on C99 since the language
   is so much simpler.
4. Increased compilation speed given how interconnected the Qt files are and how large
   the headers have to be after the preprocessor has run.

### Compound vs. Core Commands

A compound command is any command that is executed by calling a sequence of more
basic commands. These basic commands that cannot be divided further are the "core commands",
they will always be written in C or C++ and won't use the `call` mechanism within their
source code. Unless otherwise stated it is assumed that a C or C++ function with the suffix
`_cmd` is a core command.

## Coding Standards

A basic set of guidelines to use when submitting code.

### Naming Conventions

* Name variables and functions intelligently to minimize the need for comments.
  It should be immediately obvious what information it represents.
* Short names such as `x` and `y` are fine when referring to coordinates.
* Short names such as `i` and `j` are fine when doing loops.

Variable names should be "camelCase", starting with a lowercase word followed by uppercase word(s).
C++ Class Names should be "CamelCase", using all uppercase word(s).
C Functions that attempt to simulate namespacing, should be "nameSpace_camelCase".

All files and directories shall be lowercase and contain no spaces.

### Code Style

Tabs should not be used when indenting. Setup your IDE or text editor to use 4 spaces.

When using braces, please put the brace on a new line, unless the code is specially formatted
for easier reading such as a block of one liner if/else statements.

Use exceptions sparingly.

if/else is preferred over switch/case.

Do not use ternary operator (?:) in place of if/else.

Do not repeat a variable name that already occurs in an outer scope.

### Version Control 

Being an open source project, developers can grab the latest code at any time
and attempt to build it themselves. We try our best to ensure that it will build smoothly
at any time, although occasionally we do break the build. In these instances,
please provide a patch, pull request which fixes the issue or open an issue and
notify us of the problem, as we may not be aware of it and we can build fine.

Try to group commits based on what they are related to:
features/bugs/comments/graphics/commands/etc...

### Comments

Always use C style comments even in C++ code. We're in the process of changing the
comments over.

When writing code, sometimes there are items that we know can be improved,
incomplete or need special clarification. In these cases, use the types of
comments shown below. They are pretty standard and are highlighted by many editors to
make reviewing code easier. We also use shell scripts to parse the code to find
all of these occurances so someone wanting to go on a bug hunt will be able to
easily see which areas of the code need more love.

```c++
//C++ Style Comments
//TODO: This code clearly needs more work or further review.
//BUG: This code is definitely wrong. It needs fixed.
//HACK: This code shouldn't be written this way or I don't feel right about it. There may a better solution.
//WARNING: Think twice (or more times) before changing this code. I put this here for a good reason.
//NOTE: This comment is much more important than lesser comments.
```

libembroidery is written in C and adheres to C89 standards. This means
that any C99 or C++ comments will show up as errors when compiling with
gcc. In any C code, you must use:

```c
/* C Style Comments */
/* TODO: This code clearly needs more work or further review. */
/* BUG: This code is definitely wrong. It needs fixed. */
/* HACK: This code shouldn't be written this way or I don't feel right about it. There may a better solution */
/* WARNING: Think twice (or more times) before changing this code. I put this here for a good reason. */
/* NOTE: This comment is much more important than lesser comments. */
```

/* TODO: Josh, Review this file and move any info still valid or needing work into TODO comments in the actual
 *       libembroidery code. Many items in this list are out of date and do not reflect the current status of
 *       libembroidery. When finished, delete this file.
 */

## Progress

- Test that all formats read data in correct scale (format details should match other programs)
- Add which formats to work with to preferences.
- Check for memory leaks
- Update all formats without color to check for edr or rgb files
- Fix issues with DST (VERY important that DST work well)

### File Formats

| FORMAT | READ  | WRITE | NOTES |
| ------ | ----- | ----- | ----- |
| `10o`  | YES   |       | read (need to fix external color loading) (maybe find out what ctrl code flags of 0x10, 0x08, 0x04, and 0x02 mean) |
| `100`  |       |       | none (4 byte codes) 61 00 10 09 (type, type2, x, y ?) x & y (signed char) |
| `art`  |       |       | none |
| `bro`  | YES   |       | read (complete)(maybe figure out detail of header) |
| `cnd`  |       |       | none |
| `col`  |       |       | (color file no design) read(final) write(final) |
| `csd`  | YES   |       | read (complete) | 
| `dat`  |       |       | read () |
| `dem`  |       |       | none (looks like just encrypted cnd) |
| `dsb`  | YES   |       | read (unknown how well) (stitch data looks same as 10o) |
| `dst`  | YES   |       | read (complete) / write(unknown) |
| `dsz`  | YES   |       | read (unknown) |
| `dxf`  |       |       | read (Port to C. needs refactored) |
| `edr`  |       |       | read (C version is broken) / write (complete) |
| `emd`  |       |       | read (unknown) |
| `exp`  | YES   |       | read (unknown) / write(unknown) |
| `exy`  | YES   |       | read (need to fix external color loading) |
| `fxy`  | YES   |       | read (need to fix external color loading) |
| `gnc`  |       |       | none |
| `gt`   |       |       | read (need to fix external color loading) |
| `hus`  | YES   |       | read (unknown) / write (C version is broken) |
| `inb`  | YES   |       | read (buggy?) |
| `jef`  | YES   |       | write (need to fix the offsets when it is moving to another spot) |
| `ksm`  | YES   |       | read (unknown) / write (unknown) |
| `pcd`  |       |       |  |
| `pcm`  |       |       |  |
| `pcq`  |       |       | read (Port to C) |
| `pcs`  | BUGGY |       | read (buggy / colors are not correct / after reading, writing any other format is messed up) |
| `pec`  |       |       | read / write (without embedded images, sometimes overlooks some stitches leaving a gap) |
| `pel`  |       |       | none |
| `pem`  |       |       | none |
| `pes`  | YES   |       |  |
| `phb`  |       |       |  |
| `phc`  |       |       |  |
| `rgb`  |       |       |  |
| `sew`  | YES   |       |  |
| `shv`  |       |       | read (C version is broken) |
| `sst`  |       |       | none |
| `svg`  |       | YES   |  |
| `tap`  | YES   |       | read (unknown) |
| `u01`  |       |       |  |
| `vip`  | YES   |       |  |
| `vp3`  | YES   |       |  |
| `xxx`  | YES   |       |  |
| `zsk`  |       |       | read (complete) |

Support for Singer FHE, CHE (Compucon) formats?

