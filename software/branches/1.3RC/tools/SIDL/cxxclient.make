# Common makefile stub used for building C++ bindings for an interface

LIBNAME = cxxclient
C_SRCS = 
CXX_SRCS = $(STUBSRCS)
LIBLINK = $(CXXLINK)
LTLIBS = ../server/lib$(IFACE)server.la $(BASE_LINK) 
SIDL_LIBS = -lsidlstub_cxx

subdir = /$(IFACE)/$(LIBNAME)
relpath = ../../
include $(relpath)config.make
include $(top_srcdir)/bind.make
