EMBROIDERMODDER 2 DEVELOPER'S SCRIPT

Overview
--------

If you're building the alpha of Embroidermodder in order to try it before the
beta release use:

    ./needle --windows
    ./needle --macos
    ./needle --gnu-linux

depending on your system. Then run without installing using:

    cd embroidermodder2 && ./embroidermodder2

or, on Windows double-click the executable in the embroidermodder2 directory.

If you're building as part of an effort to improve the software, build in the
debug mode then read the gcov output:

    ./needle --debug
    # do some user interactions
    ./needle --gcov

Then read the .gcov files and debug.txt for information about how your changes
have altered the performance of the software. Note that --debug works without
very GCC specific features but --gcov may not. At some point we'll add specific
callgrind/cachegrind support.

For in-depth information about the software please read some of the PDF manual included
in the top level of the repository. Finishing the manual is the current top priority
in order to fascilitate new developers joining the project.
