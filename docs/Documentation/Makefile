all: iBase_userguide.pdf iMesh_userguide.pdf iGeom_userguide.pdf

%.pdf: %.ps
	ps2pdf $*.ps $*.pdf

%.dvi: %.tex
	latex $*
	-bibtex $*
	latex $*
	latex $*

%.ps: %.dvi
	dvips -o $*.ps $*.dvi

view-iBase: iBase_userguide.pdf
	acroread iBase_userguide.pdf

view-iMesh: iMesh_userguide.pdf
	acroread iMesh_userguide.pdf

view-iGeom: iGeom_userguide.pdf
	acroread iGeom_userguide.pdf

clean:
	-rm *~ *.log *.toc *.aux *.bbl *.blg
