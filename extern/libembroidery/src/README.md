Libembroidery v1.0.0-alpha
==========================

Introduction
------------

It is advised that new progammers to our project and people using
libembroidery for non-trivial integrations into their own project start by
reading this file and the header. This library has one header and it does a lot
of heavy lifting to keep libembroidery from having non-standard dependencies and
reducing the number of functions in the API to keep upgrading to future versions
trivial.

If the header does not change outside of comments (and version data) then
users of the library have to change nothing to upgrade their code. If only
constants change then it is likely they won't have to change their code.

Data Storage
------------

The approach here is to make our code friendly towards being included in
embedded, desktop and mobile applications. By making more data managed as
compiler directives we reduce the overall code footprint.

To protect against namespace pollution in more object oriented environments
like mobile applications all new defines start with `EMB_`, all functions
with `emb` and all typedefs with `Emb` . (INCOMPLETE)

A fixed number is always a #define and any list of constant strings is stored
as a `const char *[]` and has identifier integers for each entry. For example
formats have various data associated with them, to access the extension for 
the filetype CSV we can use `emb_ext[EMB_FORMAT_CSV]`.

Geometry System
---------------

Our geometry is designed on supporting 3 key systems which are generally more
complex than what is needed for embroidery. They are DXF, SVG and PostScript.
Each has it's own model for geometry and attempting to support them is the
most complex task involved in writing software to support embroidery. They
are chosen based on the reasoning that each has something to contribute
PostScript is the language of printers, SVG a language of graphic designers
with good open-source support and DXF the language of hardware design.

We want to support: people that want to use their existing embroidery
machines, other software embroidery projects, open-hardware development,
existing graphic designers that want to translate their designs to
embroidery and existing machine embroidery businesses that want to extend
what they can do. Supporting 3 graphical systems is necessary to do this.

Our approach to allow more freedom in altering how we do this without
effecting all these users too much across versions is to assign all
attributes an identifier and all geometric types accross these systems an
identifier. Then we can seperate each task as a task accross the matrix
combinations of these.

### EXAMPLE: GETTING AN ATTRIBUTE

The attribute `EMB_ATTR_AREA` and the geometry
`EMB_CIRCLE` are a valid combination and a user can determine this
in their program by calling `emb_prop_get(EMB_ATTR_AREA, EMB_CIRCLE)` asking
the question "If I attempt to get the area of a circle, is that something
that libembroidery can do?". In this case, it is and it returns 1.

Then to get the area we know that it is a real number and therefore we use
the real number version of getting an attribute of a geometric object:
`emb_ggetr`. Calling `emb_ggetr` on this combination like
`emb_ggetr(circle, EMB_ATTR_AREA)` will return the area calculated from
the radius stored as `circle->object->circle.radius`.

### EXAMPLE: SETTING AN ATTRIBUTE

From the previous example

The Header
----------

The header will be commented to guide you through how the header
works and uses a view on dependency to order everything into:

1. `#include`s
2. `#define`s
3. `typedef`s
4. function prototypes
5. `extern`al data

If you wish to add to this file please use the appropriate section.

Testing
-------

Formats
-------

## DXF

Based on the DraftSight color table. It doesn't appear to be authoratitive
and there isn't much said in the official manual about colors 8-255:
[manual](http://images.autodesk.com/adsk/files/autocad_2012_pdf_dxf-reference_enu.pdf).

A possible "standard" solution here is to use xterm ESC color codes with the first
8 overridden for compatibility. That way a curious user may be able to guess
and be _almost_ right. See this script for how simple those codes are:
[codes](https://gist.github.com/hSATAC/1095100).

The first 8 are:

```
    {   0,   0,   0 }, /*   '0' (BYBLOCK)    */
    { 255,   0,   0 }, /*   '1' (red)        */
    { 255, 255,   0 }, /*   '2' (yellow)     */
    {   0, 255,   0 }, /*   '3' (green)      */
    {   0, 255, 255 }, /*   '4' (cyan)       */
    {   0,   0, 255 }, /*   '5' (blue)       */
    { 255,   0, 255 }, /*   '6' (magenta)    */
    { 255, 255, 255 }, /*   '7' (white)      */
    { 128, 128, 128 }, /*   '8' (dark gray)  */
    { 192, 192, 192 }, /*   '9' (light gray) */
```

## SVG

Converted from the table at:
[w3 standard](https://www.w3.org/TR/SVGb/types.html#ColorKeywords)
NOTE: This supports both UK and US English names, so the repeated values aren't an error.

# Report pattern

Report_pattern function that prints everything we know about the pattern at this
point including details specific to the manufacturer like hoop size. Rather
than scattershot details reported by each pattern.
