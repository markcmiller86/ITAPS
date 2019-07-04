viewps: new-outline.ps
	kghostview new-outline.ps

new-outline.pdf: new-outline.ps
	ps2pdf new-outline.ps

new-outline.ps: new-outline.dvi
	dvips -t letter -o new-outline.ps new-outline.dvi

new-outline.dvi: new-outline.tex intro.tex data-model.tex interface-desc.tex best-practices.tex conclusions.tex appendix.tex ../biblio/tstt.bib
	latex new-outline
	bibtex new-outline
	latex new-outline
	latex new-outline

clean: 
	-rm -f *.aux *.bbl *.blg *.dvi *.log *~
