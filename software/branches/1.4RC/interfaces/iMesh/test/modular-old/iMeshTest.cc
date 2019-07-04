#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iomanip>

#include "iMesh.h"
#include "iMeshTest.hh"
#include "TestData.hh"

iMeshTest::iMeshTest() :
  data(NULL), instance((void*)(0xFFFFFFFF)), rootSet((void*)(0xFFFFFFFF)),
      storageOrder(iBase_UNDETERMINED), numPassed(0), numFailed(0),
      thisTestStatus(NOT_TESTED), testInProgress(NUM_FUNCTIONS),
      isThisFuncOK(true), gotRootSet(false), checkErrorHandling(true)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      reportedAdjTable[i][j] = iBase_UNAVAILABLE;
    }
  }
  for (int i = 0; i < NUM_FUNCTIONS; i++) {
    testStatus[i] = NOT_TESTED;
  }
}

iMeshTest::~iMeshTest()
{
  // Don't free "data" or "instance", as they're owned externally.
  delete [] testStatus;
}

void iMeshTest::provideTestData(TestData * const ptrData)
{
  assert(ptrData != NULL);
  data = ptrData;
}

void iMeshTest::provideMeshInstance(const iMesh_Instance inst)
{
  // Not sure there's anything I can safely assert about inst.
  instance = inst;
}

void iMeshTest::provideRootSet(const iBase_EntitySetHandle rootSetIn)
{
  rootSet = rootSetIn;
  gotRootSet = true;
}

void iMeshTest::requireRootSet()
{
  if (!gotRootSet) {
    enum functionEnum actualTIP = testInProgress;
    testInProgress = NUM_FUNCTIONS;
    getRootSet();
    testInProgress = actualTIP;
  }
}

void iMeshTest::printSummary()
{
  cout << "Tests passed:  " << numPassed << endl;
  cout << "Tests failed:  " << numFailed << endl;
  cout << "Percentage: " << std::setprecision(3) << (100.*numPassed)
      / (numPassed + numFailed) << endl;
}

// Functions that essentially can't be tested at all.

bool iMeshTest::dtor()
{
  BEGIN_FUNC(DTOR);
  // dtor is supposed to destroy an instance, but there's no way to test
  // the outcome using the interface.
  int err;
  iMesh_dtor(instance, &err);
  HANDLE_ERROR(err);
  END_FUNC;
}

bool iMeshTest::save(const char *name, const char *options)
{
  BEGIN_FUNC(SAVE);
  // For save, it's possible to test whether a file with the correct
  // full path name exists after the save.
  int err;
  iMesh_save(instance, rootSet, name, options,
      &err, strlen(name), strlen(options));
  HANDLE_ERROR(err);
  struct stat buf;
  int result = stat(name, &buf);
  if (result == 0) {
    err = iBase_SUCCESS;
  }
  else {
    assert(result == -1);
    err = iBase_FILE_NOT_FOUND;
  }
  HANDLE_ERROR(err);
  END_FUNC;
}

bool iMeshTest::areEHValid(const int doReset)
{
  BEGIN_FUNC(ARE_EH_VALID);
  int err, areHandlesInvariant;
  iMesh_areEHValid(instance, doReset, &areHandlesInvariant, &err);
  HANDLE_ERROR(err);
  END_FUNC;
}

// Functions that can only be tested inferentially.

bool iMeshTest::getRootSet()
{
  BEGIN_FUNC(GET_ROOT_SET);
  // getRootSet has to return something, which has to be intelligible to
  // the implementation.  We can't pass judgment on that though, just
  // test that it returns.
  int err;
  iMesh_getRootSet(instance, &rootSet, &err);
  HANDLE_ERROR(err);
  gotRootSet = true;
  END_FUNC;
}

bool iMeshTest::load(const char *name, const char *options)
{
  BEGIN_FUNC(LOAD);
  // load can't fail, and in fact has to read something sensible, but
  // that success must be tested elsewhere.
  int err;
  requireRootSet();
  iMesh_load(instance, rootSet, name, options,
      &err, strlen(name), strlen(options));
  HANDLE_ERROR(err);
  END_FUNC;
}

bool iMeshTest::newMesh(const char *options)
{
  BEGIN_FUNC(NEW_MESH);
  // newMesh can't fail, and should return an empty mesh, but that
  // result must be tested elsewhere.
  int err;
  iMesh_newMesh(options, &instance, &err, strlen(options));
  HANDLE_ERROR(err);
  END_FUNC;
}

// Functions whose results aren't directly testable, but which, if
// known, can guide other tests.

bool iMeshTest::getDfltStorage()
{
  BEGIN_FUNC(GET_DFLT_STORAGE);
  // getDfltStorage should return either iBase_BLOCKED or
  // iBase_INTERLEAVED.
  int err;
  // The following line is redundant until getDfltStorage() has been called.
  storageOrder = iBase_UNDETERMINED;
  iMesh_getDfltStorage(instance, &storageOrder, &err);
  HANDLE_ERROR(err);
  TEST(storageOrder == iBase_BLOCKED || storageOrder == iBase_INTERLEAVED);
  END_FUNC;
}

bool iMeshTest::getAdjTable()
{
  BEGIN_FUNC(GET_ADJ_TABLE);
  // The result here gets stored in an internal adjacency table.
  int err;
  int adjTableAlloc = 16, adjTableSize = 0;
  iMesh_getAdjTable(instance, (int**)(&reportedAdjTable), &adjTableAlloc,
      &adjTableSize, &err);
  HANDLE_ERROR(err);
  TEST(adjTableSize == 16);

  bool qEntriesOK = true;
  for (int i_ = 0; i_ < 4; i_++) {
    for (int j_ = 0; j_ < 4; j_++) {
      if (reportedAdjTable[i_][j_] < iBase_ALL_ORDER_1
          || reportedAdjTable[i_][j_] > iBase_SOME_ORDER_N)
        qEntriesOK = false;
    }
  }
  TEST(qEntriesOK);
  END_FUNC;
}

// Functions that can be tested explicitly and independently, given a
// known set of data.  Note that all of these tests are done on the root
// set. 

bool iMeshTest::getGeometricDimension()
{
  BEGIN_FUNC(GET_GEOMETRIC_DIMENSION);
  // Easy to test.
  int err, geomDim;
  iMesh_getGeometricDimension(instance, &geomDim, &err);
  HANDLE_ERROR(err);
  int correctGeomDim = data->geomDim();
  TEST(geomDim == correctGeomDim);
  END_FUNC;
}

bool iMeshTest::getNumOfTopo(const int entTopo)
{
  BEGIN_FUNC(GET_NUM_OF_TOPO);
  int err, numTopo;
  requireRootSet();
  iMesh_getNumOfTopo(instance, rootSet, entTopo, &numTopo, &err);
  HANDLE_ERROR(err);
  int correctNumTopo = data->numOfTopo(entTopo);
  TEST(numTopo == correctNumTopo);
  END_FUNC;
}

bool iMeshTest::getNumOfType(const int entType)
{
  BEGIN_FUNC(GET_NUM_OF_TYPE);
  int err, numType;
  requireRootSet();
  iMesh_getNumOfType(instance, rootSet, entType, &numType, &err);
  HANDLE_ERROR(err);
  int correctNumType = data->numOfType(entType);
  TEST(numType == correctNumType);
  END_FUNC;
}

bool iMeshTest::getAllVtxCoords(const int order)
{
  BEGIN_FUNC(GET_ALL_VTX_COORDS);
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
      &orderInput, &err);
  HANDLE_ERROR(err);

  int numVerts = data->numOfType(iBase_VERTEX);

  TEST(coordinatesSize == 3*numVerts);
  TEST(coordinatesAlloc >= 3*numVerts);
  TEST(inEntitySetSize == numVerts);
  TEST(inEntitySetAlloc >= numVerts);

  // Shouldn't change the order if it was specified.
  TEST(orderInput == iBase_UNDETERMINED ||
      order == orderInput);

  // inEntitySet had better always be true.
  bool inSetOK = true;
  int i;
  for (i = 0; i < inEntitySetSize; i++) {
    inSetOK = inEntitySet[i];
  }
  TEST(inSetOK);

  // Now check coords against the real thing.
  bool coordsOK = true;
  for (i = 0; i < numVerts && coordsOK; i++) {
    if (order == iBase_BLOCKED) {
      if (coordinates[i + numVerts*0] != data->coords(i, 0)
          || coordinates[i + numVerts*1] != data->coords(i, 1)
          || coordinates[i + numVerts*2] != data->coords(i, 2)) {
        coordsOK = false;
      }
    } // Done with BLOCKED case
    else {
      if (coordinates[i*3 + 0] != data->coords(i, 0) || coordinates[i*3 + 1]
          != data->coords(i, 1) || coordinates[i*3 + 2] != data->coords(i, 2)) {
        coordsOK = false;
      }
    } // Done with INTERLEAVED case
  } // Done looping over verts
  TEST(coordsOK);

  free(coordinates);
  free(inEntitySet);
  coordinates = NULL;
  inEntitySet = NULL;

  // Now check that the function gives proper behavior for erroneous
  // inputs.
  if (checkErrorHandling) {
    // The only bad input scenarios tested here are undersized arrays.
    coordinates = (double*) calloc(1, sizeof(double));
    coordinatesAlloc = 1;
    iMesh_getAllVtxCoords(instance, rootSet,
        &coordinates, &coordinatesAlloc, &coordinatesSize,
        &inEntitySet, &inEntitySetAlloc, &inEntitySetSize,
        &orderInput, &err);
    TEST(err == iBase_BAD_ARRAY_SIZE);

    if (coordinates != NULL)
      free(coordinates);
    if (inEntitySet != NULL)
      free(inEntitySet);
    coordinates = NULL;
    inEntitySet = NULL;

    inEntitySet = (int*) calloc(1, sizeof(int));
    inEntitySetAlloc = 1;
    iMesh_getAllVtxCoords(instance, rootSet,
        &coordinates, &coordinatesAlloc, &coordinatesSize,
        &inEntitySet, &inEntitySetAlloc, &inEntitySetSize,
        &orderInput, &err);
    TEST(err == iBase_BAD_ARRAY_SIZE);

    if (coordinates != NULL)
      free(coordinates);
    if (inEntitySet != NULL)
      free(inEntitySet);
    coordinates = NULL;
    inEntitySet = NULL;
  }
  END_FUNC;
}

