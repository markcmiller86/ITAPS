# Common makefile stub used for building python bindings for an interface

LIBNAME = pyclient
C_SRCS = $(STUBSRCS)
CXX_SRCS = 
LIBLINK = $(LINK)
LTLIBS = ../server/lib$(IFACE)server.la $(BASE_LINK)
SIDL_LIBS = 

subdir = /$(IFACE)/$(LIBNAME)
relpath = ../../
include $(relpath)config.make

SETUP_BUILD = 	--include-dirs="../server" \
		--include-dirs="../../iBase/server" \
		--include-dirs="../../iBase/pyclient" \
		--include-dirs="$(SIDL_INCDIR)" \
		--include-dirs="$(SIDL_INCDIR)/python$(BABEL_PY_VERSION)/babel" \
		--include-dirs="$(SIDL_INCDIR)/python$(BABEL_PY_VERSION)/llnl_babel" \
		--include-dirs="$(SIDL_INCDIR)/python$(BABEL_PY_VERSION)/llnl_babel_sidl_sidlx" \
		--library-dirs="$(SIDL_LIBDIR)" \
		--library-dirs="$(SIDL_LIBDIR)/python$(BABEL_PY_VERSION)/site-packages"

SETUP_INSTALL =	--root="$(DESTDIR)" \
		--prefix="$(prefix)" \
		--exec-prefix="$(exec_prefix)" \
		--install-lib="$(libdir)" \
		--install-headers="$(incluedir)" \
		--install-data="$(datadir)" \
		--install-scripts="$(bindir)" 


all:
	CPPFLAGS="$(CPPFLAGS) $(BABEL_PY_INCLUDE)" python setup.py build $(SETUP_BUILD)

install: all
	python setup.py install $(SETUP_INSTALL)
	
.PHONY: all install
