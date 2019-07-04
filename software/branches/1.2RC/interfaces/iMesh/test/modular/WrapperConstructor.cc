#include <dlfcn.h>
#include "iMeshWrapper.hh"

#define QUOTE(a) #a
#define QUOTED_NAME(a) QUOTE(a)

iMeshWrapper::iMeshWrapper(std::string filenames[],
			   const int numFiles,
			   const bool isRefImpl) :
  m_isRefImpl(isRefImpl), m_numFiles(numFiles), m_libHandles(NULL),
  m_instance(NULL), m_rootSet(NULL)
{
  m_libHandles = new void*[numFiles];
  char* errMessage = dlerror();
  for (int f = numFiles - 1; f >= 1; f--) {
    m_libHandles[f] = dlopen(filenames[f].c_str(), RTLD_LAZY + RTLD_GLOBAL);
    errMessage = dlerror();
    if (errMessage) 
      std::cout << errMessage << std::endl;
  }

  void* dlib = dlopen(filenames[0].c_str(), RTLD_LAZY);
  m_libHandles[0] = dlib;
  errMessage = dlerror();
  if (errMessage) 
    std::cout << errMessage << std::endl;
    
  getErrorType = reinterpret_cast
    <void (*)(iMesh_Instance, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getErrorType)));
  
  getDescription = reinterpret_cast
    <void (*)(iMesh_Instance, char*, int*, int)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getDescription)));
  
  newMesh = reinterpret_cast
    <void (*)(const char*, iMesh_Instance*, int*, int)>
    (dlsym(dlib, QUOTED_NAME(iMesh_newMesh)));
  
  dtor = reinterpret_cast
    <void (*)(iMesh_Instance, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_dtor)));

  load = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const char*, 
	      const char*, int*, int, int)>
    (dlsym(dlib, QUOTED_NAME(iMesh_load)));
 
  save = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const char*, 
	      const char*, int*, const int, int)>
   (dlsym(dlib, QUOTED_NAME(iMesh_save)));

  getRootSet = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getRootSet)));

  getGeometricDimension = reinterpret_cast
    <void (*)(iMesh_Instance, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getGeometricDimension)));

  setGeometricDimension = reinterpret_cast
    <void (*)(iMesh_Instance, int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setGeometricDimension)));
  
  getDfltStorage = reinterpret_cast
    <void (*)(iMesh_Instance, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getDfltStorage)));
  
  getAdjTable  = reinterpret_cast
    <void (*)(iMesh_Instance, int**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getAdjTable)));
  
  getNumOfType = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int,
	      int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getNumOfType)));
  
  getNumOfTopo = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int,
	      int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getNumOfTopo)));
  
  areEHValid = reinterpret_cast
    <void (*)(iMesh_Instance, int, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_areEHValid)));
  
  getEntities = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int,
	      const int, iBase_EntityHandle**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntities)));
  
  getVtxArrCoords = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int, const int,
	      double**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getVtxArrCoords)));
  
  initEntArrIter = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int,
	      const int, const int, iMesh_EntityArrIterator*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_initEntArrIter)));
  
  getNextEntArrIter = reinterpret_cast
    <void (*)(iMesh_Instance, iMesh_EntityArrIterator, iBase_EntityHandle**,
	      int*, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getNextEntArrIter)));
  
  resetEntArrIter = reinterpret_cast
    <void (*)(iMesh_Instance, iMesh_EntityArrIterator, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_resetEntArrIter)));
  
  endEntArrIter = reinterpret_cast
    <void (*)(iMesh_Instance, iMesh_EntityArrIterator, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_endEntArrIter)));
  
  getEntArrTopo = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int, int**,
	      int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntArrTopo)));
  
  getEntArrType = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int, int**,
	      int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntArrType)));
  
  getEntArrAdj = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int, const int,
	      iBase_EntityHandle**, int*, int*, int**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntArrAdj)));
  
  getEntArr2ndAdj = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle const*, int, int, int,
	      iBase_EntityHandle**, int*, int*, int**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntArr2ndAdj)));
  
  getAdjEntIndices = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, int, int, int,
	      iBase_EntityHandle**, int*, int*, iBase_EntityHandle**,
	      int*, int*, int**, int*, int*, int**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getAdjEntIndices)));
  
  createEntSet = reinterpret_cast
    <void (*)(iMesh_Instance, const int, iBase_EntitySetHandle*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_createEntSet)));
  
  destroyEntSet = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_destroyEntSet)));
  
  isList = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_isList)));
  
  getNumEntSets = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int, int*,
	      int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getNumEntSets)));
  
  getEntSets = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int,
	      iBase_EntitySetHandle**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntSets)));
  
  addEntToSet = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle, iBase_EntitySetHandle,
	      int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_addEntToSet)));
  
  rmvEntFromSet = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle, iBase_EntitySetHandle,
	      int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_rmvEntFromSet)));
  
  addEntArrToSet = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, int,
	      iBase_EntitySetHandle, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_addEntArrToSet)));
  
  rmvEntArrFromSet = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, int,
	      iBase_EntitySetHandle, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_rmvEntArrFromSet)));
  
  addEntSet = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, iBase_EntitySetHandle,
	      int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_addEntSet)));
  
  rmvEntSet = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, iBase_EntitySetHandle,
	      int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_rmvEntSet)));
  
  isEntContained = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, iBase_EntityHandle,
	      int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_isEntContained)));
  
  isEntArrContained = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, const iBase_EntityHandle*,
	      int, int**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_isEntArrContained)));
  
  isEntSetContained = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle,
	      const iBase_EntitySetHandle, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_isEntSetContained)));
  
  addPrntChld = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, iBase_EntitySetHandle,
	      int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_addPrntChld)));
  
  rmvPrntChld = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, iBase_EntitySetHandle,
	      int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_rmvPrntChld)));
  
  isChildOf = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle,
	      const iBase_EntitySetHandle, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_isChildOf)));
  
  getNumChld = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int,
	      int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getNumChld)));
  
  getNumPrnt = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int,
	      int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getNumPrnt)));
  
  getChldn = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int,
	      iBase_EntitySetHandle**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getChldn)));
  
  getPrnts = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int,
	      iBase_EntitySetHandle**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getPrnts)));
  
  setVtxArrCoords = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int,
	      const int, const double*, const int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setVtxArrCoords)));
  
  createVtxArr = reinterpret_cast
    <void (*)(iMesh_Instance, const int, const int, const double*, const int,
	      iBase_EntityHandle**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_createVtxArr)));
  
  createEntArr = reinterpret_cast
    <void (*)(iMesh_Instance, const int, const iBase_EntityHandle*,
	      const int, iBase_EntityHandle**, int*, int*, int**,
	      int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_createEntArr)));
  
  deleteEntArr = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_deleteEntArr)));
  
  createTag = reinterpret_cast
    <void (*)(iMesh_Instance, const char*, const int, const int,
	      iBase_TagHandle*, int*, const int)>
    (dlsym(dlib, QUOTED_NAME(iMesh_createTag)));
  
  destroyTag = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_TagHandle, const int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_destroyTag)));
  
  getTagName = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_TagHandle, char*, int*, int)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getTagName)));
  
  getTagSizeValues = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_TagHandle, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getTagSizeValues)));
  
  getTagSizeBytes = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_TagHandle, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getTagSizeBytes)));
  
  getTagHandle = reinterpret_cast
    <void (*)(iMesh_Instance, const char*, iBase_TagHandle*, int*, int)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getTagHandle)));
  
  getTagType = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_TagHandle, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getTagType)));
  
  setEntSetData = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, const iBase_TagHandle,
	      const char*, const int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setEntSetData)));
  
  setEntSetIntData = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, const iBase_TagHandle,
	      const int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setEntSetIntData)));
  
  setEntSetDblData = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, const iBase_TagHandle,
	      const double, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setEntSetDblData)));
  
  setEntSetEHData = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle, const iBase_TagHandle,
	      const iBase_EntityHandle, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setEntSetEHData)));
  
  getEntSetData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle,
	      const iBase_TagHandle, char**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntSetData)));
  
  getEntSetIntData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle,
	      const iBase_TagHandle, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntSetIntData)));
  
  getEntSetDblData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle,
	      const iBase_TagHandle, double*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntSetDblData)));
  
  getEntSetEHData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle,
	      const iBase_TagHandle, iBase_EntityHandle*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntSetEHData)));
  
  getAllEntSetTags = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle,
	      iBase_TagHandle**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getAllEntSetTags)));
  
  rmvEntSetTag = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntitySetHandle,
	      const iBase_TagHandle, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_rmvEntSetTag)));
  
  setVtxCoord = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle,
	      const double,  const double, const double, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setVtxCoord)));
  
  createVtx = reinterpret_cast
    <void (*)(iMesh_Instance, const double,  const double, const double,
	      iBase_EntityHandle*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_createVtx)));
  
  createEnt = reinterpret_cast
    <void (*)(iMesh_Instance, const int, const iBase_EntityHandle*,
	      const int, iBase_EntityHandle*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_createEnt)));
  
  deleteEnt = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_deleteEnt)));
  
  getArrData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int,
	      const iBase_TagHandle, char**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getArrData)));
  
  getIntArrData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int,
	      const iBase_TagHandle, int**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getIntArrData)));
  
  getDblArrData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int,
	      const iBase_TagHandle, double**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getDblArrData)));
  
  getEHArrData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int,
	      const iBase_TagHandle, iBase_EntityHandle**, int*,
	      int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEHArrData)));
  
  setArrData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int,
	      const iBase_TagHandle, const char*, const int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setArrData)));
  
  setIntArrData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int,
	      const iBase_TagHandle, const int*, const int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setIntArrData)));
  
  setDblArrData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int,
	      const iBase_TagHandle, const double*, const int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setDblArrData)));
  
  setEHArrData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int,
	      const iBase_TagHandle, const iBase_EntityHandle*, const int,
	      int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setEHArrData)));
  
  rmvArrTag = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle*, const int,
	      const iBase_TagHandle, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_rmvArrTag)));
  
  getData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle, const iBase_TagHandle,
	      char**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getData)));
  
  getIntData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle, const iBase_TagHandle,
	      int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getIntData)));
  
  getDblData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle, const iBase_TagHandle,
	      double*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getDblData)));
  
  getEHData = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle, const iBase_TagHandle,
	      iBase_EntityHandle*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEHData)));
  
  setData = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle, const iBase_TagHandle,
	      const char*, const int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setData)));
  
  setIntData = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle, const iBase_TagHandle,
	      const int, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setIntData)));
  
  setDblData = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle, const iBase_TagHandle,
	      const double, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setDblData)));
  
  setEHData = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle, const iBase_TagHandle,
	      const iBase_EntityHandle, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_setEHData)));
  
  getAllTags = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle, iBase_TagHandle**,
	      int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getAllTags)));
  
  rmvTag = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle, const iBase_TagHandle, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_rmvTag)));
  
  initEntIter = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle, const int,
	      const int, iMesh_EntityIterator*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_initEntIter)));
  
  getNextEntIter = reinterpret_cast
    <void (*)(iMesh_Instance, iMesh_EntityIterator, iBase_EntityHandle*,
	      int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getNextEntIter)));
  
  resetEntIter = reinterpret_cast
    <void (*)(iMesh_Instance, iMesh_EntityIterator, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_resetEntIter)));
  
  endEntIter = reinterpret_cast
    <void (*)(iMesh_Instance, iMesh_EntityIterator, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_endEntIter)));
  
  getEntTopo = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntTopo)));
  
  getEntType = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntType)));
  
  getVtxCoord = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle,
	      double*,  double*,  double*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getVtxCoord)));
  
  getEntAdj = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntityHandle, const int,
	      iBase_EntityHandle**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEntAdj)));
  
  getEnt2ndAdj = reinterpret_cast
    <void (*)(iMesh_Instance, iBase_EntityHandle, int, int,
	      iBase_EntityHandle**, int*, int*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_getEnt2ndAdj)));
  
  subtract = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle,
	      const iBase_EntitySetHandle, iBase_EntitySetHandle*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_subtract)));
  
  intersect = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle,
	      const iBase_EntitySetHandle, iBase_EntitySetHandle*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_intersect)));
  
  unite = reinterpret_cast
    <void (*)(iMesh_Instance, const iBase_EntitySetHandle,
	      const iBase_EntitySetHandle, iBase_EntitySetHandle*, int*)>
    (dlsym(dlib, QUOTED_NAME(iMesh_unite)));
}

