viewps: overview.ps
	ghostview overview.ps

overview.ps: overview.dvi
	dvips -t letter -o overview.ps overview.dvi

overview.dvi: overview.tex intro.tex simulation.tex interface.tex geometry.tex adapt_loop.tex improve.tex conclusions.tex
	latex overview
	bibtex overview
	latex overview
	latex overview

clean: 
	-rm -f *.aux *.bbl *.blg *.dvi *.log *~
