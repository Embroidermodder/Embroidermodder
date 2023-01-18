# EmbroiderBot: The Open Source Embroidery Machine

We're in the process of developing an embroidery machine that is completely
free of patents and runs on software that is also free.

The full cutting list and pricing of the design will come out as we amend it
so hopefully various shops can build the experimental alpha design alongside
our own. Perhaps, in the long run this could be sold as a complete kit similar
to recent open source hardware 3D printer designs, but as with
our other projects we prefer donations to keep the Embroidermodder Team going.

## Software

The software component of EmbroiderBot is a hand translated collection
of assembly macros, functions and data from our sister project
[`libembroidery`](https://github.com/Embroidermodder/libembroidery).
No verbatim code is copied over because, in practice, sharing code between
the desktop, mobile and embedded versions was impractical. However, the
documentation and C functions serve as a good template for the development
of our own user interface.

### The Example Program

For testing purposes, the library is shipped as a working example of how to use
it. So you can alter the EmbroiderBot.ino file, build and make your machine
variant from this starting point.

To build:

    $ make install

installs the arduino-cli program to bin/ folder, then run:

    $ make

It assumes you already have the necessary `LiquidCrystal.h` library installed
which you can install with the standard Arduino framework.

Note that if you want to use arduino then the
`embroidery_embedded` library should work on other C embedded environments
with little alteration.

## Hardware

This is going to be housed in another repository and is currently just a pipe dream.

We intend for this to be buildable from standard parts with a minimum of
waste. For example:

1. The housing will be laser cut plywood, reducing unnecessary plastic waste.
2. The drive will use no more than the required 3 stepper motors and hopefully less.
3. Wooden rack and pinion rather than worm gears to reduce the cost of the build.
4. There will be no locking to a specific thread brand or material: hopefully standard sewing machine cottons should run just as well in it.
5. The sewing head will use repurposed existing parts, so standard bobbins, needles, bobbin cases, free presser foot etc.
