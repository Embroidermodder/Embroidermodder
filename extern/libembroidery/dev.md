# DXF

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

# SVG

Converted from the table at:
[w3 standard](https://www.w3.org/TR/SVGb/types.html#ColorKeywords)
NOTE: This supports both UK and US English names, so the repeated values aren't an error.

# Report pattern

Report_pattern function that prints everything we know about the pattern at this
point including details specific to the manufacturer like hoop size. Rather
than scattershot details reported by each pattern.
