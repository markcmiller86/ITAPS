#include <dlfcn.h>
#include <math.h>

#include "iMeshWrapper.hh"

iMeshWrapper::~iMeshWrapper()
{
  char* errMessage = dlerror();
  if (errMessage)
    std::cout << errMessage << std::endl;
  for (int f = m_numFiles - 1; f >= 0; f--) {
    dlclose(m_libHandles[f]);
    errMessage = dlerror();
    if (errMessage)
      std::cout << errMessage << std::endl;
  }
  delete [] m_libHandles;
}

static bool approxEqual(double a, double b)
{
  return (fabs(a-b) <= 1.e-12*(fabs(a)+fabs(b)));
}

bool VertMatch::operator<(const VertMatch& VM) const
{
  return ((x < VM.x) ||
	  (approxEqual(x, VM.x) && ((y < VM.y) ||
					 (approxEqual(y, VM.y) &&
					  z < VM.z))));
}

bool VertMatch::operator==(const VertMatch& VM) const 
{
  return (approxEqual(x, VM.x) &&
	  approxEqual(y, VM.y) &&
	  approxEqual(z, VM.z));
}

void iMeshWrapper::createVtxArray(iBase_EntityHandle** vtxArray,
				  int &numVerts)
{
  // NOT GENERAL
  // This code assumes that getEntities will work for vertices.
  // Simplest possible approach
  int vtxArray_allocated = 0, err = INT_MIN;
  getEntities(m_instance, m_rootSet, iBase_VERTEX, iMesh_ALL_TOPOLOGIES,
	      vtxArray, &vtxArray_allocated, &numVerts, &err);
  checkError("iMesh_getEntities", __LINE__, __FILE__, err);

  // More complicated versions will eventually follow.
}

void iMeshWrapper::initWrapper(iMesh_Instance instance,
			       iBase_EntitySetHandle rootSet)
{
  m_instance = instance;
  if (rootSet == reinterpret_cast<iBase_EntitySetHandle>(INT_MIN+1) &&
      getRootSet) {
    // This is the default arg, which isn't really the root set.  Since
    // the needed function exists, use it.
    int err;
    getRootSet(m_instance, &m_rootSet, &err);
    checkError("iMesh_getRootSet", __LINE__, __FILE__, err);
  }
  else {
    // Either there's a real rootSet passed in, or no change of getting
    // one.  Either way, use the one passed in.
    m_rootSet = rootSet;
  }
}    

void iMeshWrapper::sortVertices(std::set<VertMatch>& setVerts)
{
  // NOT GENERAL
  // This code assumes that getVtxArrCoords will always work.
  iBase_EntityHandle* vtxArray;
  int numVerts = INT_MIN, err = INT_MIN;
  createVtxArray(&vtxArray, numVerts);

  double dummy = 1;
  double *coords = &dummy;
  int coords_allocated = 0, coords_size;
  getVtxArrCoords(m_instance, vtxArray, numVerts, iBase_INTERLEAVED,
		  &coords, &coords_allocated, &coords_size, &err);
  checkError("iMesh_getVtxArrCoords", __LINE__, __FILE__, err);

  std::pair<std::set<VertMatch>::iterator, bool> result;
  for (int i = 0; i < numVerts; i++) {
    VertMatch VM;
    VM.x = coords[3*i];
    VM.y = coords[3*i+1];
    VM.z = coords[3*i+2];
    VM.handle = vtxArray[i];
    result = setVerts.insert(VM);
    if (!result.second) {
      std::cout << "Found verts with identical locations!" << std::endl;
      std::cout << "  Coords: (" << VM.x << "," << VM.y << "," << VM.z
		<< ")" << std::endl;
      std::cout << "  Handles: " << VM.handle << ", "
		<< result.first->handle << std::endl;
    }
  }
}

// What I need to be able to do (goals):
// 1. Call an arbitrary iMesh function on a given implementation, with
//    proper args.
// 2. Find out whether an arbitrary iMesh function exists at all in an
//    implementation. 

// Strategy, as I now see it:
// 1.  Define internal function pointers for all iMesh functions.
// 2.  Identify these (and cast to right type) in initializing a wrapper
//     object.  (dlsym calls with macro magic for name mangling).
// 3.  The wrapper class has to have two functions for each iMesh
//     function: (perhaps combined in a single name with fancy args?):
//     one to tell whether the iMesh func exists, and one as a wrapper
//     to call it.  The latter needs to error check anyway (does the
//     func exist?).  The wrapper could/should have the same args as the
//     wrapped func, so if the func exists, it could call it and return
//     true, or else return false.  This puts error handling at an
//     appropriate level, I think...

// Actually, I was able to combine 1 and 3 by having the function
// pointers be public.  Plus: simplifies things, because the wrapper
// function goes away.  Minus: have to check elsewhere for the existence
// of a function before calling it (or use try blocks to catch that seg
// fault, maybe?).
