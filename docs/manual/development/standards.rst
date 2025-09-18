Code Standards
==============

A basic set of guidelines to use when submitting code.

Naming Conventions
------------------

Name variables and functions intelligently to minimize the need for comments.
It should be immediately obvious what information it represents.
Short names such as x and y are fine when referring to coordinates.
Short names such as i and j are fine when doing loops.

Variable names should be "camelCase", starting with a lowercase word followed by uppercase word(s).
C++ Class Names should be "CamelCase", using all uppercase word(s).
C Functions that attempt to simulate namespacing, should be "nameSpace_camelCase".

All files and directories shall be lowercase and contain no spaces.

Code Style
----------

Tabs should not be used when indenting. Setup your IDE or text editor to use 4 spaces.

If you use KATE (KDE Advanced Text Editor), modelines are included in our code to enforce 
some of our coding standards. When creating new C/C++ files, please add
the modeline to the bottom of the file followed by a blank line. Always make sure there
is an extra blank line at the end of a file.

When using braces, please put the brace on a new line, unless the code is specially formatted
for easier reading such as a block of one liner if/else statements.

Use exceptions sparingly.

if/else is preferred over switch/case.

Do not use ternary operator (?:) in place of if/else.

Do not repeat a variable name that already occurs in an outer scope.

Version Control 
---------------

Being an open source project, developers can grab the latest code at any time
and attempt to build it themselves. We try our best to ensure that it will build smoothly
at any time, although occasionally we do break the build. In these instances,
please provide a patch, pull request which fixes the issue or open an issue and
notify us of the problem, as we may not be aware of it and we can build fine.

Try to group commits based on what they are related to: features/bugs/comments/graphics/commands/etc...

Comments
--------

When writing code, sometimes there are items that we know can be improved,
incomplete or need special clarification. In these cases, use the types of
comments shown below. They are pretty standard and are highlighted by many editors to
make reviewing code easier. We also use shell scripts to parse the code to find
all of these occurances so someone wanting to go on a bug hunt will be able to
easily see which areas of the code need more love::

    //C++ Style Comments
    //TODO: This code clearly needs more work or further review.
    //BUG: This code is definitely wrong. It needs fixed.
    //HACK: This code shouldn't be written this way or I don't feel right about it. There may a better solution.
    //WARNING: Think twice (or more times) before changing this code. I put this here for a good reason.
    //NOTE: This comment is much more important than lesser comments.

libembroidery is written in C and adheres to C89 standards. This means
that any C99 or C++ comments will show up as errors when compiling with
gcc. In any C code, you must use::

    /* C Style Comments */
    /* TODO: This code clearly needs more work or further review. */
    /* BUG: This code is definitely wrong. It needs fixed. */
    /* HACK: This code shouldn't be written this way or I don't feel right about it. There may a better solution */
    /* WARNING: Think twice (or more times) before changing this code. I put this here for a good reason. */
    /* NOTE: This comment is much more important than lesser comments. */
