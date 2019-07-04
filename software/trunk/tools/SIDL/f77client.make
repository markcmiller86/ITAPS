# Common makefile stub used for building Fortran 77 bindings for an interface

LIBNAME = f77client
C_SRCS = $(STUBSRCS)
CXX_SRCS = 
LIBLINK = $(LINK)
LTLIBS = ../server/lib$(IFACE)server.la $(BASE_LINK)
SIDL_LIBS = -lsidlstub_f77

subdir = /$(IFACE)/$(LIBNAME)
relpath = ../../
include $(relpath)config.make
include $(top_srcdir)/bind.make
