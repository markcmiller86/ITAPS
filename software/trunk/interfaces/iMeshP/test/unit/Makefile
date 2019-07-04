# Please modify accordingly
include $(IMESHP_DEFS)/lib/iMeshP-Defs.inc

#MPI_INC=-I$(MPI)/include

#MPI_LIB=-L$(MPI)/lib -lmpi -lmpi_cxx

ALL_INCLUDES=$(IMESH_INCLUDES) 

LIBS=$(IMESHP_LIBS)

DEPS=$(IMESH_FILES)

.cc.o:
	mpiCC -fPIC -DSAVE_NAME=\"/tmp/iMeshP-save-test$(SAVE_EXT)\" $(ALL_INCLUDES) $(CXXWARNFLAGS) -g -c $<

iMeshP_unitTest: iMeshP_unitTest.o $(DEPS)
	mpiCC -fPIC -g -o iMeshP_unitTest iMeshP_unitTest.cc ${ALL_INCLUDES} ${MPI_INC} ${LIBS} ${MPI_LIB}

MOAB_iMeshP_unitTest: MOAB_iMeshP_unitTest.o $(DEPS)
	mpiCC -fPIC -DSAVE_NAME=\"/tmp/iMeshP-save-test$(SAVE_EXT)\" -g -o \
	    MOAB_iMeshP_unitTest MOAB_iMeshP_unitTest.cc ${ALL_INCLUDES} ${MPI_INC} ${LIBS}

depend:
	makedepend $(ALL_INCLUDES) -Y *.cc 2> /dev/null

clean: 
	-rm -f iMeshP_unitTest iMeshP_unitTest.o MOAB_iMeshP_unitTest MOAB_iMeshP_unitTest.o

distclean: clean
	-rm -f *~ config.*
