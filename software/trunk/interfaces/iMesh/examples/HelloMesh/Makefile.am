AUTOMAKE_OPTIONS = foreign
DEFS = $(DEFINES)

@IMESH_CONFIG_OPTIONS@

INCLUDES += $(IMESH_INCLUDES) -I$(top_srcdir) -I$(top_builddir)
LIBS += $(IMESH_LIBS)

bin_PROGRAMS = 

if ENABLE_f77
  bin_PROGRAMS += HelloMeshF77
  HelloMeshF77_SOURCES = HelloMeshF77.F
endif

if ENABLE_f90
  bin_PROGRAMS += HelloMeshF90
  HelloMeshF90_SOURCES = HelloMeshF90.F90
endif

if ENABLE_c
  bin_PROGRAMS += HelloMeshC
  HelloMeshC_SOURCES = HelloMeshC.c
endif

if ENABLE_cxx
  bin_PROGRAMS += HelloMeshCXX
  HelloMeshCXX_SOURCES = HelloMeshCXX.cpp
endif


