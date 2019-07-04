#ifndef IMESH_F_H
#define IMESH_F_H

#ifdef POINTER_SIZE
#if POINTER_SIZE == 8
#define iMesh_Instance integer*8
#elif POINTER_SIZE == 4
#define iMesh_Instance integer*4
#else
#define iMesh_Instance integer
#endif
#else
#define iMesh_Instance integer
#endif
#endif

#include "iBase_f.h"

      integer  iMesh_EntityTopology_MIN
      integer  iMesh_POINT
      integer  iMesh_LINE_SEGMENT
      integer  iMesh_POLYGON
      integer  iMesh_TRIANGLE
      integer  iMesh_QUADRILATERAL
      integer  iMesh_POLYHEDRON
      integer  iMesh_TETRAHEDRON
      integer  iMesh_HEXAHEDRON
      integer  iMesh_PRISM
      integer  iMesh_PYRAMID
      integer  iMesh_SEPTAHEDRON
      integer  iMesh_ALL_TOPOLOGIES
      integer  iMesh_EntityTopology_MAX

      parameter(iMesh_EntityTopology_MIN=0)
      parameter(iMesh_POINT=0)
      parameter(iMesh_LINE_SEGMENT=1)
      parameter(iMesh_POLYGON=2)
      parameter(iMesh_TRIANGLE=3)
      parameter(iMesh_QUADRILATERAL=4)
      parameter(iMesh_POLYHEDRON=5)
      parameter(iMesh_TETRAHEDRON=6)
      parameter(iMesh_HEXAHEDRON=7)
      parameter(iMesh_PRISM=8)
      parameter(iMesh_PYRAMID=9)
      parameter(iMesh_SEPTAHEDRON=10)
      parameter(iMesh_ALL_TOPOLOGIES=11)
      parameter(iMesh_EntityTopology_MAX=11)

