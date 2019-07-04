# This is the common make file used to build all SIDL bindings (client
# and server).  It is expected to be invoked:
# o With babel.make already created
# o In the subdiretory containing babel.make
# o Two directories below the top directory
# o With FACE defined to be the interface type (i.e. Mesh, Geom, etc.)
# o With LIBNAME defined to be the binding name (i.e. server, cclient, cxxclient, etc.)
# o With C_SRCS defined to be C source files to be included in the lib
# o With CXX_SRCS defined to be C++ source files
# o With LTLIBS defined to be internal libraries to link with
# o With LIBLINK defined to be $(LINK) or $(CXXLINK) or whatever

include babel.make
#include babel.make.depends

IFACE = i$(FACE)
TARGET_LIB = lib$(IFACE)$(LIBNAME).la
INCLUDES = -I.. -I$(srcdir)/.. \
           -I../../iBase -I$(srcdir)/../../iBase \
	   -I../../iBase/$(LIBNAME) \
           -I../.. -I$(srcdir)/../.. \
           -I. \
	   $(SIDL_INCLUDES) \
	   $(SIDL_INCLUDES)/cxx \
	   $(SIDL_INCLUDES)/f77 \
	   $(SIDL_INCLUDES)/f90 \
	   $(SIDL_INCLUDES)/java \
	   $(BABEL_JNI_INCLUDE)


LTCXXCOMPILE = $(LIBTOOL) --tag=CXX --mode=compile $(CXX) $(DEFS) $(EXTRA_DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(CXXFLAGS)

LTCOMPILE = $(LIBTOOL) --tag=CC --mode=compile $(CC) $(DEFS) $(EXTRA_DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)

LINK = $(LIBTOOL) --tag=CC --mode=link $(CC) $(CFLAGS)  $(LDFLAGS) -o $@

CXXLINK = $(LIBTOOL) --tag=CXX --mode=link $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@

SOURCES = $(C_SRCS) @(CXX_SRCS)
OBJECTS = $(C_SRCS:.c=.lo) $(CXX_SRCS:.cxx=.lo)
HEADERS = $(IMPLHDRS) $(IORHDRS) $(STUBHDRS)
INSTALL_HDRS = $(STUBHDRS) $(STUBDOCS) $(STUBFORTRANINC)

all: $(TARGET_LIB)

install: install-lib install-headers

$(TARGET_LIB): $(OBJECTS) $(LTLIBS)
	$(LIBLINK) -rpath $(libdir) $(OBJECTS) $(LTLIBS) -L$(SIDL_LIBDIR) $(SIDL_LIBS)

install-lib: $(TARGET_LIB)
	test -d "$(DESTDIR)$(libdir)" || $(mkdir_p) "$(DESTDIR)$(libdir)"
	$(LIBTOOL) --mode=install $(install_sh) $(INSTALL_STRIP_FLAG) $< '$(DESTDIR)$(libdir)/$<'

install-headers: $(INSTALL_HDRS)
	test -d "$(DESTDIR)$(includedir)" || $(mkdir_p) "$(DESTDIR)$(includedir)"
	$(install_sh) -C -m 644 $(INSTALL_HDRS) $(DESTDIR)$(includedir)

debug:
	@echo "FACE =       $(FACE)"
	@echo "IFACE =      $(IFACE)"
	@echo "LIBNAME =    $(LIBNAME)"
	@echo "TARGET_LIB = $(TARGET_LIB)"
	@echo "LTLIBS =     $(LTLIBS)"

clean:
	rm -rf .libs 
	rm -f $(OBJECTS) 
	rm -f $(OBJECTS:.lo=.o)
	rm -f $(IORHDRS)
	rm -f $(IORSRCS)
	rm -f $(SKELSRCS)
	rm -f $(STUBHDRS)
	rm -f $(STUBSRCS)
	rm -f $(STUBDOCS)
	rm -f $(STUBFORTRANINC)
	rm -f $(TARGET_LIB)
	rm -f babel.make babel.make.depends babel.make.package

check:

.cxx.lo: $(HEADERS)
	$(LTCXXCOMPILE) -c -o $@ $<

.cc.lo: $(HEADERS)
	$(LTCXXCOMPILE) -c -o $@ $<

.c.lo: $(HEADERS)
	$(LTCOMPILE) -c -o $@ $<

.SUFFIXES: .lo .cc .cxx .c .java .o

.PHONY: all install install-lib install-headers clean debug
