

all:
	texi2any --html embroidermodder.texi
	texi2any --latex embroidermodder.texi
	texi2any embroidermodder.texi

	# Run three times to ensure that the cross references are properly
	# generated and inserted.
	pdflatex embroidermodder.tex
	pdflatex embroidermodder.tex
	pdflatex embroidermodder.tex

clean:
	rm -f embroidermodder.log
	rm -f embroidermodder.aux
	rm -f embroidermodder.out
	rm -f embroidermodder.toc
	rm -f embroidermodder.tex

	# Indexing Files
	rm -f cp.*

maintainer-clean: clean
	rm -f embroidermodder.pdf
	rm -f embroidermodder.info
	rm -f embroidermodder.html
	rm -fr embroidermodder_html
