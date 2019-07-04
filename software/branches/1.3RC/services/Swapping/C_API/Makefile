.SILENT:
include /home/cfog/ITAPS/iMesh-refImpl/iMesh-Defs.inc

PACKAGE_NAME=Swapping

ALL_INCLUDES=$(INCLUDES) $(IBASE_INCLUDES) $(IMESH_INCLUDES) -I.

OPTDebug = -O3 -DNDEBUG

CXX=g++ -fPIC
CC=gcc -fPIC
CFLAGS=-g -O2 $(OPTDebug)
CXXFLAGS=-g -O2 $(OPTDebug)
CPPFLAGS= $(ALL_INCLUDES)
# Yes, Virginia, make and autoconf disagree about the variable names.
FC=@F77@
F90=@FC@
FFLAGS=@FFLAGS@
F90FLAGS=@FCFLAGS@ $(OPTDebug)

LDFLAGS=

.cc.o:
	echo Compiling $<
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(EXTRA_INCLUDES) -c $<

.c.o:
	echo Compiling $<
	$(CC) $(CFLAGS) $(CPPFLAGS) $(EXTRA_INCLUDES) -c $<

.F90.o:
	echo Compiling $<
	$(F90) $(F90FLAGS) $(CPPFLAGS) $(EXTRA_INCLUDES) -c $<

.f90.o:
	echo Compiling $<
	$(F90) $(F90FLAGS) -c $<

.F.o:
	echo Compiling $<
	$(FC) $(FFLAGS) $(CPPFLAGS) $(EXTRA_INCLUDES) -c $<

.f.o:
	echo Compiling $<
	$(FC) $(FFLAGS) -c $<

STATIC_LIB=lib${PACKAGE_NAME}.a
SHARED_LIB=lib${PACKAGE_NAME}.so

INTERNAL_SRCS=ITAPS_Delaunay3D.cc \
ITAPS_InitCanon.cc \
ITAPS_MaxMinSine3D.cc \
ITAPS_MinMaxAngle3D.cc \
ITAPS_Swap2D.cc \
ITAPS_Swap3D.cc

INTERNAL_OBJS=ITAPS_Delaunay3D.o \
ITAPS_MaxMinSine3D.o \
ITAPS_MinMaxAngle3D.o \
ITAPS_Swap2D.o \
ITAPS_Swap3D.o

all: $(STATIC_LIB) $(SHARED_LIB)

objects: 
	echo Compiling server for $(PACKAGE_NAME)...
	make $(INTERNAL_OBJS)

$(STATIC_LIB): objects
	echo Creating static library for $(PACKAGE_NAME) server...
	ar ruv $@ $(INTERNAL_OBJS)

$(SHARED_LIB): objects
	echo Creating shared library for $(PACKAGE_NAME) server...
	g++ -fPIC -shared -o $@ $(INTERNAL_OBJS)

clean: 
	-rm -f *.o *~ swap3d_test

distclean: clean
	-rm -f Makefile ITAPS_Swap_config.hh $(STATIC_LIB) $(SHARED_LIB)
	-rm -f config.log config.status

INSTALL=/usr/bin/install -c
  
install: /usr/local/lib /usr/local/include
	/usr/bin/install -c $(STATIC_LIB) $(SHARED_LIB) /usr/local/lib
	${INSTALL} -m 644 *.hh /usr/local/include

/usr/local/include/ITAPS:
	mkdir -p /usr/local/include/ITAPS

SWAP_DEPS=./libSwapping.so $(IMESH_FILES) $(IMESH_BACKEND_FILES)
SWAP_LIBS=-L. -lSwapping $(IMESH_LIBS) $(IMESH_BACKEND_LIBS)

# GNU-standard target for 'test'
check: test

test: swap3d_test
	./swap3d_test rand1.vtk

swap3d_test: all swap3d_test.o libSwapping.so
	g++ -static -fPIC  -o swap3d_test swap3d_test.o $(SWAP_LIBS)

depend:
	makedepend -Y. *.cc > /dev/null 2>1

