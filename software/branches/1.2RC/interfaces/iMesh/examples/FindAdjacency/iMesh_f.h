#ifndef IMESH_F_H
#define IMESH_F_H

#ifndef POINTER_SIZE
#if PTRDIFF_MAX == 2147483647
#define POINTER_SIZE 4
#else
#define POINTER_SIZE 8
#endif
#endif

#define iMesh_Instance integer*POINTER_SIZE
#define iMesh_EntityIterator integer*POINTER_SIZE
#define iMesh_EntityArrIterator integer*POINTER_SIZE

#endif 

#include "iBase_f.h"

      integer iMesh_POINT
      integer iMesh_LINE_SEGMENT
      integer iMesh_POLYGON
      integer iMesh_TRIANGLE
      integer iMesh_QUADRILATERAL
      integer iMesh_POLYHEDRON
      integer iMesh_TETRAHEDRON
      integer iMesh_HEXAHEDRON
      integer iMesh_PRISM
      integer iMesh_PYRAMID
      integer iMesh_SEPTAHEDRON
      integer iMesh_ALL_TOPOLOGIES

      parameter (iMesh_POINT = 0)
      parameter (iMesh_LINE_SEGMENT = 1)
      parameter (iMesh_POLYGON = 2)
      parameter (iMesh_TRIANGLE = 3)
      parameter (iMesh_QUADRILATERAL = 4)
      parameter (iMesh_POLYHEDRON = 5)
      parameter (iMesh_TETRAHEDRON = 6)
      parameter (iMesh_HEXAHEDRON = 7)
      parameter (iMesh_PRISM = 8)
      parameter (iMesh_PYRAMID = 9)
      parameter (iMesh_SEPTAHEDRON = 10)
      parameter (iMesh_ALL_TOPOLOGIES = 11)


      integer iMesh_UNAVAILABLE
      integer iMesh_IMMEDIATE
      integer iMesh_LOCAL_TRAVERSAL
      integer iMesh_GLOBAL_TRAVERSAL

      parameter (iMesh_UNAVAILABLE = 0)
      parameter (iMesh_IMMEDIATE = 1)
      parameter (iMesh_LOCAL_TRAVERSAL = 2)
      parameter (iMesh_GLOBAL_TRAVERSAL = 3)


