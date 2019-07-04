bin_PROGRAMS = iMeshZoltanTest

iMeshZoltanTest_SOURCES = ITAPSZoltan.cpp ITAPSZoltanTest.cpp ITAPSZoltan.hpp checkTest test_meshes

include $(IMESH_DEFS)

AM_CPPFLAGS = $(IMESH_CPPFLAGS) $(IMESH_INCLUDES) -I$(with_zoltan_include)/ -I$(with_mpi_include)/

iMeshZoltanTest_LDADD = $(IMESH_LIBS) -L$(with_zoltan_lib) -lzoltan $(with_parmetis) -L$(with_mpi_lib) -lsimpi

TESTS=checkTest
