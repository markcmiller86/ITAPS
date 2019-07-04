AUTOMAKE_OPTIONS = foreign
DEFS = $(DEFINES)

@IMESH_CONFIG_OPTIONS@

INCLUDES += $(IMESH_INCLUDES) -I$(top_srcdir) -I$(top_builddir)
LIBS += $(IMESH_LIBS)

bin_PROGRAMS = 

if ENABLE_f77
  bin_PROGRAMS += FindAdjacencyF77
  FindAdjacencyF77_SOURCES = FindAdjacencyF77.F
endif

if ENABLE_f90
  bin_PROGRAMS += FindAdjacencyF90
  FindAdjacencyF90_SOURCES = FindAdjacencyF90.F90
endif

if ENABLE_f03
  bin_PROGRAMS += FindAdjacencyF03
  FindAdjacencyF90_SOURCES = FindAdjacencyF2003.F03
endif

if ENABLE_c
  bin_PROGRAMS += FindAdjacencyC
  FindAdjacencyC_SOURCES = FindAdjacencyC.c
endif

if ENABLE_cxx
  bin_PROGRAMS += FindAdjacencyCXX
  FindAdjacencyCXX_SOURCES = FindAdjacencyCXX.cpp
endif


