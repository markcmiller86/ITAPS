# Common makefile stub used for building java bindings for an interface

LIBNAME = javaclient
C_SRCS = $(STUBSRCS)
CXX_SRCS = 
LIBLINK = $(LINK)
LTLIBS = ../server/lib$(IFACE)server.la $(BASE_LINK)
SIDL_LIBS = -lsidlstub_java

subdir = /$(IFACE)/$(LIBNAME)
relpath = ../../
include ../../config.make
include $(top_srcdir)/bind.make

# In addition to the C code compiled by the rules in bind.make,
# we need to compile some java code and create a .jar file.

all: $(IFACE)/client.jar

install: install_jar

install_jar: $(IFACE)/client.jar
	test -d "$(DESTDIR)$(libdir)" || $(mkdir_p) "$(DESTDIR)$(libdir)"
	$(install_sh) -m 644 $(IFACE)/client.jar "$(DISTDIR)$(libdir)/$(IFACE)_client.jar"

$(IFACE)/client.jar:
	cd $(IFACE) && $(MAKE) -f ../$(relpath)jar.make IFACE=$(IFACE) client.jar

.PHONY: install install_jar all
