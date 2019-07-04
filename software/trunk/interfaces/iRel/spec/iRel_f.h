#ifndef IREL_F_H
#define IREL_F_H

#define IREL_VERSION_MAJOR ITAPS_VERSION_MAJOR
#define IREL_VERSION_MINOR ITAPS_VERSION_MINOR
#define IREL_VERSION_PATCH ITAPS_VERSION_PATCH 
#define IREL_MAJOR_VERSION IREL_VERSION_MAJOR
#define IREL_MINOR_VERSION IREL_VERSION_MINOR
#define IREL_PATCH_VERSION IREL_VERSION_PATCH
#define IREL_VERSION_GE(Maj,Min,Pat) ITAPS_VERSION_GE(Maj,Min,Pat)
#define IREL_VERSION_STRING ITAPS_VERSION_STRING_(iRel)
#define IREL_VERSION_TAG ITAPS_VERSION_TAG_(iRel)
#define IREL_CREATE_NAME__(A,B,C) A##_##B##_##C
#define IREL_CREATE_NAME_(A,B,C) IREL_CREATE_NAME__(A,B,C)
#define IREL_CREATE_NAME(A) IREL_CREATE_NAME_(A,IREL_VERSION_MAJOR,IREL_VERSION_MINOR)

#define iRel_Instance IBASE_HANDLE_T
#define iRel_PairHandle IBASE_HANDLE_T

#endif

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

