#
# TransFig makefile
#

all: downward.eps second.eps upward.eps 

# translation into eps

downward.tex: downward.eps Makefile
	fig2ps2tex downward.eps >downward.tex
clean::
	rm -f downward.tex

downward.eps: downward.fig Makefile
	fig2dev -L eps downward.fig downward.eps
clean::
	rm -f downward.eps


# translation into eps

second.tex: second.eps Makefile
	fig2ps2tex second.eps >second.tex
clean::
	rm -f second.tex

second.eps: second.fig Makefile
	fig2dev -L eps second.fig second.eps
clean::
	rm -f second.eps


# translation into eps

upward.tex: upward.eps Makefile
	fig2ps2tex upward.eps >upward.tex
clean::
	rm -f upward.tex

upward.eps: upward.fig Makefile
	fig2dev -L eps upward.fig upward.eps
clean::
	rm -f upward.eps

