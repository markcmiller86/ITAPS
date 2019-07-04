#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "iMesh.h"
#include "iMeshTest.hh"

using namespace iMeshTest;

#if __STDC_VERSION__ < 199901L
# if __GNUC__ >= 2
#  warning FUNCTION definition
#  define __func__ __FUNCTION__
# else
#  warning UNKNOWN definition
#  define __func__ "<unknown>"
# endif
#endif


// Functions that essentially can't be tested at all.

iMeshTest::dtor()
{
  // dtor is supposed to destroy an instance, but there's no way to test
  // the outcome using the interface.
  int err;
  iMesh_dtor(instance, &err);
  handleError(err, __LINE__, __FILE__, __func__);
}

iMeshTest::save(const iBase_EntitySetHandle entity_set_handle,
	      const char *name, const char *options,
	      const int nameLen, const in optionsLen)
{
  // For save, it's possible to test whether a file with the correct
  // full path name exists after the save.
  int err;
  iMesh_load(instance, entity_set_handle, name, options,
	     &err, name_len, options_len);
  handleError(err, __LINE__, __FILE__, __func__);
  struct stat buf;
  int result = stat(name, &buf);
  if (result == 0) {
    err = iBase_SUCCESS;
  }
  else {
    assert(result == -1);
    err = iBase_FILE_NOT_FOUND;
  }
  handleError(err, __LINE__, __FILE__, __func__);
}

iMeshTest::areEHValid(const int doReset)
{
  int err, areHandlesInvariant;
  iMesh_areEHValid(instance, doReset, *areHandlesInvariant, &err);
  handleError(err, __LINE__, __FILE__, __func__);
}

// Functions that can only be tested inferentially.

iMeshTest::getRootSet()
{
  // getRootSet has to return something, which has to be intelligible to
  // the implementation.  We can't pass judgment on that though, just
  // test that it returns.
  int err;
  iMesh_getRootSet(instance, &rootSet, &err);
  handleError(err, __LINE__, __FILE__, __func__);
}

iMeshTest::load(const iBase_EntitySetHandle entity_set_handle,
	      const char *name, const char *options,
	      const int nameLen, const in optionsLen)
{
  // load can't fail, and in fact has to read something sensible, but
  // that success must be tested elsewhere.
  int err;
  iMesh_load(instance, entity_set_handle, name, options,
	     &err, name_len, options_len);
  handleError(err, __LINE__, __FILE__, __func__);
}

iMeshTest::newMesh(const char *options, const in optionsLen)
{
  // newMesh can't fail, and should return an empty mesh, but that
  // result must be tested elsewhere.
  int err;
  iMesh_newMesh(options, instance, &err, options_len);
  handleError(err, __LINE__, __FILE__, __func__);
}

// Functions whose results aren't directly testable, but which, if
// known, can guide other tests.

iMeshTest::getDfltStorage()
{
  // getDfltStorage should return either iBase_BLOCKED or
  // iBase_INTERLEAVED.
  int err;
  // The following line is redundant until getDfltStorage() has been called.
  storageOrder = iBase_UNDETERMINED;
  iMesh_getDfltStorage(instance, &order, &err);
  handleError(err, __LINE__, __FILE__, __func__);
  TEST(storageOrder == iBase_BLOCKED || storageOrder == iBase_INTERLEAVED);
}  

iMeshTest::getAdjTable()
{
  // The result here gets stored in an internal adjacency table.
  int err;
  int adjTableAlloc = 16, adjTableSize = 0;
  iMesh_getAdjTable(instance, (int**)reportedAdjTable, &adjTableAlloc,
		    &adjTableSize, &err);
  handleError(err, __LINE__, __FILE__, __func__);
  TEST(adjTableSize == 16);

  bool qEntriesOK = true;
  for (int i_ = 0; i_ < 4; i_++) {
    for (int j_ = 0; j_ < 4; j_++) {
      if (reportedAdjTable[i_][j_] < iBase_ALL_ORDER_1 ||
	  reportedAdjTable[i_][j_] > iBase_SOME_ORDER_N)
	qEntriesOK = false;
    }
  }
  TEST(qEntriesOK);
}

// Functions that can be tested explicitly and independently, given a
// known set of data.  Note that all of these tests are done on the root
// set. 

iMeshTest::getGeometricDimension()
{
  // Easy to test.
  int err, geomDim;
  iMesh_getGeometricDimension(instance, &geomDim, &err);
  handleError(err, __LINE__, __FILE__, __func__);
  TEST(geomDim == 3);
}

iMeshTest::getNumOfTopo(const int entTopo)
{
  int err, numTopo;
  requireRootSet();
  iMesh_getNumOfTopo(instance, rootSet, entTopo, &numTopo, &err);
  handleError(err, __LINE__, __FILE__, __func__);
  int correctNumTopo = actualNumOfTopo(entTopo);
  TEST(numTopo == correctNumTopo);
}

iMeshTest::getNumOfType(const int entType)
{
  int err, numType;
  requireRootSet();
  iMesh_getNumOfType(instance, rootSet, entType, &numType, &err);
  handleError(err, __LINE__, __FILE__, __func__);
  int correctNumType = actualNumOfType(entType);
  TEST(numType == correctNumType);
}

iMeshTest::getAllVtxCoords(const int order)
{
  int err;
  requireRootSet();
  int orderInput = order;
  double *coordinates;
  int *inEntitySet;
  int coordinatesAlloc = 0, coordinatesSize = 0;
  int inEntitySetAlloc = 0, inEntitySetSize = 0;
  iMesh_getAllVtxCoords(instance, rootSet,
			&coordinates, &coordinatesAlloc, &coordinatesSize,
			&inEntitySet, &inEntitySetAlloc, &inEntitySetSize,
			&order, &err);
  handleError(err, __LINE__, __FILE__, __func__);

  int numVerts = actualNumOfType(iBase_VERTEX);

  TEST(coordinatesSize == 3*numVerts);
  TEST(coordinatesAlloc >= 3*numVerts);
  TEST(inEntitySetSize == numVerts);
  TEST(inEntitySetAlloc >= numVerts);
  
  // Shouldn't change the order if it was specified.
  TEST(orderInput == iBase_UNDETERMINED ||
       order == orderInput);

  // inEntitySet had better always be true.
  bool inSetOK = true;
  for (int i = 0; i < inEntitySetSize; i++) {
    inSetOK = inEntitySet[i];
  }
  TEST(inSetOK);

  // Now check coords against the real thing.
  bool coordsOK = true;
  for (i = 0; i < numVerts && coordsOK; i++) {
    if (order == iBase_BLOCKED) {
      if (coordinates[i + numVerts*0] != actualCoords(i, 0) ||
	  coordinates[i + numVerts*1] != actualCoords(i, 1) ||
	  coordinates[i + numVerts*2] != actualCoords(i, 2)) {
	coordsOK = false;
      }
    } // Done with BLOCKED case
    else {
      if (coordinates[i*3 + 0] != actualCoords(i, 0) ||
	  coordinates[i*3 + 1] != actualCoords(i, 1) ||
	  coordinates[i*3 + 2] != actualCoords(i, 2)) {
	coordsOK = false;
      }
    } // Done with INTERLEAVED case
  } // Done looping over verts
  TEST(coordsOK);

  free(coordinates);
  free(inEntitySet);
  coordinates = inEntitySet = NULL;

  // Now check that the function gives proper behavior for erroneous
  // inputs.
  if (checkErrorThrowing) {
    // The only bad input scenarios tested here are undersized arrays.
    coordinates = (double*) calloc(1, sizeof(double));
    coordinatesAlloc = 1;
    iMesh_getAllVtxCoords(instance, rootSet,
			  &coordinates, &coordinatesAlloc, &coordinatesSize,
			  &inEntitySet, &inEntitySetAlloc, &inEntitySetSize,
			  &order, &err);
    TEST(err == iBase_BAD_ARRAY_SIZE);

    if (coordinates != NULL) free(coordinates);
    if (inEntitySet != NULL) free(inEntitySet);
    coordinates = inEntitySet = NULL;

    inEntitySet = (double*) calloc(1, sizeof(double));
    inEntitySetAlloc = 1;
    iMesh_getAllVtxCoords(instance, rootSet,
			  &coordinates, &coordinatesAlloc, &coordinatesSize,
			  &inEntitySet, &inEntitySetAlloc, &inEntitySetSize,
			  &order, &err);
    TEST(err == iBase_BAD_ARRAY_SIZE);

    if (coordinates != NULL) free(coordinates);
    if (inEntitySet != NULL) free(inEntitySet);
    coordinates = inEntitySet = NULL;
  }
}

