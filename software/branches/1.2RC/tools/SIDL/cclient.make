# Common makefile stub used for building C bindings for an interface

LIBNAME = cclient
C_SRCS = $(STUBSRCS)
CXX_SRCS = 
LIBLINK = $(LINK)
LTLIBS = ../server/lib$(IFACE)server.la $(BASE_LINK) 
SIDL_LIBS = -lsidl

subdir = /$(IFACE)/$(LIBNAME)
relpath = ../../
include $(relpath)config.make
include $(top_srcdir)/bind.make
