include /home/cfog/GRUMMP/trunk/iMesh-Defs.inc

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

# Temporarily maintain backward compat. with older iMesh-Defs.inc varnames
# Eventually, we should remove reference to IMESH_INCLUDES
TEST_CPPFLAGS=${IMESH_CPPFLAGS} ${IMESH_INCLUDES}

iMesh_unitTest: iMesh_unitTest.o $(OBJ) $(IMESH_DEPENDENCIES) speccheck 
	g++ -fPIC -g -O2 -o iMesh_unitTest iMesh_unitTest.o $(OBJ) -Wl,-Bstatic ${IMESH_LIBS} -Wl,-Bdynamic ${SYS_LIBS}

%.o: %.cc iMesh_unitTest.hh iMesh_unitTest_config.h
	g++ -fPIC  -g -O2 ${TEST_CPPFLAGS} -c $<

check: test

test: iMesh_unitTest
	./iMesh_unitTest ../../data/globe_mats.vtk

depend:
	makedepend ${TEST_CPPFLAGS} -Y *.cc 2> /dev/null

clean: 
	-${RM} *.o

distclean: clean
	-rm -f Makefile *~ config.* iMesh_unitTest iMesh_unitTest_config.h

INSTALL=/usr/bin/install -c
install: /usr/local/lib /usr/local/include
	/usr/bin/install -c -d /usr/local/bin
	${INSTALL} iMesh_unitTest /usr/local/bin

speccheck:
	@if [ "$(ITAPS_SPECCHECK)" = "none" ] ; then \
	    exit 0 ;\
	fi ;\
	for url in $(SPEC_URLS); do \
	    fname=$$(basename $$url) ;\
	    fdir=$$(echo $$fname | cut -d'.' -f1) ;\
	    impl_h="" ;\
	    for idir in $(TEST_CPPFLAGS); do \
	        dir=$$(echo $$idir | cut -d'I' -f2-) ;\
	        if [ -e $$dir/$$fname ]; then \
	            impl_h=$$dir/$$fname ;\
	            break ;\
	        fi ;\
	    done ;\
	    if [ "$(ITAPS_SPECCHECK)" != "loose" ] ; then \
	        if [ -e ../../../$$fdir/spec/$$fname ]; then \
	            diff -u ../../../$$fdir/spec/$$fname $$impl_h ;\
	        else \
	            svn cat $$url | diff -u - $$impl_h ;\
	        fi ;\
	        if [ $$? -ne 0 ]; then \
	            echo "Implementation not in strict compliance with $$fname specification." ;\
                    echo "To override this failure run 'make ITAPS_SPECCHECK=loose'." ;\
	            exit 4 ;\
	        fi \
	    else \
	        cat $$impl_h | g++ -fPIC -E -g -O2 $(TEST_CPPFLAGS) - | grep -v '^\(#.*\)\?$$' > .tmp-impl.h ;\
	        if [ -e ../../../$$fdir/spec/$$fname ]; then \
		    cat ../../../$$fdir/spec/$$fname | g++ -fPIC -E -g -O2 $(TEST_CPPFLAGS) - | grep -v '^\(#.*\)\?$$' > .tmp-spec.h ;\
	        else \
	            svn cat $$url | g++ -fPIC -E -g -O2 $(TEST_CPPFLAGS) - | grep -v '^\(#.*\)\?$$' > .tmp-spec.h ;\
	        fi ;\
	        diff -u -w .tmp-spec.h .tmp-impl.h ;\
	        if [ $$? -ne 0 ]; then \
	            echo "Implementation not in loose compliance with $$fname specification." ;\
                    echo "To override this failure run 'make ITAPS_SPECCHECK=none'." ;\
	            exit 5 ;\
	        fi ;\
		rm -f .tmp-spec.h .tmp-impl.h ;\
	    fi ;\
	done \

# DO NOT DELETE

