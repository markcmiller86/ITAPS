#ifndef _ITAPS_iMeshP_EXT
#define _ITAPS_iMeshP_EXT

#include "iMesh.h"
#include "iMeshP.h"

/**
*  \page imeshp_ext IMeshP_ext:  Experimental, extended ITAPS Parallel Mesh Interface
*  \defgroup imeshp_ext IMeshP_ext:  Experimental, extended ITAPS Parallel Mesh Interface

iMeshP_ext.h -- ITAPS Parallel Extended Mesh Interface

Release 0.1; Created June 2012

\section INTRO Overview
ITAPS' iMeshP extended interface provides functions to create, query and modify a parallel distribution of a mesh with automatic tag data management focused on mutiple part per process environment. 

In iMesh and iMeshP data model, a partition is a high-level description of the distribution of mesh entities assigning entities to subsets called parts. The partition maps each part to a process such that each process may have zero, one, or many parts. Parts are identified globally by unique part IDs and, within a process, by opaque part handles. Each entity physically created in a part. Each entity set physically created in the mesh instance. The entity set consists of other entity sets and mesh entities from one or more parts.

For entities duplicated and shared on multiple parts termed "part boundary entity", one part owns the entities while other part(s) have copies of the entities. The residence part is the part where the entity physically exists as a owner or copies. The source part is the local part of the entity. Regardless of entity status - owner or duplicate copy as part boundary entity or ghost copy, each entity has a source part and one or more residence parts. The residence part is the source part plus other parts wherever the entity exists.

 - For an internal entity created on part 2, the source and residence part are part 2.

 - For a part boundary entity existing on part 1, 2, and 3, the source part ID of the entity on part 1 is 1 and residence part ID's are 1, 2, and 3. The source part ID of the entity on part 2 is 2 and the residence part ID's are 1, 2, and 3. 

 - For a ghost entity existing on part 1 with its owner copy on part 2, the source part ID is 1 and the residence part ID's are 1 and 2. For its owner copy existing on part 2, the source part ID is 2 and the residence part ID's are 1 and 2.

To compute the entity's owning part, the entity's source part should be known for each entity. For instance, the owning part ID of internal entity is its source part ID. In addition to the entity's owning part function, any mesh modification functions also need entity's source part since the entity ownership imbues the right to modify. If the implementation doesn't maintain entity's source part at the entity level, entity's source part should be provided externally.

The mesh migration is a procedure to exchange mesh entities and any relevant data between parts and is the primary tool to accomplish global/local mesh partitioning. For some case, migrating an entity set is also desirable between parts. Therefore, in distributed mesh, both mesh entities and entity set can be designated to migrate. The basic unit to assign destination part in migration is termed as "partition object" and it includes entity with no upward adjacent entities and entity set consisting of such a entities. The Zoltan partitioner efficiently and effectively supports the partitioning graph consisting of entities and entity sets. Entity set migration is proven to be an essential tool for parallel boundary layer mesh adaptation. 

For the sake of the application, in addition to the tag data migration for inidividual entities, iMeshP extended interface allows to register tag handles to automatically migrate during entity or entity set migration.

\section ADD Added functions

The iMeshP Extended interface provides additional functions to support advanced parallel mesh manipulation as the following

 [1] paralell mesh modification 
     - on-process: creating/deleting entity and entity set on a part
     - off-process: migrating entity and entity set between parts
 [2] extending existing iMeshP functions to account for the implementation which doesn't store entity's source part.
      - getting entity's owning part
      - migration
 [3] tag handle registration/un-registration for automatic tag data migration.
 [4] Zoltan partitioner parameter control for static and dynamic partitioning. 
     - Static mesh partitioning is to load and distribute serial mesh in parallel. 
     - Dynamic mesh partitioning is local/global mesh re-distribution during parallel mesh modification
 [5] global/Local mesh partitioning
 [6] neighboring process inquiry per entity type
 [7] printing distributed mesh information (partitioning quality & mesh size info)

The interfaces to be covered but not included are
 [1] more parallel mesh modification including
      - part boundary link update after creating entities on part boundary
      - investigating the feasibility and use case of iMeshP_replaceOnPartBdry 
      - investigating the feasibility and use case of iMeshP_updateVtxCoords
 [2] Zoltan weight control for entity and entity set 

*
*\{
*/

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Compile time version number digits
 *
 * iMesh maintains a major, minor and patch digit in its version number.
 * Technically speaking, there is not much practical value in patch digit
 * for an interface specification. A patch release is typically only used
 * for bug fix releases. Although it is rare, sometimes a bug fix
 * necessitates an API change. So, we define a patch digit for iMesh.
 ******************************************************************************/
#define IMESHP_EXT_VERSION_MAJOR ITAPS_VERSION_MAJOR
#define IMESHP_EXT_VERSION_MINOR ITAPS_VERSION_MINOR
#define IMESHP_EXT_VERSION_PATCH ITAPS_VERSION_PATCH

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Maintain backward compatibility with old version symbol names
 ******************************************************************************/
#define IMESHP_EXT_MAJOR_VERSION IMESHP_EXT_VERSION_MAJOR
#define IMESHP_EXT_MINOR_VERSION IMESHP_EXT_VERSION_MINOR
#define IMESHP_EXT_PATCH_VERSION IMESHP_EXT_VERSION_PATCH

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Version Comparison
 *
 * Evaluates to true at CPP time if the version of iMesh currently being
 * compiled is greater than or equal to the version specified.
 ******************************************************************************/
#define IMESHP_EXT_VERSION_GE(Maj,Min,Pat) ITAPS_VERSION_GE(Maj,Min,Pat)

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Compose string represention of the iMesh version number
 ******************************************************************************/
#define IMESHP_EXT_VERSION_STRING ITAPS_VERSION_STRING_(iMeshP)

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Compose a symbol name derived from the current iMesh version number.
 ******************************************************************************/
#define IMESHP_EXT_VERSION_TAG ITAPS_VERSION_TAG_(iMeshP)

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Define iMesh_newMesh symbol such that it depends on version number.
 *
 * Note: We ran into problems with this as it influences or is influenced by
 * fortran name mangling and so breaks fortran compilation. So, this is
 * currently disabled.
 ******************************************************************************/
#define IMESHP_EXT_NEW_MESH_NAME__(A,B,C) A##_##B##_##C
#define IMESHP_EXT_NEW_MESH_NAME_(A,B,C) IMESHP_EXT_NEW_MESH_NAME__(A,B,C)
#define IMESHP_EXT_NEW_MESH_NAME(A) IMESHP_EXT_NEW_MESH_NAME_(A,IMESHP_EXT_VERSION_MAJOR,IMESHP_EXT_VERSION_MINOR)

#ifdef __cplusplus
extern "C" {
#endif

/** Types for Zoltan LB_Method. */
enum iMeshP_LB_Method
{
  iMeshP_LB_Method_MIN = 0,
  iMeshP_RCB = iMeshP_LB_Method_MIN,  /**< for recursive coordinate bisection */
  iMeshP_RIB,  /**< for recursive inertial bisection */
  iMeshP_GRAPH,  /**< to choose from collection of methods for graphs */
  iMeshP_HYPERGRAPH,  /**< to choose from a collection of methods for hypergraphs */
  iMeshP_PARMETIS,  /**< to choose from collection of methods in ParMETIS graph partitioning library */
  iMeshP_LB_Method_MAX = iMeshP_PARMETIS
};

/** Types for Zoltan LB_Approach. */
enum iMeshP_LB_Approach
{
  iMeshP_LB_Approach_MIN = 0,
  iMeshP_PARTITION = iMeshP_LB_Approach_MIN,   /**< Partition from scratch, not taking into account the current data dis-
                                                    tribution; this option is recommended for static load balancing. */
  iMeshP_REPARTITION, /**< Partition but take into account current data distribution to keep
                           data migration low; this option is recommended for dynamic load balancing. */
  iMeshP_REFINE,  /**< Quickly improve the current data distribution. */
  iMeshP_LB_Approach_MAX = iMeshP_REFINE
};
  
/** Types for Zoltan ParMETIS_Method. */
enum iMeshP_PARMETIS_Method
{
  iMeshP_PARMETIS_Method_MIN = 0,
  iMeshP_PartKway = iMeshP_PARMETIS_Method_MIN,  /**< - multilevel Kernighan-Lin partitioning */
  iMeshP_PartGeom,  /**< space filling curves (coordinate based) */
  iMeshP_PartGeomKway,  /**< hybrid method based on PartKway and PartGeom (needs both graph data and coordinates) */
  iMeshP_AdaptiveRepart,  /**< adaptive repartitioning (only in ParMETIS 3.0 and higher) */
  iMeshP_RefineKway,  /**< refine the current partition (balance) */
  iMeshP_PARMETIS_Method_MAX = iMeshP_RefineKway 
};
 

/** Predefined variable indicating no value. */
#define iMeshP_NONE -1

/**\} */  /* end group imeshp_defs */

/**\defgroup imeshp_localmod Requests for on-processor mesh modification
 *\{
 */

/***************************************************************************//**
 * \brief  Create a new vertex at specified coordinates
 *
 *  Create a new vertex at specified coordinates.
 *
 *  Correspondence in iMesh: iMesh_createVtx
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/

void iMeshP_createVtx(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartHandle part, 
        /**< [in] The part handle in which the vertex is created. */
    const double x, 
        /**< [in] x coordinate of new vertex */
    const double y, 
        /**< [in] y coordinate of new vertex */
    const double z, 
        /**< [in] z coordinate of new vertex */
    iBase_EntityHandle* new_vertex_handle, 
        /**< [out] Pointer to new vertex handles returned from  */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief  Create a new entity with specified lower-order topology.
 *
 *  Given partition and part handle, create an owner or remote copy entity 
 *  with specified lower-order topology.  
 *  Specified new_entity_topology must be value in iMesh_EntityTopology
 *  enumeration. Value returned as status must be a value in the
 *  iBase_CreationStatus enumeration.
 *
 *  Correspondence in iMesh: iMesh_createEnt
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/

void iMeshP_createEnt(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartHandle part, 
        /**< [in] The part handle in which the entity is created. */
    const int new_entity_topology, 
        /**< [in] Topology of created entity */
    const iBase_EntityHandle* lower_order_entity_handles, 
        /**< [in] Array of lower order entity handles to be used to construct
           new entity. */
    const int lower_order_entity_handles_size, 
        /**< [in] Number of entities in lower_order_entity_handles array */
    iBase_EntityHandle* new_entity_handle,
        /**< [out] Pointer to new entity handle returned from  */
    int* status, 
        /**< [out] Pointer to creation status returned from function
           (see iBase_CreationStatus) */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief  Delete specified entity from a part
 *
 * Delete specified entity from a part
 * 
 *  Correspondence in iMesh: iMesh_deleteEnt
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/

void iMeshP_deleteEnt(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartHandle part, 
        /**< [in] The part handle from which the entity is deleted. */
    iBase_EntityHandle entity_handle, 
        /**< [in] Entity to be deleted */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);


/***************************************************************************//**
 * \brief  Create an array of new vertices at specified coordinates
 *
 *  Create an array of new vertices at specified coordinates.  Value of
 *  storage_order must be either iBase_INTERLEAVED or iBase_BLOCKED. 
 *  This function is used for creating owned or remote copy vertices.
 *
 *  Correspondence in iMesh: iMesh_createVtxArr
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/

void iMeshP_createVtxArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartHandle part, 
        /**< [in] The part handle in which the vertices are created. */
    const int num_verts, 
        /**< [in] Number of new vertices to be created */
    const int storage_order, 
        /**< [in] Storage order of coordinates in new_coords array
             (see iBase_StorageOrder) */
    const double* new_coords, 
        /**< [in] Array of coordinates of new vertices. Should be G*num_verts
           in length where G is geometric dimension of the mesh. */
    const int new_coords_size, 
        /**< [in] Number of coordinates in new_coords array, should */
    iBase_EntityHandle** new_vertex_handles, 
        /**< [in,out] Pointer to array of new vertex handles \ref trio) */
    int* new_vertex_handles_allocated, 
        /**< [in,out] Pointer to allocated size of  */
    int* new_vertex_handles_size, 
        /**< [out] Pointer to occupied size of  */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief  Create an array of new entities with specified lower-order topology
 *
 *  Create an array of new entities with specified lower-order topology.  
 *  Specified new_entity_topology must be value in iMesh_EntityTopology
 *  enumeration.  Values return in status array must be values in the
 *  iBase_CreationStatus enumeration.
 *  This function is used for creating owned or remote copy entities.
 *
 *  Correspondence in iMesh: iMesh_createEntArr
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/

void iMeshP_createEntArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartHandle part, 
        /**< [in] The part handle in which the entities are created. */
    const int new_entity_topology, 
        /**< [in] Topology of created entity */
    const iBase_EntityHandle* lower_order_entity_handles, 
        /**< [in] Array of lower order entity handles to be used to construct
             new entities */
    const int lower_order_entity_handles_size, 
        /**< [in] Number of entities in lower_order_entity_handles array */
    iBase_EntityHandle** new_entity_handles, 
        /**< [in,out] Pointer to array of new_entity_handles
             \ref trio) */
    int* new_entity_handles_allocated, 
        /**< [in,out] Pointer to allocated size of  */
    int* new_entity_handles_size, 
        /**< [out] Pointer to occupied size of  */
    int** status, 
        /**< [in,out] Pointer to array of creation status returned from
           \ref trio) (see iBase_CreationStatus) */
    int* status_allocated, 
        /**< [in,out] Pointer to allocated size of status array */
    int* status_size, 
        /**< [out] Pointer to occupied size of status array */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \ingroup  Entities
 * \brief  Delete specified entities from the part
 *
 *  Delete specified entities
 * 
 *  Correspondence in iMesh: iMesh_deleteEntArr
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/

void iMeshP_deleteEntArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartHandle part, 
        /**< [in] The part handle from which the entities are deleted. */
    const iBase_EntityHandle* entity_handles, 
        /**< [in] Array of entity handles to be deleted */
    const int entity_handles_size, 
        /**< [in] Number of entities in array to be deleted */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief  Create an entity set
 *
 *  Given a partition and part handle, create an entity set, either ordered 
 *  (isList=1) or unordered (isList=0) in the part.
 *  Unordered entity sets can contain a given entity or set only once.
 *
 *  Correspondence in iMesh: iMesh_createEntSet
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/

void iMeshP_createEntSet(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartHandle part, 
        /**< [in] The part handle in which the entity set is created. */
    const int isList, 
        /**< [in] If non-zero, an ordered list is created, otherwise an
           unordered set is created. */
    iBase_EntitySetHandle* entity_set_created, 
        /**< [out] Entity set created by function */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief  Delete an entity set
 * 
 *  Correspondence in iMesh: iMesh_destroyEntSet
 * 
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/

void iMeshP_deleteEntSet(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartHandle part, 
        /**< [in] The part handle from which the entity is deleted. */
    iBase_EntitySetHandle entity_set, 
        /**< [in] Entity set to be deleted */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_localmod */


/**\defgroup imeshp_remotemod Requests for off-processor mesh modification
 *\{
 */

/***************************************************************************//**
 * \brief Request migrating an entity and downward adjacencies.
 *
 *  This function migrate the entity and its downward adjacencies 
 *  to other part. This operation may require multiple
 *  rounds of communication, and at some times, certain entities may be
 *  locked (unavailable for local modification) while info about their
 *  remote copies is still in question.  Tags and parallel set membership 
 *  are migrated as well as the appropriate adjacency info.
 *  An iMeshP request handle is returned.
 *
 *  Correspondence in iMeshP: iMeshP_migrateEntity
 * 
 *  \xrefitem comm "Communication" "Communication" point-to-point, non-blocking,
 *  pull. 
 ******************************************************************************/
void iMeshP_migrateEnt(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_PartHandle source_part, 
        /**< [in] The part handle from which the entity is migrated. */
    iBase_EntityHandle local_entity, 
        /**< [in] The local entity copy for the entity to be migrated. */
    iMeshP_Part dest_part, 
        /**< [in] The part id to which the entity is migrated. */
    iMeshP_RequestHandle* request, 
        /**< [out] The iMeshP request handle returned. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Migrate an entity set to a designated part
 *
 *  Given an entity set and a destination part ID, this function migrate 
 *  an entity set and all consisting entities to the designated part. 
 *  This operation may require multiple rounds of communication, and at some 
 *  times, certain entities may be locked (unavailable for local modification) 
 *  while info about their remote copies is still in question.  
 *  Registerd tags by iMeshP_migrateTag are migrated as well as the appropriate 
 *  adjacency info. An iMeshP request handle is returned.
 *
 *  \xrefitem comm "Communication" "Communication" point-to-point, non-blocking,
 *  pull. 
 ******************************************************************************/
void iMeshP_migrateEntSet(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_PartHandle source_part, 
        /**< [in] The part from which the set is migrated. */
    iBase_EntitySetHandle local_entityset, 
        /**< [in] The local entity set to migrate. */
    iMeshP_Part dest_part_id, 
        /**< [in] The destination part id to which the entityset is migrated. */  
    iMeshP_RequestHandle* request, 
        /**< [out] The iMeshP request handle returned. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Exchange entities and entity sets between parts
 *
 *  Given the list of partition object entity and entity set, migrate them
 *  from source part to destination parts.  The entities in the list of entities 
 *  and entity sets are partition object entities which do not have higher order
 *  adjacent entities (e.g. regions in 3D manifold mesh, faces in 2D manifold mesh).
 *  The entities and entity set are entirely migrated to the destination parts.  
 *  If an entity is found in both entity list and entity set and the destination 
 *  parts of entity sand entity set mismatch, the destination part of entity set 
 *  takes priority. The entities' downward adjacencies are also copied and/or
 *  migrated as appropriate to support the entities.
 *  This function is a collective, non-blocking operation
 *  to be called by all processes in the partition's communicator.
 *  An iMeshP_RequestHandle is returned; any of the
 *  iMeshP_wait* functions can be used to block until the request is completed.
 *
 * Correspondence in iMeshP: iMeshP_exchEntArrToPartsAll
 * 
 *  \xrefitem comm "Communication" "Communication" Collective.  Non-blocking.
 ******************************************************************************/
void iMeshP_migratePtnObj(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] Handle for the partition being queried. */
    const iMeshP_PartHandle *part_handles,    
        /**< [in] The source part handles where the entities and sets exists. 
                  Its size should be is entities_size+entitysets_size */
    const iBase_EntityHandle* entities, 
        /**< [in] Entities to be sent. */
    const int entities_size, 
        /**< [in] Number of entities to be sent. */
    const iBase_EntitySetHandle entitysets, 
        /**< [in] Entity sets to be sent. */
    const int entitysets_size, 
        /**< [in] Number of entity sets to be sent. */
    const iMeshP_Part* dest_part_ids, 
        /**< [in] Array of size entities_size+entitysets_size listing 
        the parts to which the entities should be sent. */
    int update_ghost, 
        /**< [in] Flag indicating whether (1) or not (0 ghost copies of the
        entities should be updated with new owner information. */
    iMeshP_RequestHandle* request, 
        /**< [out] iMeshP RequestHandle returned; can be used for blocking
        until this send is complete. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);


/**\} */  /* end group imeshp_remotemod */

/**\defgroup imeshp_set Parts and Sets
 *\{
 */
/***************************************************************************//**
 * \brief Return the part ID of the part owning an entity.
 *
 *  Given an entity handle and a partition handle and a part handle, return the part ID 
 *  of the part that owns the entity.
 *  Return an error code if an entity is not in the part.
 *
 * Correspondence in iMeshP: iMeshP_getEntOwnerPart
 * 
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getEntOwnPart(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in]  The entity's source part handle. */
    const iBase_EntityHandle entity, 
        /**< [in] Entity handle whose owning part is to be returned. */
    iMeshP_Part* owner_part_id, 
        /**< [out] Part ID of the part owning the entity. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the part IDs of the parts owning the given entities.
 *
 *  Given an array of entity handles, a partition handle, and an array of 
 *  source part handles, return the part ID of the part that owns the entity 
 *  for each entity handle. Return an error code if an entity is not in the part.
 *
 * Correspondence in iMeshP: iMeshP_getEntOwnerPartArr
 * 
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getEntArrOwnPart(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle *part_handles,    
        /**< [in] The source part handles where the entities exist */
    const iBase_EntityHandle* entities, 
        /**< [in] Entity handles whose owning part is to be returned. */
    const int entities_size, 
        /**< [in] Number of entities in entities array. */
    iMeshP_Part** owner_part_ids, 
        /**< [out] Part IDs of the parts owning the entities. */
    int* part_ids_allocated, 
        /**< [in,out] Allocated size of part_ids array. */
    int* part_ids_size, 
        /**< [out] Occupied size of part_ids array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_sets */

/**\defgroup imeshp_commtag Functions to automatically migrate Tag data along the migration
 *\{
 */

/***************************************************************************//**
 * \brief Asynchronously send tag data for individual entities.
 *
 *  Send tag information for the specified entities.
 *  The tag data is "pushed" from the owner entities to all copies.
 *  This function assumes tag handles given on various
 *  calling parts are consistent; i.e. they have the same name,
 *  data type, size, etc.  This call blocks until communication is
 *  completed.
 *
 *  Correspondence in iMeshP: iMeshP_iPushTagsEnt
 * 
 *  \xrefitem comm "Communication" "Communication" point-to-point, non-blocking.
 ******************************************************************************/
void iMeshP_migrateTagData(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iBase_TagHandle source_tag, 
        /**< [in] Tag handle for the sending entities. */
    const iMeshP_PartHandle *part_handles,    
        /**< [in] The source part handles where the individual entities exist */
    const iBase_EntityHandle* entities, 
        /**< [in] Owned entities for which to send data. */
    int entities_size, 
        /**< [in] The number of entities for which to send data. */
    iMeshP_RequestHandle* request, 
        /**< [out] The iMeshP request handle returned. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Register a tag handle to automatically migrate in
 *        iMeshP_migrateEnt, iMeshP_migrateEntSet and iMeshP_migratePtnObj
 *
 *  Register tag handle for automatic tag data migration for entities of specified 
 *  type and/or topology, and/or entity set. For tag handle registered, the tag data
 *  is automatically "pushed" from the owner entities to all copies during migration
 *  performed by iMeshP_migrateEnt, iMeshP_migrateSet or iMeshP_migratePtnObj.
 *
 *  This version operates on all entities of specified type and topology 
 *  (all types/topologies if iBase_ALL_TYPES/iMesh_ALL_TOPOLOGIES are
 *  given or no types/topologies if iMeshP_NONE is given) and entity sets.  
 *  This function assumes tag handles given on various
 *  calling parts are consistent; i.e. they have the same name,
 *  data type, size, etc.  
 *  For a tag handle not registed for migration, tag data is lost entirely during 
 *  migratiton iMeshP_migrateEnt, iMeshP_migrateSet or iMeshP_migratePtnObj
 *
 *  \xrefitem comm "Communication" "Communication" None++
 ******************************************************************************/
void iMeshP_migrateTag(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being set. */
    iBase_TagHandle tag_handle, 
        /**< [in] Tag handle to register for automatic migration along with entity/set migration */
    int entity_type, 
        /**< [in] Tag data is exchanged only for this entity type. */
    int entity_topo, 
        /**< [in] Tag data is exchanged only for this entity topology. */
    int include_entity_set, 
        /**< [in] If non-zero, tag data is exchanged for entity set. 
                  If zero, tag data is not exchanged for entity set */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Register tag handles to automatically migrate in 
 *        iMeshP_migrateEnt, iMeshP_migrateEntSet and iMeshP_migratePtnObj
 *
 *  For each tag handle in the array of tag handles,  Register tag handle 
 *  for automatic tag data migration for entities of specified 
 *  type and/or topology, and/or entity set. For tag handle registered, the tag data
 *  is automatically "pushed" from the owner entities to all copies during migration
 *  performed by iMeshP_migrateEnt, iMeshP_migrateSet or iMeshP_migratePtnObj.
 * 
 *  This version operates on all entities of specified type and topology 
 *  (all types/topologies if iBase_ALL_TYPES/iMesh_ALL_TOPOLOGIES are
 *  given or no types/topologies if iMeshP_NONE is given) and entity sets.  
 *  This function assumes tag handles given on various
 *  calling parts are consistent; i.e. they have the same name,
 *  data type, size, etc.  
 *  If no tag handle is registed to migrate, any tag data attached to entity 
 *  or entity set which are entirely migrated through iMeshP_exchAll or 
 *  iMeshP_migrateEntity is lost/
 *
 *  \xrefitem comm "Communication" "Communication" None++
 ******************************************************************************/
void iMeshP_migrateTagArr(
    iMesh_Instance instance,
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition,
        /**< [in] The partition being set. */
    iBase_TagHandle* tag_handles,
        /**< [in] Tag handles to register for automatic migration along with entity/set migration */
    int tag_handles_size,
        /**< [in] Number of tag handles */
    int entity_type,
        /**< [in] Tag data is exchanged only for this entity type. */
    int entity_topo,
        /**< [in] Tag data is exchanged only for this entity topology. */
    int include_entity_set,
        /**< [in] Flag (0 or 1) to indicate whether to migrate tag attached to entity set
                  If 1, tag data is exchanged for entity set. 
                  If 0, tag data is not exchanged for entity set */
    int* err
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Un-register a tag handle to turn off automatic tag data migration
 *
 *  Given a tag handle, withdraw the tag handle from automatic tag data migration
 *
 *  \xrefitem comm "Communication" "Communication" None++
 ******************************************************************************/
void iMeshP_resetTag(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being set. */
    iBase_TagHandle tag_handle, 
        /**< [in] Tag handle to undo automatic migration along with entity/set migration */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Un-register tag handles to turn off automatic migration
 *
 *  Given an array of tag handles, withdraw the tag handles from automatic 
 *  tag data migration
 *
 *  \xrefitem comm "Communication" "Communication" None++
 ******************************************************************************/
void iMeshP_resetTagArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being set. */
    iBase_TagHandle* tag_handles, 
        /**< [in] Tag handle to undo automatic migration along with entity/set migration */
    int tag_handles_size,
        /**< [in] Number of tag handles */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_commtag */

/**\defgroup imeshp_ptn Global/Local Partitioning
 *\{
 */

// COMMENT: It's not clear whether iMeshP is the right place to have partitioning 
// function and its parameter control.
// Need to double check with Zoltan team for the location, scope and method of 
// partitioning and parameter control 

/***************************************************************************//** 
 * \brief Register Zoltan parameters for static/dynamic load balancing
 * 
 *  Register LB_Method, LB_Approach, ParMETIS_Method, imbalance tolerance
 *  and debug level for static and dynamic mesh partitioning.
 *  For iMeshP_GRAPH or iMeshP_PARMETIS, either LB_Approach or ParMETIS_Method 
 *  can be specified. If both are specified, ParMETIS_Method is taken.
 *  For iMeshP_RCB, iMeshP_RIB, and iMeshP_HYPERGRAPH, iMeshP_NONE can be given
 *  to ParMETIS_Method. For all LB_Method, if neither LB_Approach nor 
 *  ParMETIS_Method is specified, the error code iBase_INVALID_ARGUMENT is returned.
 *
 *  Imbalance tolerance is the amount of load imbalance the partitioning algorithm 
 *  should be acceptable. The load on each processor is computed as the sum of the 
 *  weights of objects it is assigned. The imbalance is then computed as the maximum
 *  load divided by the average load. An value for imbalance tol of 1.2 indicates
 *  that 20% imbalance is OK; that is, the maximum over the average shouldn’t exceed 1.2.
 *  
 *  There are five values pre-defined for debug_level
 *    0 - no debugging output
 *    1 - printing debugging output (timing information, more information bits)
 *    2 - printing debugging output, and local on-processor graph checking
 *    3 - printing debugging output, and full graph checking
 *    4 - printing debugging output, full graph checking, and creating kdd debugging files
 *
 *  If iMeshP_setZoltanParam is not called, the default partitioning parameters 
 *  are at implementors' distretion.
 *
 *  \xrefitem comm "Communication" "Communication" None++
 ******************************************************************************/
void iMeshP_setPartitionParam(
    iMesh_Instance instance,
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition,
        /**< [in] The partition being set. */
    const int LB_method,
        /**< [in] LB_Method parameter value set in Zoltan_Set_Param */
    const int LB_approach,
        /**< [in] LB_Approach parameter value set in Zoltan_Set_Param */
    const int ParMETIS_method,
        /**< [in] ParMETIS_Method parameter value set in Zoltan_Set_Param */
    const double imbalance_tolerance,
        /**< [in] Imbalance tolerance value set in Zoltan_Set_Param */
    const int debug_level,
        /**< [in] Debug level value (0,...,4) set in Zoltan_Set_Param */
    int* err
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//** 
 * \brief Synchronously perform global mesh partitioning
 * 
 *  Given a mesh intance and partition handle, perform global mesh 
 *  partitioning to distribute mesh
 *
 *  \xrefitem comm "Communication" "Communication" Collective. Blocking.
 ******************************************************************************/
void iMeshP_globPartition(
    iMesh_Instance instance,
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition,
        /**< [in] The partition being partitioned. */
    int* err
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//** 
 * \brief Asynchronously perform local mesh partitioning
 * 
 *  Given a mesh intance and partition handle, perform local mesh 
 *  partitioning to distribute mesh load within the local process. Local mesh 
 *  partitioning doesn't change the part boundary links between parts on 
 *  different processes. Only part boundary links between parts on the same 
 *  process change.
 *
 *  \xrefitem comm "Communication" "Communication" None
 ******************************************************************************/
void iMeshP_localPartition(
    iMesh_Instance instance,
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition,
        /**< [in] The partition being partitioned. */
    int* err
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

//
// Additional partitioner control functions: weight setting for partition node and edges
//    - partitioning node can be either p-set or partition object entity
//

/**\} */  /* end group imeshp_ptninfo */

/**\defgroup imeshp_nbor Neighboring process information
 *\{
 */

/***************************************************************************//**
* \brief Return the number of processes that neighbor a local process

 *
*  Given a mesh instance, partition handle, and an entity type,
*  return the number of processes in the partition that neighbor the local process
*  (i.e., that (1) have copies of entities of the given entity type owned by
*  the local process or (2) own entities of the given entity type that are
*  copied on the local process). All entity types are accounted if iBase_ALL_TYPES 
*  is given for entity_type.
*
*  \xrefitem comm "Communication" "Communication" None++.
******************************************************************************/
void iMeshP_getNumProcNbors(
    iMesh_Instance instance,
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition,
        /**< [in] The partition being queried. */
    int entity_type,
        /**< [in] Entity type of the copied entities may be iBase_ALL_TYPES. */
    int* num_proc_nbors,
        /**< [out] Number of processes neighboring the local process. */
    int* err
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
* \brief Return the process ranks that neighbor the local process.
*
*  Given a mesh instance, partition handle, and an entity type,
*  return the process ranks of processes that neighbor the local process.
*  (i.e., that (1) have copies of entities of the given entity type owned by
*  the local process or (2) own entities of the given entity type that are
*  copied on the local process). All entity types are accounted if iBase_ALL_TYPES 
*  is given for entity_type.
*
*  \xrefitem comm "Communication" "Communication" None++.
******************************************************************************/
void iMeshP_getProcNbors(
    iMesh_Instance instance,
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition,
        /**< [in] The partition being queried. */
    int entity_type,
        /**< [in] Entity type of the copie entities; may be iBase_ALL_TYPES. */
    int* num_proc_nbors,
        /**< [out] Number of processes neighboring the local process. */
    iMeshP_Part** nbor_proc_ranks,
        /**< [in,out] Array of proc ranks for process neighbors of the local process */
    int* nbor_proc_ranks_allocated,
        /**< [in,out] Allocated size of nbor_proc_ranks array. */
    int* nbor_proc_ranks_size,
        /**< [out] Occupied size of nbor_proc_ranks array. */
    int* err
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_nbor */


/**\defgroup imeshp_ptninfo Print mesh partitioning and size information
 *\{
 */
/***************************************************************************//**
* \brief Synchronously collect and display mesh information per part and process
*
*  The content is at implementor's discretion and it may include the following:
*  - global statistics with respect to the count (#entity set, #entity) per process.
*      E.g. mesh load imbalance ratio, avg/max/min #entity set, #entity
*  - global statistics with respect to the count (#entity set, #entity) per part
*  - local (per process) statistics
*  - #owning and non-owning entities by type per part and process
*  - #entity sets per part and process
* 
*  \xrefitem comm "Communication" "Communication" Collective.  Non-blocking.
******************************************************************************/
void iMeshP_printMeshInfo(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_ptninfo */

#ifdef __cplusplus
} /*  extern "C"  */
#endif

/** \} */ /* end group IMESHP_ext */

#endif /* !defined(_ITAPS_iMeshP_EXT) */

