CFLAGS 	= -g -O2 -I.

HDRS	= TSTTmesh.h
SRCC	= TSTTmesh.c TSTTtag.c TSTTentset.c
OBJSC	= TSTTmesh.o TSTTtag.o TSTTentset.o

.SUFFIXES: .c .o

.c.o:
	echo Compiling $<
	-${CC} -c ${CFLAGS} $<

PACKAGE_NAME	= SimpleMesh

STATIC_LIB=lib${PACKAGE_NAME}.a
SHARED_LIB=lib${PACKAGE_NAME}.so

$(STATIC_LIB): $(OBJSC)
	echo Creating static library
	ar ruv $@ $(OBJSC)

$(SHARED_LIB): $(OBJSC)
	echo Creating shared library
	/usr/bin/gcc -fPIC -shared -o $@ $(OBJSC)

all: ${SHARED_LIB} ${STATIC_LIB}

clean:
	-rm -f *.o *~

distclean: clean
	-rm -f $(STATIC_LIB) $(SHARED_LIB)

