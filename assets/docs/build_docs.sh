#!/bin/sh

pdflatex embroidermodder_2.0_manual.tex
bibtex embroidermodder_2.0_manual.aux
makeindex embroidermodder_2.0_manual.idx
pdflatex embroidermodder_2.0_manual.tex
pdflatex embroidermodder_2.0_manual.tex

rm -f *.aux *.bbl *.blg *.idx
rm -f *.ilg *.ind *.log *.out *.toc

