include /home/cfog/ITAPS/repository/Implementations/GRUMMP/iMesh-Defs.inc
include /home/cfog/ITAPS/repository/Implementations/GRUMMP/iBase-Defs.inc

IMPL_FLAGS=-DCREATE_ARG=\"$(IMESH_CREATE_ARG_2D)\"

ALL_INCLUDES=$(IMESH_INCLUDES) $(IBASE_INCLUDES)

LIBS=$(IMESH_LIB_DIR) $(IBASE_LIB_DIR) $(IMESH_LIBS) $(IMESH_BACKEND_LIBS)

DEPS=$(IMESH_CLIENT_CXX_LIBFILE) $(IMESH_SERVER_LIBFILE) $(IBASE_CLIENT_CXX_LIBFILE) $(IBASE_SERVER_LIBFILE) $(IMESH_BACKEND_LIBFILE)

all: modularTest iMeshTest.o TetTestData.o

.cc.o: iMesh_unitTest.hh iMesh_unitTest-config.h 
	g++ $(IMPL_FLAGS) -DSAVE_NAME=\"/tmp/iMesh-save-test$(SAVE_EXT)\" $(ALL_INCLUDES) -g -c $<

modularTest: modularTest.o iMeshTest.o TetTestData.o $(DEPS)
	g++ -static -g -o modularTest iMeshTest.o TetTestData.o modularTest.o ${LIBS}

iMesh_unitTest: iMesh_unitTest.o iMesh_iterTest.o $(DEPS)
	g++ -static -g -o iMesh_unitTest iMesh_unitTest.o iMesh_iterTest.o ${LIBS}


depend:
	makedepend $(INCLUDES) *.cc 2> /dev/null

clean: 
	${RM} *.o

distclean: clean
	-rm -f Makefile *~ config.* iMesh_unitTest iMesh_unitTest-config.h

# DO NOT DELETE
