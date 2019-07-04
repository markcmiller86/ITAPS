#ifndef IMESHP_F_H
#define IMESHP_F_H

#ifdef POINTER_SIZE
#if POINTER_SIZE == 8
#define iMeshP_PartitionHandle integer*8
#define iMeshP_RequestHandle integer*8
#elif POINTER_SIZE == 4
#define iMeshP_PartitionHandle integer*4
#define iMeshP_RequestHandle integer*4
#else
#define iMeshP_PartitionHandle integer
#define iMeshP_RequestHandle integer
#endif
#else
#define iMeshP_PartitionHandle integer
#define iMeshP_RequestHandle integer
#endif
#endif

#include "iMesh_f.h"

      integer  iMeshP_EntStatus_MIN
      integer  iMeshP_INTERNAL
      integer  iMeshP_EntStatus_MAX

      parameter(iMeshP_EntStatus_MIN=0)
      parameter(iMeshP_INTERNAL=0)
      parameter(iMeshP_EntStatus_MAX=0)
