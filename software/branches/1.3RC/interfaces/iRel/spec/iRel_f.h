#ifndef IREL_F_H
#define IREL_F_H

#ifdef POINTER_SIZE
#if POINTER_SIZE == 8
#define iRel_Instance integer*8
#define iRel_PairHandle integer*8
#elif POINTER_SIZE == 4
#define iRel_Instance integer*4
#define iRel_PairHandle integer*4
#else
#define iRel_Instance integer
#define iRel_PairHandle integer
#endif
#else
#define iRel_Instance integer
#define iRel_PairHandle integer
#endif
#endif

#include "iBase_f.h"

      integer  iRel_IfaceType_MIN
      integer  iRel_IGEOM_IFACE
      integer  iRel_IREL_IFACE
      integer  iRel_IfaceType_MAX

      integer  iRel_RelationType_MIN
      integer  iRel_ENTITY
      integer  iRel_BOTH
      integer  iRel_RelationType_MAX

      integer  iRel_RelationStatus_MIN
      integer  iRel_ACTIVE
      integer  iRel_NOTEXIST
      integer  iRel_RelationStatus_MAX

      parameter(iRel_IfaceType_MIN=0)
      parameter(iRel_IGEOM_IFACE=0)
      parameter(iRel_IREL_IFACE=1)
      parameter(iRel_IfaceType_MAX=1)

      parameter(iRel_RelationType_MIN=0)
      parameter(iRel_ENTITY=0)
      parameter(iRel_BOTH=1)
      parameter(iRel_RelationType_MAX=1)

      parameter(iRel_RelationStatus_MIN=0)
      parameter(iRel_ACTIVE=0)
      parameter(iRel_NOTEXIST=1)
      parameter(iRel_RelationStatus_MAX=1)

