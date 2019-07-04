AUTOMAKE_OPTIONS = foreign
DEFS = $(DEFINES)

@IMESH_CONFIG_OPTIONS@

INCLUDES += $(IMESH_INCLUDES) -I$(top_srcdir) -I$(top_builddir)
LIBS += $(IMESH_LIBS)

bin_PROGRAMS = 

if ENABLE_f77
  bin_PROGRAMS += ListSetsNTagsF77
  ListSetsNTagsF77_SOURCES = ListSetsNTagsF77.F
endif

if ENABLE_f90
  bin_PROGRAMS += ListSetsNTagsF90
  ListSetsNTagsF90_SOURCES = ListSetsNTagsF90.F90
endif

if ENABLE_c
  bin_PROGRAMS += ListSetsNTagsC
  ListSetsNTagsC_SOURCES = ListSetsNTagsC.c
endif

if ENABLE_cxx
  bin_PROGRAMS += ListSetsNTagsCXX
  ListSetsNTagsCXX_SOURCES = ListSetsNTagsCXX.cpp
endif


