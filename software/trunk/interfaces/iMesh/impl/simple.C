// Simple iMesh Implementation (simple)
// Currently, this is hacked to just explore design
// space using STL containers
//
// Goals:
// 1) Variable precision geometry
// 2) Variable dimension
// 3) Ability to initialize handles to 'non-valid' value
// 4) Ability to distinguish handles from different interface instances
// 5) Maximal use of STL-like library
// 5.5) Support Boost or STL
// 6) Maintain, efficiently, full dimensional cascade.
// 7) Vertex 'handle' is the vertex entity's storage
// 8) Entity handles returned to iMesh client are same used internally
//    by implementation
// 9) Variable sized handles (minimally either 32 or 64 bit).
//

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <typeinfo>
using namespace std;

class entBase
{ public: virtual ~entBase() {;}; };

// Type for a vertex entity (1,2 or 3D, single/double precision
//template<typename T=double, size_t D=3>
class vertEnt : public entBase
{ public: double c[3]; };
typedef const vertEnt* vertPtr;

class compareVerts
{ public: bool operator()(const vertEnt& a, const vertEnt& b){return true;};};

// 2 verts per any edge
class edgeEnt : public entBase
{ public: vertPtr v[2]; };
typedef const edgeEnt* edgePtr;

class compareEdges
{ public: bool operator()(const edgeEnt& a, const edgeEnt& b){return true;};};

// max 4 edges per any FE zoo element face
class faceEnt : public entBase
{ public: edgePtr e[4]; };
typedef const faceEnt* facePtr;

// max 6 faces per any FE zoo element region
class regnEnt : public entBase
{ public: facePtr f[6]; };
typedef const regnEnt* regnPtr;

// Will maintain full dimensional cascade of all elements.
// Entities are stored in sets using their lower-dimensional composition
// as the keys. We haven't defined the key compare method yet but its
// simple. Entity handles will be const refs to entities inserted in
// these sets.
class iMesh_Instance_Private
{
public:
    set<vertEnt, compareVerts> verts;
    set<edgeEnt, compareEdges> edges;
    set<faceEnt> faces;
    set<regnEnt> regns;
};
typedef iMesh_Instance_Private* iMesh_Instance;
//typedef entBase iBase_EntityHandle_Private;
typedef const entBase* iBase_EntityHandle;

  enum iBase_ErrorType {
    iBase_ErrorType_MIN = 0,
    iBase_SUCCESS = iBase_ErrorType_MIN,
    iBase_MESH_ALREADY_LOADED,
    iBase_NO_MESH_DATA,
    iBase_FILE_NOT_FOUND,
    iBase_FILE_WRITE_ERROR,
    iBase_NIL_ARRAY,
    iBase_BAD_ARRAY_SIZE,
    iBase_BAD_ARRAY_DIMENSION,
    iBase_INVALID_ENTITY_HANDLE,
    iBase_INVALID_ENTITY_COUNT,
    iBase_INVALID_ENTITY_TYPE,
    iBase_INVALID_ENTITY_TOPOLOGY,
    iBase_BAD_TYPE_AND_TOPO,
    iBase_ENTITY_CREATION_ERROR,
    iBase_INVALID_TAG_HANDLE,
    iBase_TAG_NOT_FOUND,
    iBase_TAG_ALREADY_EXISTS,
    iBase_TAG_IN_USE,
    iBase_INVALID_ENTITYSET_HANDLE,
    iBase_INVALID_ITERATOR_HANDLE,
    iBase_INVALID_ARGUMENT,
    iBase_MEMORY_ALLOCATION_FAILED,
    iBase_NOT_SUPPORTED,
    iBase_FAILURE,
    iBase_ErrorType_MAX = iBase_FAILURE
  };


void iMesh_createVtx(
    iMesh_Instance instance,
    /*in*/ const double x,
    /*in*/ const double y,
    /*in*/ const double z,
    /*out*/ iBase_EntityHandle* new_vertex_handle,
    /*out*/ int *err)
{
    vertEnt coord;
    coord.c[0] = x;
    coord.c[1] = y;
    coord.c[2] = z;
    pair<set<vertEnt>::iterator,bool> ret = instance->verts.insert(coord);
    *new_vertex_handle = &(*ret.first);
    *err = iBase_SUCCESS;
}

void
iMesh_getVtxCoord(iMesh_Instance instance,
    /*in*/ const iBase_EntityHandle vertex_handle,
    /*out*/ double *x,
    /*out*/ double *y,
    /*out*/ double *z,
    /*out*/ int *err)
{
    vertPtr vr = dynamic_cast<const vertEnt*>(vertex_handle);
    if (vr == 0)
    {
        *err = iBase_INVALID_ENTITY_HANDLE;
	return;
    }
    *x = vr->c[0];
    *y = vr->c[1];
    *z = vr->c[2];
    *err = iBase_SUCCESS;
}

void
iMesh_createEnt(iMesh_Instance instance,
    /*in*/ const int new_entity_topology,
    /*in*/ const iBase_EntityHandle* lower_order_entity_handles,
    /*in*/ const int lower_order_entity_handles_size,
    /*out*/ iBase_EntityHandle* new_entity_handle,
    /*out*/ int* status,
    /*out*/ int *err)
{
    switch (new_entity_topology)
    {
        case 1:
        {
            if (lower_order_entity_handles_size != 2)
            {
                *err = iBase_FAILURE;
                return;
            }
            vertPtr vr0 = dynamic_cast<const vertEnt*>(lower_order_entity_handles[0]);
            vertPtr vr1 = dynamic_cast<const vertEnt*>(lower_order_entity_handles[1]);
            edgeEnt edge;
            edge.v[0] = vr0;
            edge.v[1] = vr1;
            instance->edges.insert(edge);
        }

        case 0:
        default:
        {
            *err = iBase_FAILURE;
            return;
        }
    }
}



int main()
{
    // iMesh language agnostic API
    iMesh_Instance mesh = new iMesh_Instance_Private;
    iBase_EntityHandle v[3];
    int err;
    iMesh_createVtx(mesh, 1.0, 2.0, 3.0, &v[0], &err);
    iMesh_createVtx(mesh, 10.0, 20.0, 30.0, &v[1], &err);
    iMesh_createVtx(mesh, 100.0, 200.0, 300.0, &v[2], &err);
    cout << "#verts = " << mesh->verts.size() << endl;
    double x,y,z;
    iMesh_getVtxCoord(mesh, v[1], &x, &y, &z, &err);
    cout << "Got x,y,z = " << x << "," << y << "," << z << endl;
    iBase_EntityHandle edgeEnt;
    int status;
    iMesh_createEnt(mesh, 1, v, 2, &edgeEnt, &status, &err);
    cout << "#edges = " << mesh->edges.size() << endl;

    // test a failure mode and dynamic cast
#warning DO NOT LIKE INITIALIZING HANDLE
    entBase foo;
    iBase_EntityHandle edgeHandle = &foo;
    iMesh_getVtxCoord(mesh, edgeHandle, &x, &y, &z, &err);
    if (err != iBase_INVALID_ENTITY_HANDLE) cout << "Didn't get an error" << endl;

    return 0;
}
