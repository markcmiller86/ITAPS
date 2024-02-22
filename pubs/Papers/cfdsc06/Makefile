viewps: cfdsc06.ps
	ghostview cfdsc06.ps

cfdsc06.ps: cfdsc06.dvi
	dvips -t letter -o cfdsc06.ps cfdsc06.dvi

cfdsc06.dvi: cfdsc06.tex ../biblio/tstt.bib
	latex cfdsc06
	bibtex cfdsc06
	latex cfdsc06
	latex cfdsc06

clean: 
	-rm -f *.aux *.bbl *.blg *.dvi *.log *~
