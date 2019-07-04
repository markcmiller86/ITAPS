AUTOMAKE_OPTIONS = foreign
DEFS = $(DEFINES)

@IMESH_CONFIG_OPTIONS@

INCLUDES += $(IMESH_INCLUDES) -I$(top_srcdir) -I$(top_builddir)
LIBS += $(IMESH_LIBS)

bin_PROGRAMS = 

if ENABLE_f77
  bin_PROGRAMS += SimpleIteratorF77
  SimpleIteratorF77_SOURCES = SimpleIteratorF77.F
endif

if ENABLE_f90
  bin_PROGRAMS += SimpleIteratorF90
  SimpleIteratorF90_SOURCES = SimpleIteratorF90.F90
endif

if ENABLE_c
  bin_PROGRAMS += SimpleIteratorC
  SimpleIteratorC_SOURCES = SimpleIteratorC.c
endif

if ENABLE_cxx
  bin_PROGRAMS += SimpleIteratorCXX
  SimpleIteratorCXX_SOURCES = SimpleIteratorCXX.cpp
endif


