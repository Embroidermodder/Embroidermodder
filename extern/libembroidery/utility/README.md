libembroidery on Arduino
------------------------

This folder contains Arduino specific files for libembroidery.

Compatible Boards
-----------------

We recommend using an Arduino Mega 2560 or another board
with equal or greater specs. That being said, we have had success
using an Arduino Uno R3 but this will likely require further
optimization and other improvements to ensure continued compatibility
with the Uno. See below for more information.

Arduino Considerations
----------------------
There are two main concerns here: Flash Storage & SRAM.

libembroidery continually outgrows the 32KB of Flash storage
on the Arduino Uno and every time this occurs, a decision has to
be made as to what capabilities should be included or omitted. While
reading files is the main focus on arduino, writing files may
also play a bigger role in the future. Long term, it would be most
practical to handle the inclusion or omission of any feature
via a single configuration header file that the user can modify
to suit their needs.

SRAM is in extremely limited supply and it will deplete quickly so
any dynamic allocation should occur early during the setup phase 
of the sketch and sparingly or not at all later in the sketch.
To help minimize SRAM consumption on Arduino and ensure libembroidery
can be used in any way the sketch creator desires, it is required that
any sketch using libembroidery must implement event handlers. See
the ino-event source and header files for more information.

There is also an excellent article by Bill Earl on the
Adafruit Learning System which covers these topics in more depth:
http://learn.adafruit.com/memories-of-an-arduino?view=all

Special Notes
-------------

Due to historical reasons and to remain compatible with the
Arduino 1.0 IDE, this folder must be called "utility".
Refer to the arduino build process for more info:
https://code.google.com/p/arduino/wiki/BuildProcess

libembroidery relies on the Arduino SD library for reading files.
See the ino-file source and header files for more information.
