# Common makefile stub used for building server implementation of an interface

LIBNAME = server
C_SRCS =  $(IORSRCS) $(SKELSRCS) $(IMPLSRCS) $(STUBSRCS)
CXX_SRCS = 
LIBLINK = $(CXXLINK)
LTLIBS = $(BASE_LINK)
SIDL_LIBS = -lsidl
EXTRA_DEFS = $(BABEL_ENUM_ARR_DEFINE)

subdir = /$(IFACE)/$(LIBNAME)
relpath = ../../
include $(relpath)config.make
include $(top_srcdir)/bind.make


# List some extra dependencies for IMPL files.
$(IMPLSRCS:.c=.lo):  ../../sidl_macros.h \
                     ../$(IFACE).h \
                     ../fmangle-$(IFACE).h \
                     ../../iBase/iBase.h
