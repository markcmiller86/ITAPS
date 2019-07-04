# Makefile for building Java archive (.jar) for Java client stubs

subdir = /$(IFACE)/javaclient/$(IFACE)
relpath = ../../../
include $(relpath)config.make
include babel.make

SIDL_CLASSPATH=$(SIDL_LIBDIR)/sidl-$(BABEL_VERSION).jar:$(SIDL_LIBDIR)/sidlstub_$(BABEL_VERSION).jar
CLASSPATH=..:$(relpath)iBase/javaclient/iBase/client.jar:$(SIDL_CLASSPATH)

client.jar: .timestamp
	cd .. && $(JAR) -cf $(IFACE)/client.jar $(IFACE)/*.class

.timestamp: $(STUBJAVA)
	rm -f *.class
	CLASSPATH=$(CLASSPATH); export CLASSPATH; \
	  $(JAVAC) $(STUBJAVA)
	touch $@
