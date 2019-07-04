#ifndef iMeshWrapper_h_
#define iMeshWrapper_h_

#include <limits.h>

#include <iostream>
#include <set>

#include "iMesh.h"

typedef struct struct_vert_match {
  double x, y, z;
  void* handle;
  bool operator<(const struct_vert_match& VM) const;
  bool operator==(const struct_vert_match& VM) const;
  bool operator!=(const struct_vert_match& VM) const
  {return !(*this == VM);}
} VertMatch;


class iMeshWrapper {
  bool m_isRefImpl;
  int m_numFiles;
  void **m_libHandles;
  iMesh_Instance m_instance;
  iBase_EntitySetHandle m_rootSet;
  // etc; should autogenerate this part of the file
public:
  iMeshWrapper(std::string filenames[], const int numFiles, bool isRefImpl);
  ~iMeshWrapper();

  iMesh_Instance getInstance() const {return m_instance;}
  iBase_EntitySetHandle getRootSetValue() const {return m_rootSet;}

  void checkError(const std::string funcName, const int lineNum,
		  const std::string fileName, const int error) throw(int)
  {
    if (error == iBase_SUCCESS) return;
    std::cout << "Error code " << error << " was returned from "
	      << funcName << " at " << fileName << ":" << lineNum
	      << std::endl;
    if (getDescription) {
      char descr[1024];
      int err = INT_MIN;
      getDescription(m_instance, descr, &err, 1024);
      // If there was an error here, we're liable to start a cascade, so
      // bail out now!
      if (err == iBase_SUCCESS) {
	std::cout << descr << std::endl;
      }
    }
    throw(error);
  };

  /// Do the upfront stuff that's essential before testing can begin.
  // Use INT_MIN+1 as a default because it can't possibly be a pointer
  // (it's odd) and is unlikely to be an integer handle.
  void initWrapper(iMesh_Instance instance,
		   iBase_EntitySetHandle rootSet =
		   reinterpret_cast<iBase_EntitySetHandle>(INT_MIN+1));

  void createVtxArray(iBase_EntityHandle** vtxArray, int& numVerts);

  void sortVertices(std::set<VertMatch>& setVerts);
  void report()
  {
    std::cout << "iMesh_getDescription:  "
	      << (getDescription ? "found" : "not found") << std::endl;
    std::cout << "iMesh_newMesh:  "
	      << (newMesh ? "found" : "not found") << std::endl;
  }

  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  // Below this point, only clones of iMesh function declarations are
  // given.  Necessary, but very dull, so (a) don't bother reading and
  // (b) don't change anything below this point except to match changes
  // in the API.
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  
  // Global operations
  void (*getErrorType)(iMesh_Instance instance, int* error_type, int* err);
  void (*getDescription)(iMesh_Instance instance, char* descr, int* err, 
			 int descr_len);
  void (*newMesh)(const char* options, iMesh_Instance* instance, int* err, 
		  int options_len);
  void (*dtor)(iMesh_Instance instance, int* err);
  void (*load)(iMesh_Instance instance,
	       const iBase_EntitySetHandle entity_set_handle,
	       const char* name, const char* options, int* err, 
	       int name_len, int options_len);
  void (*save)(iMesh_Instance instance,
	       const iBase_EntitySetHandle entity_set_handle,
	       const char* name, const char* options, int* err, 
	       const int name_len, int options_len);
  void (*getRootSet)(iMesh_Instance instance,
		     iBase_EntitySetHandle* root_set, int* err);
  void (*getGeometricDimension)(iMesh_Instance instance, int* geom_dim, 
				int* err);
  void (*setGeometricDimension)( iMesh_Instance instance, int geom_dim,
				 int* err );
  void (*getDfltStorage)(iMesh_Instance instance, int* order, int* err);
  void (*getAdjTable )(iMesh_Instance instance, int** adjacency_table,
		       int* adj_table_allocated, int* adj_table_size, 
		       int* err);
  void (*getNumOfType)(iMesh_Instance instance,
		       const iBase_EntitySetHandle entity_set_handle,
		       const int entity_type, int* num_type, int* err);
  void (*getNumOfTopo)(iMesh_Instance instance,
		       const iBase_EntitySetHandle entity_set_handle,
		       const int entity_topology, int* num_topo, int* err);
  void (*areEHValid)(iMesh_Instance instance, int doReset,
		     int* areHandlesInvariant, int* err);
  void (*getEntities)(iMesh_Instance instance,
		      const iBase_EntitySetHandle entity_set_handle,
		      const int entity_type, const int entity_topology,
		      iBase_EntityHandle** entity_handles,
		      int* entity_handles_allocated,
		      int* entity_handles_size,
		      int* err);
  void (*getVtxArrCoords)(iMesh_Instance instance,
			  const iBase_EntityHandle* vertex_handles,
			  const int vertex_handles_size,
			  const int storage_order, double** coords,
			  int* coords_allocated, int* coords_size, int* err);
  void (*getAdjEntIndices)(iMesh_Instance instance,
			   iBase_EntitySetHandle entity_set_handle,
			   int entity_type_requestor,
			   int entity_topology_requestor,
			   int entity_type_requested,
			   iBase_EntityHandle** entity_handles,
			   int* entity_handles_allocated,
			   int* entity_handles_size,
			   iBase_EntityHandle** adj_entity_handles,
			   int* adj_entity_handles_allocated,
			   int* adj_entity_handles_size,
			   int** adj_entity_indices,
			   int* adj_entity_indices_allocated,
			   int* adj_entity_indices_size,
			   int** offset, int* offset_allocated,
			   int* offset_size, int* err);

  // Entity operations
  void (*initEntIter)(iMesh_Instance instance,
		      const iBase_EntitySetHandle entity_set_handle,
		      const int requested_entity_type,
		      const int requested_entity_topology,
		      iMesh_EntityIterator* entity_iterator, int* err);
  void (*getNextEntIter)(iMesh_Instance instance,
			 iMesh_EntityIterator entity_iterator,
			 iBase_EntityHandle* entity_handle,
			 int* has_data, int* err);
  void (*resetEntIter)(iMesh_Instance instance,
		       iMesh_EntityIterator entity_iterator, int* err);
  void (*endEntIter)(iMesh_Instance instance,
		     iMesh_EntityIterator entity_iterator, int* err);
  void (*getEntTopo)(iMesh_Instance instance,
		     const iBase_EntityHandle entity_handle, int* out_topo,
		     int* err);
  void (*getEntType)(iMesh_Instance instance,
		     const iBase_EntityHandle entity_handle, int* out_type,
		     int* err);
  void (*getVtxCoord)(iMesh_Instance instance,
		      const iBase_EntityHandle vertex_handle,
		      double* x,  double* y,  double* z, int* err);
  void (*getEntAdj)(iMesh_Instance instance,
		    const iBase_EntityHandle entity_handle,
		    const int entity_type_requested,
		    iBase_EntityHandle** adj_entity_handles,
		    int* adj_entity_handles_allocated,
		    int* adj_entity_handles_size, int* err);
  void (*getEnt2ndAdj)( iMesh_Instance instance,
			iBase_EntityHandle entity_handle,
			int bridge_entity_type,
			int requested_entity_type,
			iBase_EntityHandle** adjacent_entities,
			int* adjacent_entities_allocated,
			int* adjacent_entities_size, int* err );

  // Entity array operations
  void (*initEntArrIter)(iMesh_Instance instance,
			 const iBase_EntitySetHandle entity_set_handle,
			 const int requested_entity_type,
			 const int requested_entity_topology,
			 const int requested_array_size,
			 iMesh_EntityArrIterator* entArr_iterator, int* err);
  void (*getNextEntArrIter)(iMesh_Instance instance,
			    iMesh_EntityArrIterator entArr_iterator,
			    iBase_EntityHandle** entity_handles,
			    int* entity_handles_allocated,
			    int* entity_handles_size, int* has_data, int* err);
  void (*resetEntArrIter)(iMesh_Instance instance,
			  iMesh_EntityArrIterator entArr_iterator, int* err);
  void (*endEntArrIter)(iMesh_Instance instance,
			iMesh_EntityArrIterator entArr_iterator, int* err);
  void (*getEntArrTopo)(iMesh_Instance instance,
			const iBase_EntityHandle* entity_handles,
			const int entity_handles_size,
			int** topology, int* topology_allocated,
			int* topology_size, int* err);
  void (*getEntArrType)(iMesh_Instance instance,
			const iBase_EntityHandle* entity_handles,
			const int entity_handles_size,
			int** type, int* type_allocated,
			int* type_size, int* err);
  void (*getEntArrAdj)(iMesh_Instance instance,
		       const iBase_EntityHandle* entity_handles,
		       const int entity_handles_size,
		       const int entity_type_requested,
		       iBase_EntityHandle** adjacentEntityHandles,
		       int* adjacentEntityHandles_allocated,
		       int* adj_entity_handles_size, int** offset,
		       int* offset_allocated, int* offset_size, int* err);
  void (*getEntArr2ndAdj)( iMesh_Instance instance,
			   iBase_EntityHandle const* entity_handles,
			   int entity_handles_size, int bridge_entity_type,
			   int requested_entity_type,
			   iBase_EntityHandle** adj_entity_handles,
			   int* adj_entity_handles_allocated,
			   int* adj_entity_handles_size, int** offset,
			   int* offset_allocated, int* offset_size, int* err );

  // Single entity modification operators
  void (*setVtxCoord)(iMesh_Instance instance, iBase_EntityHandle vertex_handle,
		      const double x,  const double y, const double z,
		      int* err);
  void (*createVtx)(iMesh_Instance instance, const double x,  const double y,
		    const double z, iBase_EntityHandle* new_vertex_handle,
		    int* err);
  void (*createEnt)(iMesh_Instance instance,
		    const int new_entity_topology,
		    const iBase_EntityHandle* lower_order_entity_handles,
		    const int lower_order_entity_handles_size,
		    iBase_EntityHandle* new_entity_handle, int* status,
		    int* err);
  void (*deleteEnt)(iMesh_Instance instance, iBase_EntityHandle entity_handle,
		    int* err);

  // Array entity modification operations
  void (*setVtxArrCoords)(iMesh_Instance instance,
			  const iBase_EntityHandle* vertex_handles,
			  const int vertex_handles_size,
			  const int storage_order, const double* new_coords,
			  const int new_coords_size, int* err);
  void (*createVtxArr)(iMesh_Instance instance,
		       const int num_verts, const int storage_order,
		       const double* new_coords, const int new_coords_size,
		       iBase_EntityHandle** new_vertex_handles,
		       int* new_vertex_handles_allocated,
		       int* new_vertex_handles_size, int* err);
  void (*createEntArr)(iMesh_Instance instance,
		       const int new_entity_topology,
		       const iBase_EntityHandle* lower_order_entity_handles,
		       const int lower_order_entity_handles_size,
		       iBase_EntityHandle** new_entity_handles,
		       int* new_entity_handles_allocated,
		       int* new_entity_handles_size,
		       int** status, int* status_allocated, int* status_size,
		       int* err);
  void (*deleteEntArr)(iMesh_Instance instance,
		       const iBase_EntityHandle* entity_handles,
		       const int entity_handles_size, int* err);

  // Basic entity set operations
  void (*createEntSet)(iMesh_Instance instance, const int isList,
		       iBase_EntitySetHandle* entity_set_created, int* err);
  void (*destroyEntSet)(iMesh_Instance instance,
			iBase_EntitySetHandle entity_set, int* err);
  void (*isList)(iMesh_Instance instance,
		 const iBase_EntitySetHandle entity_set, int* is_list,
		 int* err);
  void (*getNumEntSets)(iMesh_Instance instance,
			const iBase_EntitySetHandle entity_set_handle,
			const int num_hops, int* num_sets, int* err);
  
  // Entity set membership operations
  void (*getEntSets)(iMesh_Instance instance,
		     const iBase_EntitySetHandle entity_set_handle,
		     const int num_hops,
		     iBase_EntitySetHandle** contained_set_handles,
		     int* contained_set_handles_allocated,
		     int* contained_set_handles_size, int* err);
  void (*addEntToSet)(iMesh_Instance instance,
		      iBase_EntityHandle entity_handle,
		      iBase_EntitySetHandle entity_set, int* err);
  void (*rmvEntFromSet)(iMesh_Instance instance,
			iBase_EntityHandle entity_handle,
			iBase_EntitySetHandle entity_set, int* err);
  void (*addEntArrToSet)(iMesh_Instance instance,
			 const iBase_EntityHandle* entity_handles,
			 int entity_handles_size,
			 iBase_EntitySetHandle entity_set, int* err);
  void (*rmvEntArrFromSet)(iMesh_Instance instance,
			   const iBase_EntityHandle* entity_handles,
			   int entity_handles_size,
			   iBase_EntitySetHandle entity_set, int* err);
  void (*addEntSet)(iMesh_Instance instance,
		    iBase_EntitySetHandle entity_set_to_add,
		    iBase_EntitySetHandle entity_set_handle, int* err);
  void (*rmvEntSet)(iMesh_Instance instance,
		    iBase_EntitySetHandle entity_set_to_remove,
		    iBase_EntitySetHandle entity_set_handle, int* err);
  void (*isEntContained)(iMesh_Instance instance,
			 iBase_EntitySetHandle containing_entity_set,
			 iBase_EntityHandle contained_entity,
			 int* is_contained, int* err);
  void (*isEntArrContained)( iMesh_Instance instance,
			     iBase_EntitySetHandle containing_set,
			     const iBase_EntityHandle* entity_handles,
			     int num_entity_handles, int** is_contained,
			     int* is_contained_allocated,
			     int* is_contained_size, int* err );
  void (*isEntSetContained)(iMesh_Instance instance,
			    const iBase_EntitySetHandle containing_entity_set,
			    const iBase_EntitySetHandle contained_entity_set,
			    int* is_contained, int* err);
  
  // Set heirarchical relations
  void (*addPrntChld)(iMesh_Instance instance,
		      iBase_EntitySetHandle parent_entity_set,
		      iBase_EntitySetHandle child_entity_set, int* err);
  void (*rmvPrntChld)(iMesh_Instance instance,
		      iBase_EntitySetHandle parent_entity_set,
		      iBase_EntitySetHandle child_entity_set, int* err);
  void (*isChildOf)(iMesh_Instance instance,
		    const iBase_EntitySetHandle parent_entity_set,
		    const iBase_EntitySetHandle child_entity_set, int* is_child,
		    int* err);
  void (*getNumChld)(iMesh_Instance instance,
		     const iBase_EntitySetHandle entity_set,
		     const int num_hops, int* num_child, int* err);
  void (*getNumPrnt)(iMesh_Instance instance,
		     const iBase_EntitySetHandle entity_set,
		     const int num_hops, int* num_parent, int* err);
  void (*getChldn)(iMesh_Instance instance,
		   const iBase_EntitySetHandle from_entity_set,
		   const int num_hops,
		   iBase_EntitySetHandle** entity_set_handles,
		   int* entity_set_handles_allocated,
		   int* entity_set_handles_size, int* err);
  void (*getPrnts)(iMesh_Instance instance,
		   const iBase_EntitySetHandle from_entity_set,
		   const int num_hops,
		   iBase_EntitySetHandle** entity_set_handles,
		   int* entity_set_handles_allocated,
		   int* entity_set_handles_size, int* err);

  // Basic tag operations
  void (*createTag)(iMesh_Instance instance, const char* tag_name,
		    const int tag_size, const int tag_type,
		    iBase_TagHandle* tag_handle, int* err,
		    const int tag_name_len);
  void (*destroyTag)(iMesh_Instance instance, iBase_TagHandle tag_handle,
		     const int forced, int* err);
  void (*getTagName)(iMesh_Instance instance,
		     const iBase_TagHandle tag_handle,
		     char* name, int* err, int name_len);
  void (*getTagSizeValues)(iMesh_Instance instance,
			   const iBase_TagHandle tag_handle,
			   int* tag_size, int* err);
  void (*getTagSizeBytes)(iMesh_Instance instance,
			  const iBase_TagHandle tag_handle,
			  int* tag_size, int* err);
  void (*getTagHandle)(iMesh_Instance instance,
		       const char* tag_name,
		       iBase_TagHandle* tag_handle, int* err, int tag_name_len);
  void (*getTagType)(iMesh_Instance instance,
		     const iBase_TagHandle tag_handle, int* tag_type, int* err);


  // Tag operations for entities
  void (*getData)(iMesh_Instance instance,
		  const iBase_EntityHandle entity_handle,
		  const iBase_TagHandle tag_handle,
		  char** tag_value, int* tag_value_allocated,
		  int* tag_value_size, int* err);
  void (*getIntData)(iMesh_Instance instance,
		     const iBase_EntityHandle entity_handle,
		     const iBase_TagHandle tag_handle, int* out_data, int* err);
  void (*getDblData)(iMesh_Instance instance,
		     const iBase_EntityHandle entity_handle,
		     const iBase_TagHandle tag_handle, double* out_data,
		     int* err);
  void (*getEHData)(iMesh_Instance instance,
		    const iBase_EntityHandle entity_handle,
		    const iBase_TagHandle tag_handle,
		    iBase_EntityHandle* out_data, int* err);
  void (*setData)(iMesh_Instance instance,
		  iBase_EntityHandle entity_handle,
		  const iBase_TagHandle tag_handle,
		  const char* tag_value, const int tag_value_size, int* err);
  void (*setIntData)(iMesh_Instance instance,
		     iBase_EntityHandle entity_handle,
		     const iBase_TagHandle tag_handle, const int tag_value,
		     int* err);
  void (*setDblData)(iMesh_Instance instance,
		     iBase_EntityHandle entity_handle,
		     const iBase_TagHandle tag_handle,
		     const double tag_value, int* err);
  void (*setEHData)(iMesh_Instance instance,
		    iBase_EntityHandle entity_handle,
		    const iBase_TagHandle tag_handle,
		    const iBase_EntityHandle tag_value, int* err);
  void (*getAllTags)(iMesh_Instance instance,
		     const iBase_EntityHandle entity_handle,
		     iBase_TagHandle** tag_handles,
		     int* tag_handles_allocated, int* tag_handles_size,
		     int* err);
  void (*rmvTag)(iMesh_Instance instance,
		 iBase_EntityHandle entity_handle,
		 const iBase_TagHandle tag_handle, int* err);

  // Tag operations for entity arrays
  void (*getArrData)(iMesh_Instance instance,
		     const iBase_EntityHandle* entity_handles,
		     const int entity_handles_size,
		     const iBase_TagHandle tag_handle,
		     char** tag_values, int* tag_values_allocated,
		     int* tag_values_size, int* err);
  void (*getIntArrData)(iMesh_Instance instance,
			const iBase_EntityHandle* entity_handles,
			const int entity_handles_size,
			const iBase_TagHandle tag_handle,
			int** tag_values, int* tag_values_allocated,
			int* tag_values_size, int* err);
  void (*getDblArrData)(iMesh_Instance instance,
			const iBase_EntityHandle* entity_handles,
			const int entity_handles_size,
			const iBase_TagHandle tag_handle,
			double** tag_values, int* tag_values_allocated,
			int* tag_values_size, int* err);
  void (*getEHArrData)(iMesh_Instance instance,
		       const iBase_EntityHandle* entity_handles,
		       const int entity_handles_size,
		       const iBase_TagHandle tag_handle,
		       iBase_EntityHandle** tag_value,
		       int* tag_value_allocated, int* tag_value_size,
		       int* err);
  void (*setArrData)(iMesh_Instance instance,
		     const iBase_EntityHandle* entity_handles,
		     const int entity_handles_size,
		     const iBase_TagHandle tag_handle, const char* tag_values,
		     const int tag_values_size, int* err);
  void (*setIntArrData)(iMesh_Instance instance,
			const iBase_EntityHandle* entity_handles,
			const int entity_handles_size,
			const iBase_TagHandle tag_handle, const int* tag_values,
			const int tag_values_size, int* err);
  void (*setDblArrData)(iMesh_Instance instance,
			const iBase_EntityHandle* entity_handles,
			const int entity_handles_size,
			const iBase_TagHandle tag_handle,
			const double* tag_values, const int tag_values_size,
			int* err);
  void (*setEHArrData)(iMesh_Instance instance,
		       const iBase_EntityHandle* entity_handles,
		       const int entity_handles_size,
		       const iBase_TagHandle tag_handle,
		       const iBase_EntityHandle* tag_values,
		       const int tag_values_size, int* err);
  void (*rmvArrTag)(iMesh_Instance instance,
		    const iBase_EntityHandle* entity_handles,
		    const int entity_handles_size,
		    const iBase_TagHandle tag_handle, int* err);
  
  // Tag operations for entity sets
  void (*setEntSetData)(iMesh_Instance instance,
			iBase_EntitySetHandle entity_set_handle,
			const iBase_TagHandle tag_handle,
			const char* tag_value, const int tag_value_size,
			int* err);
  void (*setEntSetIntData)(iMesh_Instance instance,
			   iBase_EntitySetHandle entity_set,
			   const iBase_TagHandle tag_handle,
			   const int tag_value, int* err);
  void (*setEntSetDblData)(iMesh_Instance instance,
			   iBase_EntitySetHandle entity_set,
			   const iBase_TagHandle tag_handle,
			   const double tag_value, int* err);
  void (*setEntSetEHData)(iMesh_Instance instance,
			  iBase_EntitySetHandle entity_set,
			  const iBase_TagHandle tag_handle,
			  const iBase_EntityHandle tag_value, int* err);
  void (*getEntSetData)(iMesh_Instance instance,
			const iBase_EntitySetHandle entity_set_handle,
			const iBase_TagHandle tag_handle,
			char** tag_value, int* tag_value_allocated,
			int* tag_value_size, int* err);
  void (*getEntSetIntData)(iMesh_Instance instance,
			   const iBase_EntitySetHandle entity_set,
			   const iBase_TagHandle tag_handle,
			   int* out_data, int* err);
  void (*getEntSetDblData)(iMesh_Instance instance,
			   const iBase_EntitySetHandle entity_set,
			   const iBase_TagHandle tag_handle,
			   double* out_data, int* err);
  void (*getEntSetEHData)(iMesh_Instance instance,
			  const iBase_EntitySetHandle entity_set,
			  const iBase_TagHandle tag_handle,
			  iBase_EntityHandle* out_data, int* err);
  void (*getAllEntSetTags)(iMesh_Instance instance,
			   const iBase_EntitySetHandle entity_set_handle,
			   iBase_TagHandle** tag_handles,
			   int* tag_handles_allocated,
			   int* tag_handles_size, int* err);
  void (*rmvEntSetTag)(iMesh_Instance instance,
		       iBase_EntitySetHandle entity_set_handle,
		       const iBase_TagHandle tag_handle, int* err);

  
  // Set boolean operations
  void (*subtract)(iMesh_Instance instance,
		   const iBase_EntitySetHandle entity_set_1,
		   const iBase_EntitySetHandle entity_set_2,
		   iBase_EntitySetHandle* result_entity_set, int* err);
  void (*intersect)(iMesh_Instance instance,
		    const iBase_EntitySetHandle entity_set_1,
		    const iBase_EntitySetHandle entity_set_2,
		    iBase_EntitySetHandle* result_entity_set, int* err);
  void (*unite)(iMesh_Instance instance,
		const iBase_EntitySetHandle entity_set_1,
		const iBase_EntitySetHandle entity_set_2,
		iBase_EntitySetHandle* result_entity_set, int* err);
};
#endif
