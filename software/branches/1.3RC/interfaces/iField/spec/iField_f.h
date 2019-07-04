#ifndef IFIELD_F_H
#define IFIELD_F_H

#ifdef POINTER_SIZE
#if POINTER_SIZE == 8
#define iField_Instance integer*8
#define iField_DomainHandle integer*8
#define iField_TensorHandle integer*8
#define iField_TensorTemplateHandle integer*8
#define iField_QuantHandle integer*8
#define iField_UnitsHandle integer*8
#define iField_Handle integer*8
#elif POINTER_SIZE == 4
#define iField_Instance integer*4
#define iField_DomainHandle integer*4
#define iField_TensorHandle integer*4
#define iField_TensorTemplateHandle integer*4
#define iField_QuantHandle integer*4
#define iField_UnitsHandle integer*4
#define iField_Handle integer*4
#else
#define iField_Instance integer
#define iField_DomainHandle integer
#define iField_TensorHandle integer
#define iField_TensorTemplateHandle integer
#define iField_QuantHandle integer
#define iField_UnitsHandle integer
#define iField_Handle integer
#endif
#else
#define iField_Instance integer
#define iField_DomainHandle integer
#define iField_TensorHandle integer
#define iField_TensorTemplateHandle integer
#define iField_QuantHandle integer
#define iField_UnitsHandle integer
#define iField_Handle integer
#endif
#endif

#include "iBase_f.h"
#include "iMesh_f.h"
#include "iGeom_f.h"
#include "iRel_f.h"

      integer  iField_Precision_MIN
      integer  iField_BOOLEAN
      integer  iField_UCHAR
      integer  iField_INTEGER
      integer  iField_FLOAT
      integer  iField_DOUBLE
      integer  iField_QUAD
      integer  iField_Precision_MAX

      integer  iField_AlgType_MIN
      integer  iField_LOGICAL
      integer  iField_INTEGRAL
      integer  iField_REAL
      integer  iField_COMPLEX
      integer  iField_AlgType_MAX

      integer  iField_CoordType_MIN
      integer  iField_CARTESIAN
      integer  iField_CYLINDRICAL
      integer  iField_SPHERICAL
      integer  iField_CoordType_MAX

      integer  iField_StorageHint_MIN
      integer  iField_BLOCKED
      integer  iField_INTERLEAVED
      integer  iField_MIXED
      integer  iField_PER_ENTITY
      integer  iField_StorageHint_MAX

      parameter(iField_Precision_MIN=0)
      parameter(iField_BOOLEAN=0)
      parameter(iField_UCHAR=1)
      parameter(iField_INTEGER=2)
      parameter(iField_FLOAT=3)
      parameter(iField_DOUBLE=4)
      parameter(iField_QUAD=5)
      parameter(iField_Precision_MAX=5)

      parameter(iField_AlgType_MIN=0)
      parameter(iField_LOGICAL=0)
      parameter(iField_INTEGRAL=1)
      parameter(iField_REAL=2)
      parameter(iField_COMPLEX=3)
      parameter(iField_AlgType_MAX=3)

      parameter(iField_CoordType_MIN=0)
      parameter(iField_CARTESIAN=0)
      parameter(iField_CYLINDRICAL=1)
      parameter(iField_SPHERICAL=2)
      parameter(iField_CoordType_MAX=2)

      parameter(iField_StorageHint_MIN=0)
      parameter(iField_BLOCKED=0)
      parameter(iField_INTERLEAVED=1)
      parameter(iField_MIXED=2)
      parameter(iField_PER_ENTITY=3)
      parameter(iField_StorageHint_MAX=3)

