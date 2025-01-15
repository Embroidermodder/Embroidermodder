TEX = pdflatex -interaction=nonstopmode -file-line-error
BIB = bibtex
IND = makeindex

default: maintainer-clean emrm.pdf

all: emrm.pdf emrm_us.pdf

github: deps tables all

deps:
	echo "Installing required dependencies..."

	sudo apt-get -y update || exit 1
	sudo apt-get -y upgrade || exit 1
	sudo apt-get -y install texlive texlive-latex-base texlive-latex-extra pandoc &>> deps.log || exit 1

	echo "Dependencies installed."

tables:
	python3 bin/thread_tables.py

%.pdf: %.tex
	$(TEX) $<
	$(IND) $(subst .tex,.aux,$<)
	$(BIB) $(subst .tex,.aux,$<)
	$(TEX) $<
	$(TEX) $<

clean:
	rm -f *.aux *.dvi *.log *.bbl *.blg
	rm -f *.ilg *.ind *.idx *.log *.toc *.out
	rm -f */*.aux

maintainer-clean: clean
	rm -f *.pdf

.PHONY: all clean deps
