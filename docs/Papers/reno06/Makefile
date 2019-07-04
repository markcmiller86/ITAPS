viewps: reno06.ps
	ghostview reno06.ps

reno06.ps: reno06.dvi
	dvips -t letter -o reno06.ps reno06.dvi

reno06.dvi: reno06.tex intro.tex data-model.tex interface-desc.tex best-practices.tex conclusions.tex appendix.tex ../biblio/tstt.bib
	latex reno06
	bibtex reno06
	latex reno06
	latex reno06

clean: 
	-rm -f *.aux *.bbl *.blg *.dvi *.log *~
