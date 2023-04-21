\section embroider The `embroider` Command Line Program

\todo Move back to libembroidery now we have the combined docs build.

\subsection pipeline Embroider pipeline

Adjectives apply to every following noun so

    embroider --satin 0.3,0.6 --thickness 2 --circle 10,20,5 \
        --border 3 --disc 30,40,10 --arc 30,50,10,60 output.pes

Creates:

* a circle with properties: thickness 2, satin 0.3,0.6
* a disc with properties: 
* an arc with properties:

in that order then writes them to the output file `output.pes`.

\subsection cli embroider CLI

* Make `-circle` flag to add a circle to the current pattern.
* Make `-rect` flag to add a rectangle to the current pattern.
* Make `-fill` flag to set the current satin fill algorithm for the current geometry. (for example `-fill crosses -circle 11,13,10` fills a circle with center 11mm, 13mm with radius 10mm with crosses).
* Make `-ellipse` flag to add to ellipse to the current pattern.
* Make `-bezier` flag to add a bezier curve to the current pattern.
