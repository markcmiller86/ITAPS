# Please modify accordingly
include $(ITAPS_DEV)/lib/iMesh-Defs.inc

MPI_INC=-I$(MPI)/include

MPI_LIB=-L$(MPI)/lib -lmpi -lmpi_cxx

ALL_INCLUDES=$(IMESH_INCLUDES) 

LIBS=$(IMESH_LIBS) 

DEPS=$(IMESH_FILES)

.cc.o: g++ -fPIC -DSAVE_NAME=\"/tmp/iMeshP-save-test$(SAVE_EXT)\" $(ALL_INCLUDES) $(CXXWARNFLAGS) -g -c $<

iMeshP_Test: iMeshP_Test.o $(DEPS)
	g++ -fPIC -g -o iMeshP_unitTest iMeshP_unitTest.cc ${LIBS} ${ALL_INCLUDES} ${MPI_INC} ${MPI_LIB}
depend:
	makedepend $(ALL_INCLUDES) -Y *.cc 2> /dev/null

clean: 
	-${RM} *.o

distclean: clean
	-rm -f *~ config.* iMeshP_Test

iMeshP_Test.o: $(MPI)/include/mpi.h
