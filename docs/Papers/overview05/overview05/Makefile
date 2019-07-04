viewps: paper.ps
	kghostview paper.ps

paper.ps: paper.dvi
	dvips -t letter -o paper.ps paper.dvi

paper.dvi: paper.tex intro.tex simulation.tex interface.tex geometry.tex adapt_loop.tex improve.tex conclusions.tex examples.tex
	latex paper
	bibtex paper
	latex paper
	latex paper

clean: 
	-rm -f *.aux *.bbl *.blg *.dvi *.log *~
