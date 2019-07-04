include /home/cfog/ITAPS/iMesh-refImpl/iMesh-Defs.inc

IMESH_SPEC_URL=https://redmine.scorec.rpi.edu/anonsvn/itaps/software/trunk/interfaces/iMesh/spec/iMesh.h
IBASE_SPEC_URL=https://redmine.scorec.rpi.edu/anonsvn/itaps/software/trunk/interfaces/iBase/spec/iBase.h
SPEC_URLS=${IBASE_SPEC_URL} ${IMESH_SPEC_URL}

SRC=vTestMeshInterface.cc \
    vTestEntityInterface.cc \
    vTestArrInterface.cc \
    vTestModifyInterface.cc \
    vTestEntSetInterface.cc \
    vTestMeshWithSets.cc \
    vTestEntityWithSets.cc \
    vTestArrayWithSets.cc \
    vTestEntTagInterface.cc \
    vTestArrTagInterface.cc \
    vTestSetRelationInterface.cc \
    vTestBoolOpsInterface.cc \
    vTestSetTagInterface.cc \
    vTestLowerDimensionedCoordinates.cc

OBJ=$(SRC:.cc=.o)

ALL_INCLUDES=$(IMESH_INCLUDES)

LIBS=$(IMESH_LIBS)

DEPS=$(IMESH_FILES)

iMesh_unitTest: iMesh_unitTest.o $(OBJ) $(DEPS) speccheck 
	g++ -fPIC -g -O2 -o iMesh_unitTest iMesh_unitTest.o $(OBJ) -Wl,-Bstatic ${IMESH_LIBS} -Wl,-Bdynamic ${SYS_LIBS}

%.o: %.cc iMesh_unitTest.hh iMesh_unitTest_config.h
	g++ -fPIC  -g -O2 $(ALL_INCLUDES) $(CXXWARNFLAGS) -c $<

test: iMesh_unitTest
	./iMesh_unitTest ../../data/globe_mats.vtk

depend:
	makedepend $(ALL_INCLUDES) -Y *.cc 2> /dev/null

clean: 
	-${RM} *.o

distclean: clean
	-rm -f Makefile *~ config.* iMesh_unitTest iMesh_unitTest_config.h

speccheck:
	@if [ -z "$(ITAPS_NOCKSPEC)" ] ; then \
	    for url in $(SPEC_URLS); do \
	        fname=$$(basename $$url) ;\
		fdir=$$(echo $$fname | cut -d'.' -f1) ;\
	        impl_h="" ;\
	        for idir in $(IMESH_INCLUDES); do \
	            dir=$$(echo $$idir | cut -d'I' -f2-) ;\
	            if [ -e $$dir/$$fname ]; then \
	                impl_h=$$dir/$$fname ;\
	                break ;\
	            fi ;\
	        done ;\
	        if [ -e ../../../$$fdir/spec/$$fname ]; then \
	            diff ../../../$$fdir/spec/$$fname $$impl_h ;\
	        else \
	            svn cat $$url | diff - $$impl_h ;\
	        fi ;\
	        if [ $$? -ne 0 ]; then \
	            echo "Implementation not in compliance with $$fname specification." ;\
                    echo "To override this failure run 'make ITAPS_NOCKSPEC=1'." ;\
	            exit 4 ;\
	        fi \
	    done \
	fi

# DO NOT DELETE

