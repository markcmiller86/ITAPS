AUTOMAKE_OPTIONS = foreign
DEFS = $(DEFINES)

@IMESH_CONFIG_OPTIONS@

INCLUDES += $(IMESH_INCLUDES) -I$(top_srcdir) -I$(top_builddir)
LIBS += $(IMESH_LIBS)

bin_PROGRAMS = 

if ENABLE_f77
  bin_PROGRAMS += perfF77
  perfF77_SOURCES = perfF77.F perfF_build.c
endif

if ENABLE_f90
  bin_PROGRAMS += perfF90
  perfF90_SOURCES = perfF90.F90 perfF_build.c
endif

if ENABLE_c
  bin_PROGRAMS += perfC
  perfC_SOURCES = perfC.c
endif

if ENABLE_cxx
  bin_PROGRAMS += perfCXX
  perfCXX_SOURCES = perfCXX.cpp
endif


