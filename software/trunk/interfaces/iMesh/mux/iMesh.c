#include "iplex-iBase.h"
#include "iMesh.h"
#include <stdlib.h>

/* character used to separate options passed into iMesh_newMesh */
#define OPTIONS_SEPARATOR ':'

typedef void* EntHandle;
typedef void* TagHandle;
typedef void* IterHandle;

struct iplex_iMesh {

  struct iplex_iBase base;

    /* Destructor */
  lt_ptr ctor;
  lt_ptr dtor;
    /* Mesh */
  lt_ptr load;
  lt_ptr save;
  lt_ptr getRootSet;
  lt_ptr getGeometricDimension;
  lt_ptr getDfltStorage;
  lt_ptr getAdjTable;
  lt_ptr areEHValid;
  lt_ptr getNumOfType;
  lt_ptr getNumOfTopo;
  lt_ptr getAllVtxCoords;
  lt_ptr getVtxCoordIndex;
  lt_ptr getEntities;
  lt_ptr getVtxArrCoords;
  lt_ptr getAdjEntities;
    /* Entity */
  lt_ptr initEntIter;
  lt_ptr getNextEntIter;
  lt_ptr resetEntIter;
  lt_ptr endEntIter;
  lt_ptr getEntTopo;
  lt_ptr getEntType;
  lt_ptr getVtxCoord;
  lt_ptr getEntAdj;
    /* Arr */
  lt_ptr initEntArrIter;
  lt_ptr getNextEntArrIter;
  lt_ptr resetEntArrIter;
  lt_ptr endEntArrIter;
  lt_ptr getEntArrTopo;
  lt_ptr getEntArrType;
  lt_ptr getEntArrAdj;
    /* Modify */
  lt_ptr setVtxCoords;
  lt_ptr createVtx;
  lt_ptr createEnt;
  lt_ptr deleteEnt;
    /* ArrMod */
  lt_ptr setVtxArrCoords;
  lt_ptr createVtxArr;
  lt_ptr createEntArr;
  lt_ptr deleteEntArr;
    /* Factory */
  lt_ptr newMesh;
  
    /* Instance Data */
  void* real_instance;
  lt_dlhandle lib_handle;
  
};

static int initialize_iMesh( lt_dlhandle, struct iplex_iMesh* );

#define FUNCTION1(NAME,TYPE1) \
  int iMesh_ ## NAME ( iMesh_Instance instance, TYPE1 ARG1 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1 ))(i->NAME)) \
      (i->real_instance, ARG1); \
  }

#define FUNCTION2(NAME,TYPE1,TYPE2) \
  int iMesh_ ## NAME ( iMesh_Instance instance, TYPE1 ARG1, TYPE2 ARG2 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2); \
  }

#define FUNCTION3(NAME,TYPE1,TYPE2,TYPE3) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3); \
  }

#define FUNCTION4(NAME,TYPE1,TYPE2,TYPE3,TYPE4) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4); \
  }

#define FUNCTION5(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5); \
  }

#define FUNCTION6(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5,TYPE6) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5, \
                       TYPE6 ARG6 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6); \
  }

#define FUNCTION7(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5,TYPE6,TYPE7) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5, \
                       TYPE6 ARG6, \
                       TYPE7 ARG7 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7); \
  }

#define FUNCTION8(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5,TYPE6,TYPE7,TYPE8) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5, \
                       TYPE6 ARG6, \
                       TYPE7 ARG7, \
                       TYPE8 ARG8 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8); \
  }

#define FUNCTION9(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5,TYPE6,TYPE7,TYPE8,TYPE9) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5, \
                       TYPE6 ARG6, \
                       TYPE7 ARG7, \
                       TYPE8 ARG8, \
                       TYPE9 ARG9 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9); \
  }

#define FUNCTION10(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5,TYPE6,TYPE7,TYPE8,TYPE9,TYPE10) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5, \
                       TYPE6 ARG6, \
                       TYPE7 ARG7, \
                       TYPE8 ARG8, \
                       TYPE9 ARG9, \
                       TYPE10 ARG10 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10); \
  }

#define FUNCTION11(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5,TYPE6,TYPE7,TYPE8,TYPE9,TYPE10,TYPE11) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5, \
                       TYPE6 ARG6, \
                       TYPE7 ARG7, \
                       TYPE8 ARG8, \
                       TYPE9 ARG9, \
                       TYPE10 ARG10, \
                       TYPE11 ARG11 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10, TYPE11 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11); \
  }

#define FUNCTION12(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5,TYPE6,TYPE7,TYPE8,TYPE9,TYPE10,TYPE11,TYPE12) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5, \
                       TYPE6 ARG6, \
                       TYPE7 ARG7, \
                       TYPE8 ARG8, \
                       TYPE9 ARG9, \
                       TYPE10 ARG10, \
                       TYPE11 ARG11, \
                       TYPE12 ARG12 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10, TYPE11, TYPE12 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12); \
  }

#define FUNCTION13(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5,TYPE6,TYPE7,TYPE8,TYPE9,TYPE10,TYPE11,TYPE12,TYPE13) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5, \
                       TYPE6 ARG6, \
                       TYPE7 ARG7, \
                       TYPE8 ARG8, \
                       TYPE9 ARG9, \
                       TYPE10 ARG10, \
                       TYPE11 ARG11, \
                       TYPE12 ARG12, \
                       TYPE13 ARG13 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, TYPE7, TYPE8, TYPE9, TYPE10, TYPE11, TYPE12, TYPE13 ))(i->NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11, ARG12, ARG13); \
  }


#define BASE1(NAME,TYPE1) \
  int iMesh_ ## NAME ( iMesh_Instance instance, TYPE1 ARG1 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->base.NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1 ))(i->base.NAME)) \
      (i->real_instance, ARG1); \
  }

#define BASE2(NAME,TYPE1,TYPE2) \
  int iMesh_ ## NAME ( iMesh_Instance instance, TYPE1 ARG1, TYPE2 ARG2 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->base.NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2 ))(i->base.NAME)) \
      (i->real_instance, ARG1, ARG2); \
  }

#define BASE3(NAME,TYPE1,TYPE2,TYPE3) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->base.NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3 ))(i->base.NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3); \
  }

#define BASE4(NAME,TYPE1,TYPE2,TYPE3,TYPE4) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->base.NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4 ))(i->base.NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4); \
  }

#define BASE5(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->base.NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5 ))(i->base.NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5); \
  }

#define BASE6(NAME,TYPE1,TYPE2,TYPE3,TYPE4,TYPE5,TYPE6) \
  int iMesh_ ## NAME ( iMesh_Instance instance, \
                       TYPE1 ARG1, \
                       TYPE2 ARG2, \
                       TYPE3 ARG3, \
                       TYPE4 ARG4, \
                       TYPE5 ARG5, \
                       TYPE6 ARG6 ) \
  { \
    const struct iplex_iMesh* i = (const struct iplex_iMesh*)instance; \
    if (!i->base.NAME) \
      return iBase_NOT_SUPPORTED; \
    return ((int(*)( iMesh_Instance, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6 ))(i->base.NAME)) \
      (i->real_instance, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6); \
  }

  /* Error */
BASE1(getErrorType,int*)
BASE2(getDescription,char*,int)
  /* Tag */
BASE5(createTag, const char*, const int, const int, const int, TagHandle* )
BASE2(destroyTag, TagHandle, const int )
BASE3(getTagName, const TagHandle, char*, int )
BASE2(getTagSizeValues, const TagHandle, int* )
BASE2(getTagSizeBytes, const TagHandle, int* )
BASE3(getTagHandle, const char*, int, TagHandle* )
BASE2(getTagType, const TagHandle, int* )
  /* EntTag */
BASE5(getData, const EntHandle, const TagHandle, char**, int*, int* )
BASE3(getIntData, const EntHandle, const TagHandle, int* )
BASE3(getDblData, const EntHandle, const TagHandle, double* )
BASE3(getEHData, const EntHandle, const TagHandle, EntHandle* )
BASE4(setData, EntHandle, const TagHandle, const char*, int )
BASE3(setIntData, EntHandle, const TagHandle, const int )
BASE3(setDblData, EntHandle, const TagHandle, const double )
BASE3(setEHData, EntHandle, const TagHandle, const EntHandle )
BASE4(getAllTags, const EntHandle, TagHandle**, int*, int* )
BASE2(rmvTag, EntHandle, const TagHandle )
  /* ArrTag */
BASE6(getArrData, const EntHandle*, const int, const TagHandle, char**, int*, int* )
BASE6(getIntArrData, const EntHandle*, const int, const TagHandle, int**, int*, int*)
BASE6(getDblArrData, const EntHandle*, const int, const TagHandle, double**, int*, int*)
BASE6(getEHArrData, const EntHandle*, const int, const TagHandle, EntHandle**, int*, int*)
BASE5(setArrData, EntHandle*, const int, const TagHandle, const char*, const int )
BASE5(setIntArrData, EntHandle*, const int, const TagHandle, const int*, const int)
BASE5(setDblArrData, EntHandle*, const int, const TagHandle, const double*, const int)
BASE5(setEHArrData, EntHandle*, const int, const TagHandle, const EntHandle*, const int)
BASE3(rmvArrTag, EntHandle*, const int, const TagHandle )
  /* SetTag */
BASE4(setEntSetData, EntHandle, const TagHandle, const char*, int )
BASE3(setEntSetIntData, EntHandle, const TagHandle, const int )
BASE3(setEntSetDblData, EntHandle, const TagHandle, const double )
BASE3(setEntSetEHData, EntHandle, const TagHandle, const EntHandle )
BASE5(getEntSetData, const EntHandle, const TagHandle, char**, int*, int* )
BASE3(getEntSetIntData, const EntHandle, const TagHandle, int* )
BASE3(getEntSetDblData, const EntHandle, const TagHandle, double* )
BASE3(getEntSetEHData, const EntHandle, const TagHandle, EntHandle* )
BASE4(getAllEntSetTags, const EntHandle, TagHandle**, int*, int* )
BASE2(rmvEntSetTag, EntHandle, const TagHandle )
  /* EntSet */
BASE2(createEntSet, const int, EntHandle*)
BASE1(destroyEntSet, EntHandle)
BASE2(isList, const EntHandle, int*)
BASE3(getNumEntSets, const EntHandle, const int, int* )
BASE5(getEntSets, const EntHandle, const int, EntHandle**, int*, int* )
BASE2(addEntToSet, const EntHandle, EntHandle* )
BASE2(rmvEntFromSet, const EntHandle, EntHandle* )
BASE3(addEntArrToSet, const EntHandle*, const int, EntHandle* )
BASE3(rmvEntArrFromSet, const EntHandle*, const int, EntHandle* )
BASE2(addEntSet, const EntHandle, EntHandle* )
BASE2(rmvEntSet, const EntHandle, EntHandle* )
BASE3(isEntContained, const EntHandle, const EntHandle, int*)
BASE3(isEntSetContained, const EntHandle, const EntHandle, int*)
  /* SetRelation */
BASE2(addPrntChld, EntHandle*, EntHandle* )
BASE2(rmvPrntChld, EntHandle*, EntHandle* )
BASE3(isChildOf, const EntHandle, const EntHandle, int* )
BASE3(getNumChld, const EntHandle, const int, int* )
BASE3(getNumPrnt, const EntHandle, const int, int* )
BASE5(getChldn, const EntHandle, const int, EntHandle**, int*, int* )
BASE5(getPrnts, const EntHandle, const int, EntHandle**, int*, int* )
  /* SetBoolOps */
BASE3(subtract, const EntHandle, const EntHandle, EntHandle* )
BASE3(intersect, const EntHandle, const EntHandle, EntHandle* )
BASE3(unite, const EntHandle, const EntHandle, EntHandle* )
  /* Mesh */
FUNCTION5(load, const EntHandle, const char*, int, const char*, int)
FUNCTION5(save, const EntHandle, const char*, int, const char*, int)
FUNCTION1(getRootSet, EntHandle*)
FUNCTION1(getGeometricDimension, int*)
FUNCTION1(getDfltStorage, int*)
FUNCTION3(getAdjTable, int**, int*, int*)
FUNCTION1(areEHValid, int*)
FUNCTION3(getNumOfType, const EntHandle, const int, int*)
FUNCTION3(getNumOfTopo, const EntHandle, const int, int*)
FUNCTION8(getAllVtxCoords, const EntHandle, double**, int*, int*, int**, int*, int*, int*)
FUNCTION13(getVtxCoordIndex, const EntHandle, const int, const int, const int, int**, int*, int*, int**, int*, int*, int**, int*, int*)
FUNCTION6(getEntities, const EntHandle, const int, const int, EntHandle**, int*, int* )
FUNCTION6(getVtxArrCoords, const EntHandle*, const int, int*, double**, int*, int*)
FUNCTION13(getAdjEntities, const EntHandle, const int, const int, const int, EntHandle**, int*, int*, int**, int*, int*, int**, int*, int*)
  /* Entity */
FUNCTION5(initEntIter, const EntHandle, const int, const int, IterHandle*, int* )
FUNCTION3(getNextEntIter, IterHandle, EntHandle*, int* )
FUNCTION1(resetEntIter, IterHandle )
FUNCTION1(endEntIter, IterHandle )
FUNCTION2(getEntTopo, const EntHandle, int*)
FUNCTION2(getEntType, const EntHandle, int*)
FUNCTION4(getVtxCoord, const EntHandle, double*, double*, double* )
FUNCTION5(getEntAdj, const EntHandle, const int, EntHandle**, int*, int* )
  /* Arr */  
FUNCTION6(initEntArrIter, const EntHandle, const int, const int, const int, IterHandle*, int* )
FUNCTION5(getNextEntArrIter, IterHandle, EntHandle**, int*, int*, int* )
FUNCTION1(resetEntArrIter, IterHandle )
FUNCTION1(endEntArrIter, IterHandle )
FUNCTION5(getEntArrTopo, const EntHandle*, const int, int**, int*, int* )
FUNCTION5(getEntArrType, const EntHandle*, const int, int**, int*, int* )
FUNCTION9(getEntArrAdj, const EntHandle*, const int, const int, EntHandle**, int*, int*, int**, int*, int* )
  /* Modify */
FUNCTION4(setVtxCoords, EntHandle, const double, const double, const double )
FUNCTION4(createVtx, const double, const double, const double, EntHandle* )
FUNCTION5(createEnt, const int, const EntHandle*, const int, EntHandle*, int* )
FUNCTION1(deleteEnt, EntHandle )
  /* ArrMod */
FUNCTION5(setVtxArrCoords, EntHandle*, const int, const int, const double*, const int )
FUNCTION7(createVtxArr, const int, const int, const double*, const int, EntHandle**, int*, int* )
FUNCTION9(createEntArr, const int, const EntHandle*, const int, EntHandle**, int*, int*, int**, int*, int* )
FUNCTION2(deleteEntArr, EntHandle*, const int )

int iMesh_ctor( iMesh_Instance* i ) 
  { *i = NULL; return iBase_NOT_SUPPORTED; }

typedef int (*newMesh_t)(const char*, const int, iMesh_Instance*);
typedef int (*ctor_t)(iMesh_Instance*);
typedef int (*dtor_t)(iMesh_Instance);

int iMesh_newMesh( const char* options, 
                   const int options_len, 
                   iMesh_Instance* instance )
{
  lt_dlhandle lib_handle;
  struct iplex_iMesh* ptr;
  int err, i;
  char libpath[1024];
  
    /* first option must be library to load */
  if (options_len < 3 || !options)
    return iBase_INVALID_ARGUMENT;
    
  for (i = 0; i < sizeof(libpath)-1 && i < options_len && options[i] != OPTIONS_SEPARATOR; ++i)
    libpath[i] = options[i];
  libpath[i] = '\0';
  if (i < options_len && options[i] == OPTIONS_SEPARATOR)
    ++i;
  
    /* load the library */
  lib_handle = lt_dlopenext( libpath );
  if (lib_handle == NULL)
    return iBase_FILE_NOT_FOUND;
  
    /* allocate a struct to hold the instance */
  ptr = (struct iplex_iMesh*)malloc(sizeof(struct iplex_iMesh));
  if (!ptr) {
    ld_dlclose( lib_handle );
    return iBase_MEMORY_ALLOCATION_FAILED;
  }
  
    /* populate instance struct from library */
  if (!initialize_iMesh( lib_handle, ptr )) {
    ld_dlclose( lib_handle );
    free(ptr);
    return iBase_FAILURE;
  }
  
    /* call factory method on underlying implementation */
  ptr->lib_handle = lib_handle;
  if (ptr->newMesh) 
    err = ((newMesh_t)(ptr->newMesh))( options + i, options_len - i, &(ptr->real_instance) );
  else
    err = ((ctor_t)(ptr->ctor))( &(ptr->real_instance) );
  if (iBase_SUCCESS != err) {
    ld_dlclose( lib_handle );
    free(ptr);
    return err;
  }
 
  *instance = ptr;
  return iBase_SUCCESS;
}

int iMesh_dtor( iMesh_Instance instance )
{
  int err;
  struct iplex_iMesh* ptr = (struct iplex_iMesh*)instance;
    /* Call destructor for underlying instance */
  if (ptr->dtor) {
    err = ((dtor_t)(ptr->dtor))( &(ptr->real_instance) );
    if (iBase_SUCCESS != err)
      return err;
  }
    /* Release resources allocated for instance */
  lt_dlclose( ptr->lib_handle );
  free( ptr );
  return iBase_SUCCESS;
}


#define LOAD_SYMBOL( A ) \
  instance->A = lt_dlsym( lib, "iMesh_" #A );

static int initialize_iMesh( lt_dlhandle lib, struct iplex_iMesh* instance )
{
  instance->lib_handle = lib;
  
  int bval = initialize_iBase( lib, "iMesh_", &instance->base );
  if (!bval)
    return 0;
 
    /* Destructor */
  LOAD_SYMBOL( ctor )
  LOAD_SYMBOL( dtor )
     /* Mesh */
  LOAD_SYMBOL( load )
  LOAD_SYMBOL( save )
  LOAD_SYMBOL( getRootSet )
  LOAD_SYMBOL( getGeometricDimension )
  LOAD_SYMBOL( getDfltStorage )
  LOAD_SYMBOL( getAdjTable )
  LOAD_SYMBOL( areEHValid )
  LOAD_SYMBOL( getNumOfType )
  LOAD_SYMBOL( getNumOfTopo )
  LOAD_SYMBOL( getAllVtxCoords )
  LOAD_SYMBOL( getVtxCoordIndex )
  LOAD_SYMBOL( getEntities )
  LOAD_SYMBOL( getVtxArrCoords )
  LOAD_SYMBOL( getAdjEntities )
    /* Entity */
  LOAD_SYMBOL( initEntIter )
  LOAD_SYMBOL( getNextEntIter )
  LOAD_SYMBOL( resetEntIter )
  LOAD_SYMBOL( endEntIter )
  LOAD_SYMBOL( getEntTopo )
  LOAD_SYMBOL( getEntType )
  LOAD_SYMBOL( getVtxCoord )
  LOAD_SYMBOL( getEntAdj )
    /* Arr */
  LOAD_SYMBOL( initEntArrIter )
  LOAD_SYMBOL( getNextEntArrIter )
  LOAD_SYMBOL( resetEntArrIter )
  LOAD_SYMBOL( endEntArrIter )
  LOAD_SYMBOL( getEntArrTopo )
  LOAD_SYMBOL( getEntArrType )
  LOAD_SYMBOL( getEntArrAdj )
    /* Modify */
  LOAD_SYMBOL( setVtxCoords )
  LOAD_SYMBOL( createVtx )
  LOAD_SYMBOL( createEnt )
  LOAD_SYMBOL( deleteEnt )
    /* ArrMod */
  LOAD_SYMBOL( setVtxArrCoords )
  LOAD_SYMBOL( createVtxArr )
  LOAD_SYMBOL( createEntArr )
  LOAD_SYMBOL( deleteEntArr )
    /* Factory */
  LOAD_SYMBOL( newMesh )

    /* The absolute minimum interface must provide the factory method */
  return instance->ctor || instance->newMesh;
}
