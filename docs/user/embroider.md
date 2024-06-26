
```
EMBROIDER(1)                 General Commands Manual                EMBROIDER(1)

NAME
       embroider - a command line program for machine embroidery

SYNOPSIS
       Copyright 2013-2023 The Embroidermodder Team Licensed under the terms of
       the zlib license.

       https://github.com/Embroidermodder/libembroidery
       https://www.libembroidery.org

       Usage: embroider [OPTIONS] fileToRead...

       Conversion:
           -t, --to        Convert all files given to the format specified
                           by the arguments to the flag, for example:
                               $ embroider -t dst input.pes
                           would convert
                           in the same directory the program runs in.

                           The accepted input formats are (TO BE DETERMINED).
                           The accepted output formats are (TO BE DETERMINED).

       Output:
           -h, --help       Print this message.
           -F, --formats     Print help on the formats that embroider can deal
       with.
           -q, --quiet      Only print fatal errors.
           -V, --verbose    Print everything that has reporting.
           -v, --version    Print the version.

       Modify patterns:
           --combine        takes 3 arguments and combines the first
                            two by placing them atop each other and
                            outputs to the third
                               $ embroider --combine a.dst b.dst output.dst

       Graphics:
           -c, --circle     Add a circle defined by the arguments given to the
       current pattern.
           -e, --ellipse    Add a circle defined by the arguments given to the
       current pattern.
           -l, --line       Add a line defined by the arguments given to the
       current pattern.
           -P, --polyline   Add a polyline.
           -p, --polygon    Add a polygon.
           -r, --render     Create an image in PNG format of what the embroidery
       should look like.
           -s, --satin      Fill the current geometry with satin stitches
       according
                            to the defined algorithm.
           -S, --stitch     Add a stitch defined by the arguments given to the
       current pattern.

       Quality Assurance:
               --test       Run the basic test suite.
               --full-test-suite  Run all tests, even those we expect to fail.

                                                                    EMBROIDER(1)
```
