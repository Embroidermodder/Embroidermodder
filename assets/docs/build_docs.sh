#!/bin/sh

pdflatex embroidermodder_2.0_manual.tex
bibtex embroidermodder_2.0_manual.aux
makeindex embroidermodder_2.0_manual.idx
pdflatex embroidermodder_2.0_manual.tex
pdflatex embroidermodder_2.0_manual.tex

rm -f fdl-1.3.aux
rm -f embroidermodder_2.0_manual.aux
rm -f embroidermodder_2.0_manual.bbl
rm -f embroidermodder_2.0_manual.idx
rm -f embroidermodder_2.0_manual.ilg
rm -f embroidermodder_2.0_manual.ind
rm -f embroidermodder_2.0_manual.log
rm -f embroidermodder_2.0_manual.toc
