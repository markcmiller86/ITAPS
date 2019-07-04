#ifndef _ITAPS_iMeshP
#define _ITAPS_iMeshP

#include "iMesh.h"
#include "iMeshP_protos.h"
#include <mpi.h>

/**
*  \page imeshp IMeshP:  ITAPS Parallel Mesh Interface 
*  \defgroup imeshp IMeshP:  ITAPS Parallel Mesh Interface 

iMeshP.h -- ITAPS Parallel Mesh Interface

Release 1.0; Updated April 2009

\section INTRO Overview
ITAPS' iMeshP interface provides functions to create, query, and modify
a parallel distribution of a mesh.
Our primary target for iMeshP is distributed memory applications, but the
interface is also compatible with shared memory or global address space
applications.  In addition, backward compatibility of the serial iMesh
interface is supported, with iMesh working as an application would expect
within a single process, in shared memory, and in global address space systems.

The iMeshP interface defines a model for partitioning the entities of a mesh
among distinct processes. It
describes the distribution of and relationships among entities
on different processes.
In the model, a partition is a high-level description of the distribution
of mesh entities.
A parallel communication abstraction is used to manage communication
among entities and processes in a partition. A partition assigns
entities to subsets called parts.  The partition maps each part to
a process such that each process may have zero, one, or many parts.

Parts are identified globally by unique part IDs and, within a process, by
opaque part handles.  To simplify the iMeshP interface, we allow part handles 
to be substituted for entity-set handles in all serial iMesh functions.  Thus,
operations such as adding entities to parts and querying the number of 
entities in a part can be achieved using the same interface as adding
entities to and querying entity sets.  Additional iMeshP functions provide
information about part boundaries and neighboring parts.

Each entity is owned by only one part; ownership imbues
the right to modify.  Entities on part
boundaries are shared. One part owns the entities while other
part(s) have copies of the entities. 
Copies of additional, non-boundary entities (called ghost entities) may
be requested by an application to enable efficient computation.
The iMeshP data model defines 
rules for the amount of information about copies that an implementation must
manage.  For example, an entity's owner must store information about all copies
of the entity, and a copy must store information about the entity's owner.

Functions to easily create and modify partitions, create ghost entities,
retrieve ghost and owner entity tag data, and determine an entity's ownership
status are defined in iMeshP. In addition, the iMeshP interface supports
parallel operations needed for efficient
computation, load balancing and mesh modification.  By necessity,
these operations involve parallel communication; both
synchronous and asynchronous parallel operations are supported. This
design enables operations such as updates of tag data in ghost entities during
computation, large- or small-scale entity migration for dynamic load
balancing or edge swapping, updates of vertex coordinates in
non-owned vertices for mesh smoothing, and coordination
in the creation of
new entities along part boundaries for mesh refinement.

Below, we more carefully define these concepts.  We also review
iMesh concepts that are relevant to the iMeshP interface.

\section ADM Abstract Data Model
-  The term "mesh" refers to an abstraction in the data model; 
   it does not imply a serial or parallel distribution.
-  The term "partition" refers to an assignment of a set of entities to 
   subsets; like a "mesh," it does not imply a serial or parallel 
   implementation.
-  An application may use one or more meshes.  
-  A partition can create subsets of entities from a mesh.
-  Meshes can be subdivided by one or more partitions.
-  Partitions contain parts.  Parts contain the subsets of entities in the
   partition.

\section PAR Parallelism
-  A "process" can be thought of as an MPI process. The
   number of processes can be considered to be the result of MPI_Comm_size.
   The rank of a process can be thought of as the result of MPI_Comm_rank.
   We will think in terms of processes rather than processors.  Initial
   implementations of the parallel interface will likely use MPI terminology
   directly; future implementations may accommodate other communication 
   paradigms and libraries.
-  Partitions have communicators associated with them.  These communicators
   can be thought of as MPI communicators.  
-  "Global" operations are operations performed with respect to a 
   partition's communicator.
-  "Local" operations are operations performed with respect to a part or
   a mesh instance within a process.
-  Part A "neighbors" Part B if Part A has copies of entities owned by Part B
   and/or if Part B has copies of entities owned by Part A.
                  
\section INT Interfaces
-  Each process has one or more "mesh instances."  A mesh instance can be
   thought of as a mesh database.  An implementation should support the 
   existence of more than one mesh instance per process (e.g., it should 
   always associate mesh data with a mesh instance).  However, we expect 
   applications would most often use only one mesh instance per process.
-  There is one root set per mesh instance.
-  Many iMeshP functions are analogous to functions in iMesh.  Examples
   include iMeshP_getNumOfType, iMeshP_getNumOfTopo, iMeshP_getEntities,
   and iMeshP_initEntIter, but there are many others.  These functions
   accept both an entity set handle and a part handle, to allow operations
   like returning all entities in a given part (represented by a part handle)
   with a given boundary condition (represented by an entity set).
   When the entity set is the root set, users may either pass the root set
   to these functions or pass the part handle as the entity set in the 
   analogous iMesh function.
-  Similarly, iMeshP file I/O closely aligns with iMesh file I/O.  The major
   change is the addition of a iMeshP_PartitionHandle argument to both
   iMeshP_loadAll and iMeshP_saveAll, enabling I/O from parallel processes
   and the initialization of the partition describing the data distribution.
-  Generation and management of global IDs for entities 
   is not included in the iMeshP interface.  It can 
   be provided as a service above the iMeshP interface.
   Uniqueness of global IDs is managed at the partition level.

\section PTN Using Partitions
-  A partition assigns entities from one mesh instance to parts.  
-  Each process may have one or more partition handles.
-  Entities in a mesh instance can be partitioned by one or more partitions.  
   Mesh instances know which partitions they contain.
-  Users build partitions by 
   -  (1) creating a partition handle on each process to be included in the 
          partition; 
   -  (2) adding parts to the partition handle within the process; 
   -  (3) populating the parts with entities; and 
   -  (4) calling iMeshP_syncPartitionAll to allow the 
          implementation to compute global data for the partition.
-  Implementations of iMeshP_loadAll will perform these steps for the
   application using the given partition handle.

\section PRT Using Parts
-  Each part is wholly contained within a process.  
-  A process may have zero, one or multiple parts.
-  Parts are uniquely identified globally by part IDs of type iMeshP_Part.
   Local parts can also be accessed by part handles that provide more
   direct access to a part.  
-  Functions accepting part handles operate correctly on only local 
   parts (parts on the calling process); they will return an error 
   for remote (off-process) parts.  
-  Functions that return entity information for a part, set or mesh 
   instance return the information for all entities (including copies and
   ghosts) in that part, set or mesh instance.  Applications can check 
   whether an entity is owned or a ghost using iMeshP_isEntOwner or
   iMeshP_getEntStatus.
-  For each entity that is copied onto remote parts, the owning part knows 
   both the remote part ID and remote entity handle of all copies.
-  All parts with copies of a boundary entity know the remote part ID 
   and remote entity handle of all copies of the entity.  
-  All parts with copies of any entity know the part ID and
   entity handle corresponding to the owner of the entity.
-  Many iMesh functions that accept an iBase_EntitySetHandle 
   are also useful in the context of a iMeshP_PartHandle.
   These functions are reinterpreted so that they can accept either an
   iBase_EntitySetHandle or an iMeshP_PartHandle.  
-  In particular, entities are added to and removed from local parts via
   the same functions that are used to manipulate entity sets.
   That is, given a mesh instance, an entity handle, and a part handle,
   the entity is added to or removed from the part via calls to 
   the following functions with the part handle passed as the entity set handle:
   - Add entity to part --> iMesh_addEntToSet
   - Remove entity from part --> iMesh_rmvEntFromSet
   - Add array of entities to part --> iMesh_addEntArrToSet
   - Remove array of entities from part --> iMesh_rmvEntArrFromSet
-  iMeshP does not provide the capability to move entire parts to new
   processes;  instead, the new process must create the part in its
   partition handle and then receive the entities to 
   populate the part.  In other words, parts can be added to only a local 
   partition handle.


\section CMM Communication
-  Each function description includes its communication requirements.  The
   options are described here:
   -  COMMUNICATION:  Collective -- the function must be called by all 
      processes in the partition's communicator.  (These functions have the
      suffix "All" to indicate collective communication is done.)
   -  COMMUNICATION:  Point-to-Point -- communication is used, but the 
      communication is from one process to only one other process.  The
      receiving process must issue an appropriate receive call to receive 
      the message.
   -  COMMUNICATION:  None -- the function does not use communication; only
      local operations are performed.
   -  COMMUNICATION:  None++ -- no communication is done; the values
      are precomputed by iMeshP_syncPartitionAll or iMeshP_syncMeshAll.
-  Non-blocking calls for off-processor mesh-modification return a request 
   that indicates whether or not the operation has completed.  The request
   is more than an MPI request; it encapsulates both the MPI information and
   the mesh operations that were requested.  If non-blocking calls are used,
   appropriate calls to iMeshP "wait" or "poll" functions must be used to
   handle and satisfy requests.
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
#define IMESHP_VERSION_MAJOR ITAPS_VERSION_MAJOR
#define IMESHP_VERSION_MINOR ITAPS_VERSION_MINOR
#define IMESHP_VERSION_PATCH ITAPS_VERSION_PATCH

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Maintain backward compatibility with old version symbol names
 ******************************************************************************/
#define IMESHP_MAJOR_VERSION IMESHP_VERSION_MAJOR
#define IMESHP_MINOR_VERSION IMESHP_VERSION_MINOR
#define IMESHP_PATCH_VERSION IMESHP_VERSION_PATCH

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Version Comparison
 *
 * Evaluates to true at CPP time if the version of iMesh currently being
 * compiled is greater than or equal to the version specified.
 ******************************************************************************/
#define IMESHP_VERSION_GE(Maj,Min,Pat) ITAPS_VERSION_GE(Maj,Min,Pat)

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Compose string represention of the iMesh version number
 ******************************************************************************/
#define IMESHP_VERSION_STRING ITAPS_VERSION_STRING_(iMeshP)

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Compose a symbol name derived from the current iMesh version number.
 ******************************************************************************/
#define IMESHP_VERSION_TAG ITAPS_VERSION_TAG_(iMeshP)

/***************************************************************************//**
 * \ingroup VersionNumbers
 * \brief Define iMesh_newMesh symbol such that it depends on version number.
 *
 * Note: We ran into problems with this as it influences or is influenced by
 * fortran name mangling and so breaks fortran compilation. So, this is
 * currently disabled.
 ******************************************************************************/
#define IMESHP_NEW_MESH_NAME__(A,B,C) A##_##B##_##C
#define IMESHP_NEW_MESH_NAME_(A,B,C) IMESHP_NEW_MESH_NAME__(A,B,C)
#define IMESHP_NEW_MESH_NAME(A) IMESHP_NEW_MESH_NAME_(A,IMESHP_VERSION_MAJOR,IMESHP_VERSION_MINOR)

#ifdef __cplusplus
extern "C" {
#endif

/**\defgroup imeshp_defs Type Definitions
 *\{
 */
/** Definition of Partition Handle type for iMeshP. */
typedef struct iMeshP_PartitionHandle_Private* iMeshP_PartitionHandle;

/** Definition of Request Handle type for iMeshP. */
typedef struct iMeshP_RequestHandle_Private* iMeshP_RequestHandle;

/** Since we allow overloading of iMesh functions' entity set handles
 *  with part handles, iMeshP_PartHandle must be defined the same as
 *  iBase_EntitySetHandle. */
typedef iBase_EntitySetHandle iMeshP_PartHandle;

/** Definition of Part ID type for iMeshP; these IDs
 *  are unique across processors. */
typedef unsigned iMeshP_Part;

/** Types for classifying entities within a part. */
enum iMeshP_EntStatus
{
  iMeshP_EntStatus_MIN = 0,
  iMeshP_INTERNAL=iMeshP_EntStatus_MIN, /**< An owned entity that is not on a part boundary. */
  iMeshP_BOUNDARY, /**< A shared entity on a part boundary. */
  iMeshP_GHOST,    /**< An entity copy that is not a shared boundary entity. */
  iMeshP_EntStatus_MAX = iMeshP_GHOST
};

/** Part ID number indicating information should be returned about all parts. */
#define iMeshP_ALL_PARTS -1

/**\} */  /* end group imeshp_defs */

/**\defgroup imeshp_partition Partition Functionality
 *\{
 */

/***************************************************************************//**
 * \brief Create a partition; return its handle.
 *
 *  Given a mesh instance and a communicator,
 *  return a partition handle for a new partition within the mesh instance
 *  that uses the communicator.  
 *  In the future, we may have different creation routines for different 
 *  communication systems; once the partition is created, the application 
 *  would not have to worry about the communication system again.
 *  For now, implementations are MPI based, so MPI communicators are provided.
 *  For serial use, the communicator may be MPI_COMM_SELF or communicator may
 *  be NULL.
 *
 *  \xrefitem comm "Communication" "Communication" Collective
 ******************************************************************************/
void iMeshP_createPartitionAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    MPI_Comm communicator, 
        /**< [in] Communicator to be used for parallel communication. */
    iMeshP_PartitionHandle* partition, 
        /**< [out] The newly created partition. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Destroy a partition. 
 *
 *  Given a partition handle, 
 *  destroy the partition associated with the handle.
 *  Note that the partition handle is not invalidated upon return.
 *
 *  \xrefitem comm "Communication" "Communication" Collective.
 ******************************************************************************/
void iMeshP_destroyPartitionAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle partition, 
        /**< [in] The partition to be destroyed. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return communicator associated with a partition.
 *
 *  Given a partition handle, return the communicator associated with
 *  it during its creation by iMeshP_createPartitionAll.
 *
 *  \xrefitem comm "Communication" "Communication" None
 ******************************************************************************/
void iMeshP_getPartitionComm(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    MPI_Comm* communicator, 
        /**< [out] Communicator associated with the partition. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the number of partitions associated with a mesh instance.
 *
 *  Given a mesh instance, return the number of partition handles
 *  associated with the mesh instance.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getNumPartitions(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    int* num_partitions, 
        /**< [out] Number of partitions associated with th mesh instance. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the partition handles associated with a mesh instance.
 *
 *  Given a mesh instance, return all partition handles
 *  associated with the mesh instance.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getPartitions(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle** partitions, 
        /**< [in,out] Array of partition handles associated with the mesh
        instance. */
    int* partitions_allocated, 
        /**< [in,out] Allocated size of partitions array. */
    int* partitions_size, 
        /**< [out] Occupied size of partitions array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_partition */

/**\defgroup imeshp_partnmod Partition Modification
 *\{
 */


/***************************************************************************//**
 * \brief Create a new part in a partition.
 *
 *  Given a partition handle, create a new part and add it to the
 *  partition on the process invoking the creation.  Return the part handle
 *  for the new part.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_createPart(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle partition, 
        /**< [in] The partition being updated. */
    iMeshP_PartHandle* part, 
        /**< [out] The newly created part. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Remove a part from a partition.
 *
 *  Given a partition handle and a part handle, remove the part
 *  from the partition and destroy the part.  Note that the part handle
 *  is not invalidated by this function.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_destroyPart(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle partition, 
        /**< [in] The partition being updated. */
    iMeshP_PartHandle part, 
        /**< [in] The part to be removed. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Update a partition after parts have been added.
 *
 *  This function gives the implementation an opportunity to locally store info
 *  about the partition so that queries on the partition can be 
 *  performed without synchronous communication. 
 *  This function must be called after all parts have been added to the
 *  partition and after changes to the partition (e.g., due to load balancing).
 *  Values that are precomputed by syncPartitionAll include:
 *  -  the total number of parts in a partition;
 *  -  the mapping between part IDs and processes; and
 *  -  updated remote entity handle information.
 *
 *  \xrefitem comm "Communication" "Communication" Collective.
 ******************************************************************************/
void iMeshP_syncPartitionAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle partition, 
        /**< [in] The partition being updated. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_partnmod */

/**\defgroup imeshp_part Part Functionality
 *\{
 */

/***************************************************************************//**
 * \brief Return the global number of parts in a partition.
 *
 *  Given a partition handle, return the total number of parts 
 *  in the partition across all processes in the partition's communicator.
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getNumGlobalParts(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    int* num_global_part, 
        /**< [out] Global number of parts in the partition. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the local number of parts in a partition.
 *
 *  Given a partition handle, return the number of local (on-process) parts 
 *  in the partition.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getNumLocalParts(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    int* num_local_part, 
        /**< [out] Local (on-process) number of parts in the partition. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the part handles of local parts in a partition.
 *
 *  Given a partition handle, return the 
 *  part handles for the local (on-process) parts in the partition.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getLocalParts(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_PartHandle** parts, 
        /**< [in,out] Array of part handles for local parts in the partition. */
    int* parts_allocated, 
        /**< [in,out] Allocated size of parts array. */
    int* parts_size, 
        /**< [out] Occupied size of parts array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the process rank of a given part.
 *
 *  Given a partition handle and a part ID, return the process rank 
 *  (with respect to the partition's communicator) of the 
 *  process that owns the part. The part may be local or remote.
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getRankOfPart(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_Part part_id, 
        /**< [in] Part ID for the part being queried. */
    int* rank, 
        /**< [out] Process rank of part_id. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the process ranks of given parts.
 *
 *  Given a partition handle and an array of part IDs, return the process ranks 
 *  (with respect to the partition's communicator) of the 
 *  process that owns each part. The parts may be local or remote.
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getRankOfPartArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_Part* part_ids, 
        /**< [in] Array of Part IDs for the parts being queried. */
    const int part_ids_size, 
        /**< [in] The number of Part IDs in part_ids. */
    int** ranks, 
        /**< [in,out] Array of ranks for the Part Ids in part_ids. */
    int* ranks_allocated, 
        /**< [in,out] Allocated size of ranks array. */
    int* ranks_size, 
        /**< [out] Occupied size of ranks array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the number of entities of a given type in a partition.
 *
 *  Given a partition handle and an entity set (possibly the root set), 
 *  return the global number of  entities of a 
 *  given entity type in the partition and set.  This function may require 
 *  communication and, thus, must be called by all processes in the partition's 
 *  communicator.
 *
 *  \xrefitem comm "Communication" "Communication" Collective.
 ******************************************************************************/
void iMeshP_getNumOfTypeAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntitySetHandle entity_set, 
        /**< [in] Entity set handle for the entity se being queried. */
    int entity_type, 
        /**< [in] Requested entity type may be iBase_ALL_TYPES. */
    int* num_type, 
        /**< [out] Number of entities of entity_type i the partition and entity
        set. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the number of entities of a given topology in a partition.
 *
 *  Given a partition handle and an entity set (possibly the root set), 
 *  return the global number of  entities of a 
 *  given entity topology in the partition and set.  This function may require 
 *  communication and, thus, must be called by all processes in the partition's 
 *  communicator.
 *
 *  \xrefitem comm "Communication" "Communication" Collective.
 ******************************************************************************/
void iMeshP_getNumOfTopoAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntitySetHandle entity_set, 
        /**< [in] Entity set handle for the entity se being queried; may be the
        root set. */
    int entity_topology, 
        /**< [in] Requested entity topology may be iMesh_ALL_TOPOLOGIES. */
    int* num_topo, 
        /**< [out] Number of entities with entity_topology i the partition and
        entity set. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Obtain a part ID from a part handle.
 *
 *  Given a partition handle and a local part handle, return the part ID.
 *  If the part handle is not a valid part handle for a local part,
 *  an error is returned.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getPartIdFromPartHandle(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    iMeshP_Part* part_id, 
        /**< [out] Part ID for part. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Obtain part IDs from part handles.
 *
 *  Given a partition handle and an array of local part handles, 
 *  return the part ID for each part handle.
 *  If any part handle is not a valid part handle for a local part,
 *  an error is returned.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getPartIdsFromPartHandlesArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle* parts, 
        /**< [in] Array of part handles for the parts being queried. */
    const int parts_size, 
        /**< [in] Number of part handles being queried. */
    iMeshP_Part** part_ids, 
        /**< [in,out] Array of part IDs associated with the parts. */
    int* part_ids_allocated, 
        /**< [in,out] Allocated size of part_ids array. */
    int* part_ids_size, 
        /**< [out] Occupied size of part_ids array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Obtain a part handle from a part ID.
 *
 *  Given a partition handle and a part ID, return the part handle 
 *  associated with the part
 *  if the part is local; otherwise, return an error code.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getPartHandleFromPartId(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_Part part_id, 
        /**< [in] Part ID for the part being queried. */
    iMeshP_PartHandle* part, 
        /**< [out] Part handle associated with part_id. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Obtain part handles from part IDs.
 *
 *  Given a partition handle and an array of local part IDs, 
 *  return the part handle for each part ID.
 *  If any part ID is not a valid part ID for a local part,
 *  an error is returned.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getPartHandlesFromPartsIdsArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_Part* part_ids, 
        /**< [in] Array of part IDs for the parts being queried. */
    const int part_ids_size, 
        /**< [in] Number of part IDs being queried. */
    iMeshP_PartHandle** parts, 
        /**< [in,out] Array of part handles associated with the part_ids. */
    int* parts_allocated, 
        /**< [in,out] Allocated size of parts array. */
    int* parts_size, 
        /**< [out] Occupied size of parts array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_part */

/**\defgroup imeshp_bound Part Boundaries
 *\{
 */

/***************************************************************************//**
 * \brief Return the number of parts that neighbor a given part.
 *
 *  Given a partition handle, a part handle, and an entity type, 
 *  return the number of parts in the partition that neighbor the given part
 *  (i.e., that (1) have copies of entities of the given entity type owned by 
 *  the given part or (2) own entities of the given entity type that are 
 *  copied on the given part).
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getNumPartNbors(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    int entity_type, 
        /**< [in] Entity type of the copied entities may be iBase_ALL_TYPES. */
    int* num_part_nbors, 
        /**< [out] Number of parts neighboring the given part. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the number of parts that neighbor given parts.
 *
 *  Given a partition handle, an array of part handles, and an entity type, 
 *  return the number of parts in the partition that neighbor each of the 
 *  given parts
 *  (i.e., that (1) have copies of entities of the given entity type owned by 
 *  the given part or (2) own entities of the given entity type that are 
 *  copied on the given part).
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getNumPartNborsArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle* parts, 
        /**< [in] Array of part handles for the parts being queried. */
    int parts_size, 
        /**< [in] Number of part handles in parts. */
    int entity_type, 
        /**< [in] Entity type of the copie entities may be iBase_ALL_TYPES. */
    int** num_part_nbors, 
        /**< [in,out] Array of values specifying the number of part neighbors
        for each part in parts. */
    int* num_part_nbors_allocated, 
        /**< [in,out] Allocated size of num_part_nbors array. */
    int* num_part_nbors_size, 
        /**< [out] Occupied size of num_part_nbors array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the parts that neighbor a given part.
 *
 *  Given a partition handle, a part handle, and an entity type, 
 *  return the part IDs of parts that neighbor the given part
 *  (i.e., that (1) have copies of entities of the given entity type owned by 
 *  the given part or (2) own entities of the given entity type that are 
 *  copied on the given part).
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getPartNbors(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    int entity_type, 
        /**< [in] Entity type of the copie entities; may be iBase_ALL_TYPES. */
    int* num_part_nbors, 
        /**< [out] Number of parts neighborin the given part. */
    iMeshP_Part** nbor_part_ids, 
        /**< [in,out] Array of part IDs for part neighbors of part. */
    int* nbor_part_ids_allocated, 
        /**< [in,out] Allocated size of nbor_part_ids array. */
    int* nbor_part_ids_size, 
        /**< [out] Occupied size of nbor_part_ids array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the parts that neighbor given parts.
 *
 *  Given a partition handle, an array of part handles, and an entity type, 
 *  return the part IDs of parts that neighbor the given parts
 *  (i.e., that (1) have copies of entities of the given entity type owned by 
 *  the given part or (2) own entities of the given entity type that are 
 *  copied on the given part).
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getPartNborsArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle* parts, 
        /**< [in] The parts being queried. */
    const int parts_size, 
        /**< [in] The number of parts being queried. */
    int entity_type, 
        /**< [in] Entity type of the copied entities may be iBase_ALL_TYPES. */
    int** num_part_nbors, 
        /**< [in,out] Array of values specifying the number of part neighbors
        for each part in parts. */
    int* num_part_nbors_allocated, 
        /**< [in,out] Allocated size of num_part_nbors array. */
    int* num_part_nbors_size, 
        /**< [out] Occupied size of num_part_nbors array. */
    iMeshP_Part** nbor_part_ids, 
        /**< [in,out] Array of part IDs for part neighbors of part. */
    int* nbor_part_ids_allocated, 
        /**< [in,out] Allocated size of nbor_part_ids array. */
    int* nbor_part_ids_size, 
        /**< [out] Occupied size of nbor_part_ids array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the number of entities on a part boundary.
 *
 *  Given a partition handle, a part handle, an entity type and topology, and a
 *  target part ID, return the number of entities of the given type and/or
 *  topology on the part boundary shared with the target part.  
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getNumPartBdryEnts(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    int entity_type, 
        /**< [in] Entity type of the boundary entities may be iBase_ALL_TYPES. */
    int entity_topology, 
        /**< [in] Entity topology of the boundary entities; may be
        iMesh_ALL_TOPOLOGIES. */
    iMeshP_Part target_part_id, 
        /**< [in] Part ID with which part is sharin the boundary entities; may
        be iMeshP_ALL_PARTS. */
    int* num_entities, 
        /**< [out] Number of part boundary entities share by part and
        target_part_id. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the entity handles of entities on a part boundary.
 *
 *  Given a partition handle, a part handle, an entity type and topology, and a
 *  target part ID, return the entity handles of entities of the given type 
 *  and/or topology on the part boundary shared with the target part.  
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getPartBdryEnts(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    int entity_type, 
        /**< [in] Entity type of the boundary entities may be iBase_ALL_TYPES. */
    int entity_topology, 
        /**< [in] Entity topology of the boundary entities may be
        iMesh_ALL_TOPOLOGIES. */
    iMeshP_Part target_part_id, 
        /**< [in] Part ID with which part is sharing the boundary entities may
        be iMeshP_ALL_PARTS. */
    iBase_EntityHandle** entities, 
        /**< [in,out] Array of entity handles for entities on the part boundar
        between part and target_part_id. */
    int* entities_allocated, 
        /**< [in,out] Allocated size of entities array. */
    int* entities_size, 
        /**< [out] Occupied size of entities array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Initialize an iterator over a specified part boundary.
 *
 *  Given a partition handle, a part handle, and a 
 *  target part ID, return an iterator over all entities of a given
 *  entity type and topology along
 *  the part boundary shared with the target part.  
 *  Iterator functionality for getNext, reset, and end is 
 *  provided through the regular iMesh iterator functions
 *  iMesh_getNextEntIter, iMesh_resetEntIter, and iMesh_endEntIter,
 *  respectively.  
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_initPartBdryEntIter(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    int entity_type, 
        /**< [in] Entity type of the boundary entities may be iBase_ALL_TYPES. */
    int entity_topology, 
        /**< [in] Entity topology of the boundary entities; may be
        iMesh_ALL_TOPOLOGIES. */
    iMeshP_Part target_part_id, 
        /**< [in] Part ID with which part is sharin the boundary entities; may
        be iMeshP_ALL_PARTS. */
    iBase_EntityIterator* entity_iterator, 
        /**< [out] Iterator returned by the function. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Initialize an array iterator over a specified part boundary.
 *
 *  Given a partition handle, a part handle, and a 
 *  target part ID, return an array iterator over all entities of a given
 *  entity type and topology along
 *  the part boundary shared with the target part.  
 *  Iterator functionality for getNext, reset, and end is 
 *  provided through the regular iMesh iterator functions
 *  iMesh_getNextEntArrIter, iMesh_resetEntArrIter, and iMesh_endEntArrIter,
 *  respectively.  
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_initPartBdryEntArrIter(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    int entity_type, 
        /**< [in] Entity type of the boundary entities may be iBase_ALL_TYPES. */
    int entity_topology, 
        /**< [in] Entity topology of the boundary entities; may be
        iMesh_ALL_TOPOLOGIES. */
    int array_size, 
        /**< [in] Size of chunks of handles returned for each value of the
        iterator. */
    iMeshP_Part target_part_id, 
        /**< [in] Part ID with which part is sharin the boundary entities; may
        be iMeshP_ALL_PARTS. */
    iBase_EntityArrIterator* entity_iterator, 
        /**< [out] Iterator returned by the function. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_bound */

/**\defgroup imeshp_set Parts and Sets
 *\{
 */

/***************************************************************************//**
 * \brief Return the number of entities of a given type in both a part and an entity set.
 *
 *  Given a part handle, an entity set handle, and an entity type, return
 *  the number of entities of the given type that are in BOTH the given
 *  part AND the given entity set.
 *  This function is similar to iMesh_getNumOfType, but it also restricts
 *  the returned data with respect to its existence in the given part.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getNumOfType(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    const iBase_EntitySetHandle entity_set, 
        /**< [in] Entity set handle for the entity set being queried; may be
        the root set. */
    int entity_type, 
        /**< [in] Entity type of the boundary entities may be iBase_ALL_TYPES. */
    int* num_type, 
        /**< [out] Number of entities of entity_type i both part and entity_set. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the number of entities of a given topology in both a part and an entity set.
 *
 *  Given a part handle, an entity set handle, and an entity topology, return
 *  the number of entities of the given topology that are in BOTH the given
 *  part AND the given entity set.
 *  This function is similar to iMesh_getNumOfTopo, but it also restricts
 *  the returned data with respect to its existence in the given part.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getNumOfTopo(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    const iBase_EntitySetHandle entity_set, 
        /**< [in] Entity set handle for the entity set being queried; may be
        the root set. */
    int entity_topology, 
        /**< [in] Entity topology of the boundary entities may be
        iMesh_ALL_TOPOLOGIES. */
    int* num_topo, 
        /**< [out] Number of entities of entity_topology i both part and
        entity_set. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Get an indexed representation of a part's entitities or a subset of
 * a part's entities.
 *
 * Given part handle and an entity set and optionally a type or topology, 
 * for all entities that are in BOTH the part and the entity set, return:
 * - The entities in the part and set of the specified type or topology
 * - The entities adjacent to those entities with a specified
 *    type, as a list of unique handles.
 * - For each entity in the first list, the adjacent entities,
 *    specified as indices into the second list.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getAdjEntIndices(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    iBase_EntitySetHandle entity_set_handle, 
        /**< [in] The set being queried */
    int entity_type_requester, 
        /**< [in] If not iBase_ALL_TYPES, act only on the subset of entities
        wit the specified type. */
    int entity_topology_requester, 
        /**< [in] If not iMesh_ALL_TOPOLOGIES, act only on the subset of
        entities wit the specified topology. */
    int entity_type_requested, 
        /**< [in] The type of the adjacent entitie to return. */
    iBase_EntityHandle** entity_handles, 
        /**< [in,out] The handles of the (non-strict) subset of the union of
        the par and entity set, and the optional type and topology filtering
        arguments. */
    int* entity_handles_allocated, 
        /**< [in,out] Allocated size of entity_handles array. */
    int* entity_handles_size, 
        /**< Occupied size of entity_handles array. */
    iBase_EntityHandle** adj_entity_handles, 
        /**< [in,out] The union of the entities of type 'requested_entity_type'
        adjacent to each entity in 'entity_handles'. */
    int* adj_entity_handles_allocated, 
        /**< [in,out] Allocated size of adj_entity_handles array. */
    int* adj_entity_handles_size, 
        /**< [in,out] Occupied size of adj_entity_handles array. */
    int** adj_entity_indices, 
        /**< [in,out] For each entity in 'entity_handles', the adjacent
        entities of typ 'entity_type_requested', specified a indices into
        'adj_entity_handles'. The indices are concatenated into a single array
        in the order of the entity handles in 'entity_handles'. */
    int* adj_entity_indices_allocated, 
        /**< [in,out] Allocated size of adj_entity_indices array. */
    int* adj_entity_indices_size, 
        /**< [in,out] Occupied size of adj_entity_indices array. */
    int** offset, 
        /**< [in,out] For each entity in the corresponding position in
        'entity_handles', the position in 'adj_entity_indices' at which
        values for that entity are stored. */
    int* offset_allocated, 
        /**< [in,out] Allocated size of offset array. */
    int* offset_size, 
        /**< [in,out] Occupied size of offset array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return entities in a both given part and entity set.
 *
 *  Given an entity set handle 
 *  and a part handle, return entity handles for entities
 *  that are in both the part and the entity set.
 *  This function is similar to iMesh_getEntities, but it also restricts
 *  the returned data with respect to its existence in the given part.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getEntities(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    const iBase_EntitySetHandle entity_set, 
        /**< [in] Entity set handle for the entity set being queried; may be
        the root set. */
    int entity_type, 
        /**< [in] Entity type of th entities may be iBase_ALL_TYPES. */
    int entity_topology, 
        /**< [in] Entity topology of the entities may be iMesh_ALL_TOPOLOGIES. */
    iBase_EntityHandle** entities, 
        /**< [in,out] Array of entity handles fo entities in both part and
        entity_set. */
    int* entities_allocated, 
        /**< [in,out] Allocated size of entities. */
    int* entities_size, 
        /**< [out] Occupied size of entities. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Create an entity iterator for a given part and entity set.  
 *
 *  Given a local part and an entity set, return an iterator over the
 *  entities of the requested type and topology that are in both the
 *  part and the entity set.
 *  Iterator functionality for getNext, reset, and end is 
 *  provided through the regular iMesh iterator functions
 *  iMesh_getNextEntIter, iMesh_resetEntIter, and iMesh_endEntIter,
 *  respectively.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_initEntIter(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    const iBase_EntitySetHandle entity_set, 
        /**< [in] Entity set handle for the entity set being queried. */
    const int requested_entity_type, 
        /**< [in] Type of entities to include i the iterator. */
    const int requested_entity_topology, 
        /**< [in] Topology of entities to includ in the iterator. */
    iBase_EntityIterator* entity_iterator, 
        /**< [out] Iterator returned from function. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Create an entity array iterator for a given part and entity set.
 *
 *  Given a local part and an entity set, return an array iterator over the
 *  entities of the requested type and topology that are in both the
 *  part and the entity set.  
 *  Iterator functionality for getNext, reset, and end is 
 *  provided through the regular iMesh iterator functions
 *  iMesh_getNextEntArrIter, iMesh_resetEntArrIter, and iMesh_endEntArrIter,
 *  respectively.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_initEntArrIter(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    const iBase_EntitySetHandle entity_set, 
        /**< [in] Entity set handle for the entity set being queried. */
    const int requested_entity_type, 
        /**< [in] Type of entities to include i the iterator. */
    const int requested_entity_topology, 
        /**< [in] Topology of entities to includ in the iterator. */
    const int requested_array_size, 
        /**< [in] The number of handles returned in each value of the iterator. */
    iBase_EntityArrIterator* entArr_iterator, 
        /**< [out] Iterator returned from function. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_sets */

/**\defgroup imeshp_entity Entity Functionality
 *\{
 */


/***************************************************************************//**
 * \brief Return the part ID of the part owning an entity.
 *
 *  Given an entity handle and a partition handle, return the part ID 
 *  of the part that owns the entity.
 *  Return an error code if an entity is not in the partition.
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getEntOwnerPart(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntityHandle entity, 
        /**< [in] Entity whose owning part is to b returned. */
    iMeshP_Part* part_id, 
        /**< [out] Part ID of the part ownin the entity. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the part IDs of the parts owning the given entities.
 *
 *  Given an array of entity handles and a partition handle, return for each
 *  entity handle the part ID of the part that owns the entity.
 *  Return an error code if an entity is not in the partition.
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getEntOwnerPartArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntityHandle* entities, 
        /**< [in] Entity whose owning part is to b returned. */
    const int entities_size, 
        /**< [in] Number of entities in entities array. */
    iMeshP_Part** part_ids, 
        /**< [out] Part IDs of the parts ownin the entities. */
    int* part_ids_allocated, 
        /**< [in,out] Allocated size of part_ids array. */
    int* part_ids_size, 
        /**< [out] Occupied size of part_ids array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Test for entity ownership with respect to a part.
 *
 *  Given a partition handle, a part handle, and an entity handle, return a
 *  flag indicating whether the entity is owned by the part.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_isEntOwner(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    const iBase_EntityHandle entity, 
        /**< [in] Entity whose ownership is being tested. */
    int* is_owner, 
        /**< [out] Flag indicating whether the given part is the owner of the
        given entity. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Test for entity ownership of many entities with respect to a part.
 *
 *  Given a partition handle, a part handle, and an array of entity handles, 
 *  return for each entity handle a flag indicating whether the entity 
 *  is owned by the part.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_isEntOwnerArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    const iBase_EntityHandle* entities, 
        /**< [in] Entities whose ownership is being tested. */
    const int entities_size, 
        /**< [in] Number of entity handles i entities. */
    int** is_owner, 
        /**< [out] Flag for each entity indicating whether the given part is
        the owner of the given entity. */
    int* is_owner_allocated, 
        /**< [in,out] Allocated size of is_owner array. */
    int* is_owner_size, 
        /**< [out] Occupied size of is_owner array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return entity status (Internal, boundary, ghost).
 *
 *  Given a partition handle, a part handle, and an entity handle, return a
 *  flag indicating whether the entity is strictly internal, is on a 
 *  part boundary, or is a ghost with respect to the given part.  
 *  The returned value is a member of the iMeshP_EntStatus enumerated type.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getEntStatus(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    const iBase_EntityHandle entity, 
        /**< [in] Entity whose status is being tested. */
    int* par_status, 
        /**< [out] Value indicating the status of th is the entity with respect
        to the part. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return entity status (Internal, boundary, ghost).
 *
 *  Given a partition handle, a part handle, and an array of entity handles, 
 *  return for each entity handle a flag indicating whether the entity is 
 *  strictly internal, is on a part boundary, or is a ghost with respect 
 *  to the given part.  
 *  The returned value is a member of the iMeshP_EntStatus enumerated type.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_getEntStatusArr(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_PartHandle part, 
        /**< [in] The part being queried. */
    const iBase_EntityHandle* entities, 
        /**< [in] Entities whose status is being tested. */
    const int entities_size, 
        /**< [in] Number of entity handles i entities. */
    int** par_status, 
        /**< [out] Value for each entity indicating the status of the entity
        with respect to the part. */
    int* par_status_allocated, 
        /**< [in,out] Allocated size of par_status array. */
    int* par_status_size, 
        /**< [out] Occupied size of par_status array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the number of copies of an entity that exist in the partition.
 *
 *  Given a partition handle and an entity handle, return the number 
 *  of copies of the entity in the partition.  
 *  If the given entity is an owned entity or boundary entity, 
 *  the number of copies will be complete.
 *  If the given entity is a ghost entity, the number of copies will be two
 *  (the ghost and its owner).
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getNumCopies(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntityHandle entity, 
        /**< [in] Entity whose copy info is requested. */
    int* num_copies_ent, 
        /**< [out] Number of copies of the entity that exist in the partition. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return the part IDs of parts having copies of a given entity.
 *
 *  Given a partition handle and an entity handle, return the part IDs
 *  of copies of the entity in the partition. 
 *  If the given entity is an owned entity or boundary entity, 
 *  the number of copies considered will be complete.
 *  If the given entity is a ghost entity, the number of copies considered
 *  will be two (the ghost and its owner).
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getCopyParts(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntityHandle entity, 
        /**< [in] Entity whose copy inf is requested. */
    iMeshP_Part** part_ids, 
        /**< [out] Part IDs of parts having copie of the given entity. */
    int* part_ids_allocated, 
        /**< [in,out] Allocated size of part_ids array. */
    int* part_ids_size, 
        /**< [out] Occupied size of part_ids array. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Get (remote) entity handles of copies of a given entity.
 *
 *  Given a partition handle and an entity handle, return (remote) entity
 *  handles and part IDs of all copies of the entity.
 *  If the given entity is an owned entity or boundary entity, 
 *  the number of copies considered will be complete.
 *  If the given entity is a ghost entity, the number of copies considered
 *  will be two (the ghost and its owner).
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getCopies(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntityHandle entity, 
        /**< [in] Entity whose copy inf is requested. */
    iMeshP_Part** part_ids, 
        /**< [out] Part IDs of parts having copie of the given entity. */
    int* part_ids_allocated, 
        /**< [in,out] Allocated size of part_ids array. */
    int* part_ids_size, 
        /**< [out] Occupied size of part_ids array. */
    iBase_EntityHandle** copies, 
        /**< [out] (Remote) entity handles of the entity copies. */
    int* copies_allocated, 
        /**< [in,out] Allocated size of copies. */
    int* copies_size, 
        /**< [out] Occupied size of copies. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Get the entity handle of a copy of a given entity in a given part.
 *
 *  Given a partition handle, an entity handle and a part ID, 
 *  return the (remote) entity handle of the copy of the entity in that part.
 *  Return an error if the entity does not exist in the specified part.
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getCopyOnPart(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntityHandle entity, 
        /**< [in] Entity whose copy inf is requested. */
    const iMeshP_Part part_id, 
        /**< [in] Part ID of part whose copy of the given entity is requested. */
    iBase_EntityHandle* copy_entity, 
        /**< [out] (Remote) entity handle of the entity copy from the given part. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Get the entity handle of a copy of a given entity in its owner part.
 *
 *  Given a partition handle and an entity handle, return the (remote) 
 *  entity handle of the copy of the entity in its owner part.
 *
 *  \xrefitem comm "Communication" "Communication" None++.
 ******************************************************************************/
void iMeshP_getOwnerCopy(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntityHandle entity, 
        /**< [in] Entity whose copy inf is requested. */
    iMeshP_Part* owner_part_id, 
        /**< [out] Part ID of the entity's owner part. */
    iBase_EntityHandle* owner_entity, 
        /**< [out] (Remote) entity handle of the entity copy from the owner part. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_entity */

/**\defgroup imeshp_comm Communication
 *\{
 */

/**\defgroup imeshp_commgen General Communication
 *\{
 */


/***************************************************************************//**
 * \brief Wait for a specific iMeshP request to complete.
 *
 *  Given an iMeshP_RequestHandle, wait for the request to complete.
 *
 *  \xrefitem comm "Communication" "Communication" Blocking point-to-point.
 ******************************************************************************/
void iMeshP_waitForRequest(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_RequestHandle request, 
        /**< [in] iMeshP request for whose completio we should wait. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Wait for any of the specified iMeshP requests to complete.
 *
 *  Given an array of iMeshP_RequestHandles, wait for any one of the requests 
 *  to complete.
 *
 *  \xrefitem comm "Communication" "Communication" Blocking point-to-point.
 ******************************************************************************/
void iMeshP_waitForAnyRequest(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_RequestHandle* requests, 
        /**< [in] iMeshP requests for which we wai until one request completes. */
    int requests_size, 
        /**< [in] Number of requests in requests. */
    int* index, 
        /**< [out] Index of the request that completed. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Wait for all of the specified iMeshP requests to complete.
 *
 *  Given an array of iMeshP_RequestHandles, wait for all of the requests 
 *  to complete.
 *
 *  \xrefitem comm "Communication" "Communication" Blocking point-to-point.
 ******************************************************************************/
void iMeshP_waitForAllRequests(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_RequestHandle* requests, 
        /**< [in] iMeshP requests for which we wai until completion. */
    int requests_size, 
        /**< [in] Number of requests in requests. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Wait for a specific request to complete; return entities received.
 *
 *  Given an iMeshP_RequestHandle, wait for the request to complete.  Return
 *  entities for which information was received.
 *
 *  \xrefitem comm "Communication" "Communication" Blocking point-to-point.
 ******************************************************************************/
void iMeshP_waitForRequestEnt(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_RequestHandle request, 
        /**< [in] iMeshP request for whose completio we should wait. */
    iBase_EntityHandle** out_entities, 
        /**< [out] Entities for which information wa received. */
    int* out_entities_allocated, 
        /**< [in,out] Allocated size of out_entities. */
    int* out_entities_size, 
        /**< [out] Occupied size of out_entities. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Test whether a specific request has completed.
 *
 *  Given an iMeshP_RequestHandle, test whether the request has completed.
 *  This function will not wait until the request completes; it will only
 *  return the completion status (complete = 1 or 0).
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_testRequest(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_RequestHandle request, 
        /**< [in] iMeshP request for whose completio we should test. */
    int* completed, 
        /**< [out] Flag indicating whether (1) or not (0) the given request has
        completed. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Poll for outstanding requests.  
 *
 *  Check for outstanding requests from other parts, handle any requests 
 *  found, and return an array of requests that have been handled.  If
 *  the array has a size allocated already, then the implementation stops
 *  working when it has generated that many completed requests, even if there
 *  are more requests waiting. 
 *
 *  \xrefitem comm "Communication" "Communication" non-blocking; point-to-point.
 ******************************************************************************/
void iMeshP_pollForRequests(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_RequestHandle** requests_completed, 
        /**< [out] Requests that were completed. */
    int* requests_completed_allocated, 
        /**< [in,out] Allocated size o requests_completed. */
    int* requests_completed_size, 
        /**< [out] Occupied size o requests_completed. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_commgen */

/**\defgroup imeshp_remotemod Requests for off-processor mesh modification
 *\{
 */

/***************************************************************************//**
 * \brief Add entities to on-process and/or off-process parts.
 *
 *  Given a partition and a list of entities, add those entities to the
 *  target parts.  The entities can be added as copies or migrated entirely
 *  (i.e., change ownership of the entities)
 *  to the parts.  The entities' downward adjacencies are also copied and/or
 *  migrated as appropriate to support the entities.
 *  This function is a collective, non-blocking operation
 *  to be called by all processes in the partition's communicator.  
 *  An iMeshP_RequestHandle is returned; any of the 
 *  iMeshP_wait* functions can be used to block until the request is completed.
 *
 *  \xrefitem comm "Communication" "Communication" Collective.  Non-blocking.
 ******************************************************************************/
void iMeshP_exchEntArrToPartsAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] Handle for the partition being queried. */
    const iBase_EntityHandle* entities, 
        /**< [in] Entities to be sent. */
    const int entities_size, 
        /**< [in] Number of entities to be sent. */
    const iMeshP_Part* target_part_ids, 
        /**< [in] Array of size entities_size listin the parts to which the
        entities shoul be sent. */
    int command_code, 
        /**< [in] Flag indicating whether to migrat the entities or only make
        copies. */
    int update_ghost, 
        /**< [in] Flag indicating whether (1) or not (0 ghost copies of the
        entities should b updated with new owner information. */
    iMeshP_RequestHandle* request, 
        /**< [out] iMeshP RequestHandle returned; can be used for blocking
        until this send is complete. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Request in-migration of an entity and its upward adjacencies.
 *
 *  This function is a "pull" migration, where a part requests to become the
 *  owner of an entity that is owned by another part (so that the part has
 *  the right to modify the entity).  The requested
 *  entity must be on the part boundary and is identified by a local handle
 *  (i.e., an entity part-boundary copy).   This operation may require multiple
 *  rounds of communication, and at some times, certain entities may be
 *  locked (unavailable for local modification) while info about their
 *  remote copies is still in question.  Tags and parallel set membership 
 *  are migrated as well as the appropriate adjacency info.
 *  An iMeshP request handle is returned.
 *
 *  \xrefitem comm "Communication" "Communication" point-to-point, non-blocking,
 *  pull. 
 ******************************************************************************/
void iMeshP_migrateEntity(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iMeshP_PartHandle part, 
        /**< [in] The part to which the entity is migrated. */
    iBase_EntityHandle local_entity, 
        /**< [in] The local entity copy for the entity to b migrated. */
    iMeshP_RequestHandle* request, 
        /**< [out] The iMeshP request handle returned. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Update vertex coordinates for vertex copies.  
 *
 *  For a given vertex, update its copies with the vertex's coordinates.
 *  This function assumes that a local vertex's coordinates were updated
 *  through a call to iMesh_setVtxCoords.  This function then updates all
 *  copies of the vertex with the updated coordinates.
 *  The communication here is push-and-forget; as such, 
 *  no request handle needs to be returned.
 *
 *  \xrefitem comm "Communication" "Communication" point-to-point, non-blocking,
 *  push-and-forget.
 ******************************************************************************/
void iMeshP_updateVtxCoords(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntityHandle local_vertex, 
        /**< [in] The vertex whose copies should be updated. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Replace entities on the part boundary.
 *
 *  This function performs changes on the part boundary where the
 *  calling application can ensure that things are done
 *  identically on both sides and that the arguments are passed in an order 
 *  that can be matched.  (Specifically, matching new entities should appear in
 *  the same order in the call array.)  An example is creation of new 
 *  boundary edges during edge splitting.
 *  Communication here could be a
 *  two-way push-and-forget, or some variant on push-and-confirm.
 *  CHANGES: At Onkar's suggestion, added an offset array (similar to array
 *  adjacency requests) so that a single call can easily handle coordination
 *  with multiple entities on part-boundary.
 *
 *  \xrefitem comm "Communication" "Communication" point-to-point, non-blocking,
 *  push-and-forget. 
 ******************************************************************************/
void iMeshP_replaceOnPartBdry(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iBase_EntityHandle* old_entities, 
        /**< [in] The entities to be replaced. */
    const int old_entities_size, 
        /**< [in] The number of entities to be replaced. */
    const iBase_EntityHandle* new_entities, 
        /**< [in] The entities that replace the old entities. */
    const int new_entities_size, 
        /**< [in] The number of entities in new_entities. */
    const int* offset, 
        /**< [in] Index into new_entities; old_entities[i is replaced by
        new_entities[offset[i]] t new_entities[offset[i+1]-1]. */
    const int offset_size, 
        /**< [in] The number of entries in offset. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Push ghost copies of individual entities onto other parts.
 *
 *  Given an entity and a target part, create a ghost copy of the entity on
 *  the target part.
 *  Communication here is push-and-confirm (so that the original knows remote
 *  entity handle of the created ghosts).  The closure of a new ghost is pushed
 *  automatically as part of the underlying communication.
 *
 *  \xrefitem comm "Communication" "Communication" point-to-point, non-blocking,
 *  push.
 ******************************************************************************/
void iMeshP_addGhostOf(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_Part target_part_id, 
        /**< [in] The part to receive the new ghost. */
    iBase_EntityHandle entity_to_copy, 
        /**< [in] The entity to be copied in target_part_id. */
    iMeshP_RequestHandle* request, 
        /**< [out] The iMeshP request handle returned. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Remove ghost copies of individual entities from other parts.
 *
 *  Given an entity and a target part, remove the ghost copy of the entity on
 *  the target part.
 *  Communication is push-and-forget; as such, no request handle is needed.
 *  The remote part will clean up the closure of the removed ghost
 *  as appropriate during deletion.
 *
 *  \xrefitem comm "Communication" "Communication" point-to-point, non-blocking,
 *  push-and-forget.
 ******************************************************************************/
void iMeshP_rmvGhostOf(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    const iMeshP_Part target_part_id, 
        /**< [in] The part to lose the ghost. */
    iBase_EntityHandle copy_to_purge, 
        /**< [in] The entity whose ghost is removed from target_part_id. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Indicate completion of mesh modification.
 *
 *  Calling this function indicates that the user is finished with mesh 
 *  modification for now.  With mesh modification complete, the implementation
 *  can update ghost, partition, boundary, and other information to 
 *  re-establish a valid distributed mesh.  This function waits for all
 *  message traffic to clear and rebuilds ghost information that was
 *  allowed to go obsolete during mesh modification.
 *
 *  \xrefitem comm "Communication" "Communication" collective.
 ******************************************************************************/
void iMeshP_syncMeshAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_remotemod */

/**\defgroup imeshp_commtag Functions to send Tag data from owning entities to copies.
 *\{
 */

/***************************************************************************//**
 * \brief Synchronously send tag data for given entity types and topologies.
 *
 *  Send tag information for shared entities of specified type and
 *  topology.  The tag data is "pushed" from the owner entities to all copies.
 *  This version operates on all shared entities of specified type and topology
 *  (or all types/topologies if iBase_ALL_TYPES/iMesh_ALL_TOPOLOGIES are
 *  given).  This function assumes tag handles given on various
 *  calling parts are consistent; i.e. they have the same name,
 *  data type, size, etc.  This call blocks until communication is
 *  completed.
 *
 *  \xrefitem comm "Communication" "Communication" point-to-point, blocking.
 ******************************************************************************/
void iMeshP_pushTags(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iBase_TagHandle source_tag, 
        /**< [in] Tag handle for the sending entities. */
    iBase_TagHandle dest_tag, 
        /**< [in] Tag handle for the receiving entities. */
    int entity_type, 
        /**< [in] Tag data is exchanged only for this entity type. */
    int entity_topo, 
        /**< [in] Tag data is exchanged only for this entity topology. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Synchronously send tag data for individual entities.
 *
 *  Send tag information for the specified entities.
 *  The tag data is "pushed" from the owner entities to all copies.
 *  This function assumes tag handles given on various
 *  calling parts are consistent; i.e. they have the same name,
 *  data type, size, etc.  This call blocks until communication is
 *  completed.
 *
 *  \xrefitem comm "Communication" "Communication" point-to-point, blocking.
 ******************************************************************************/
void iMeshP_pushTagsEnt(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iBase_TagHandle source_tag, 
        /**< [in] Tag handle for the sending entities. */
    iBase_TagHandle dest_tag, 
        /**< [in] Tag handle for the receiving entities. */
    const iBase_EntityHandle* entities, 
        /**< [in] Owned entities for which to send data. */
    int entities_size, 
        /**< [in] The number of entities for which to send data. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Asynchronously send tag data for given entity types and topologies.
 *
 *  Send tag information for shared entities of specified type and
 *  topology.  The tag data is "pushed" from the owner entities to all copies.
 *  This version operates on all shared entities of specified type and topology
 *  (or all types/topologies if iBase_ALL_TYPES/iMesh_ALL_TOPOLOGIES are
 *  given).  This function assumes tag handles given on various
 *  calling parts are consistent; i.e. they have the same name,
 *  data type, size, etc.  
 *  This call does not block; applications should call 
 *  iMeshP_waitForRequest (or a similar wait function)
 *  to block until this push is completed.
 *
 *  \xrefitem comm "Communication" "Communication" point-to-point, non-blocking.
 ******************************************************************************/
void iMeshP_iPushTags(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iBase_TagHandle source_tag, 
        /**< [in] Tag handle for the sending entities. */
    iBase_TagHandle dest_tag, 
        /**< [in] Tag handle for the receiving entities. */
    int entity_type, 
        /**< [in] Tag data is exchanged only for this entity type. */
    int entity_topo, 
        /**< [in] Tag data is exchanged only for this entity topology. */
    iMeshP_RequestHandle* request, 
        /**< [out] The iMeshP request handle returned. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Asynchronously send tag data for individual entities.
 *
 *  Send tag information for the specified entities.
 *  The tag data is "pushed" from the owner entities to all copies.
 *  This function assumes tag handles given on various
 *  calling parts are consistent; i.e. they have the same name,
 *  data type, size, etc.  
 *  This call does not block; applications should call 
 *  iMeshP_waitForRequest (or a similar wait function)
 *  to block until this push is completed.
 *
 *  \xrefitem comm "Communication" "Communication" point-to-point, non-blocking.
 ******************************************************************************/
void iMeshP_iPushTagsEnt(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being queried. */
    iBase_TagHandle source_tag, 
        /**< [in] Tag handle for the sending entities. */
    iBase_TagHandle dest_tag, 
        /**< [in] Tag handle for the receiving entities. */
    const iBase_EntityHandle* entities, 
        /**< [in] Owned entities for which to send data. */
    int entities_size, 
        /**< [in] The number of entities for which to send data. */
    iMeshP_RequestHandle* request, 
        /**< [out] The iMeshP request handle returned. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_commtag */

/**\defgroup imeshp_ghost Ghosting
 *\{
 */

/***************************************************************************//**
 * \brief Create ghost entities between parts.
 *
 *  Ghost entities are specified similar to 2nd-order adjacencies, i.e.,
 *  through a "bridge" dimension.  The number of layers is measured from
 *  the inter-part interfaces.  For example, to get two layers of region
 *  entities in the ghost layer, measured from faces on the interface,
 *  use ghost_dim=3, bridge_dim=2, and num_layers=2.
 *  The number of layers specified is with respect to the global mesh;
 *  that is, ghosting may extend beyond a single neighboring processor if the
 *  number of layers is high.
 *  Ghost information is cached in the partition.  
 *  The triplet describing a ghosting "rule" (ghost dim, bridge dim, #
 *  layers) is stored in the partition; ghosting that became incorrect
 *  due to mesh modification or redistribution of mesh entities is 
 *  re-established using these rules by the end
 *  of iMeshP_syncPartitionAll and iMeshP_syncMeshAll.  
 *  Implementations can choose to keep ghosting consistent throughout 
 *  mesh modification, but ghosts are not required to be consistent until 
 *  the end of these two functions.
 *  iMeshP_createGhostEntsAll is cumulative; that is, multiple calls can only
 *  add more ghosts, not eliminate previous ghosts.  
 *
 *  \xrefitem comm "Communication" "Communication" Collective.  Blocking.
 ******************************************************************************/
void iMeshP_createGhostEntsAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle partition, 
        /**< [in] The partition in which to create ghosts. */
    int ghost_type, 
        /**< [in] Entity type of entities to be ghosted. */
    int bridge_type, 
        /**< [in] Entity type through which bridge adjacencies are found. */
    int num_layers, 
        /**< [in] Number of layers of ghost entities. */
    int include_copies, 
        /**< [in] Flag indicating whether to create ghosts of non-owned part
        boundary entities (YES=1, NO=0). */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Delete all ghost entities between parts.
 *
 *  Given a partition, delete all ghost entities in that partition of the mesh.
 *
 *  \xrefitem comm "Communication" "Communication" Collective.  Blocking.
 ******************************************************************************/
void iMeshP_deleteGhostEntsAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    iMeshP_PartitionHandle partition, 
        /**< [in] The partition from which to delete ghosts. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/***************************************************************************//**
 * \brief Return information about all ghosting on a partition.
 *
 *  Return the ghosting rules established through calls to
 *  iMeshP_createGhostEntsAll.
 *
 *  \xrefitem comm "Communication" "Communication" None.
 ******************************************************************************/
void iMeshP_ghostEntInfo(
    const iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition to be queried. */
    int* ghost_rules_allocated, 
        /**< [in,out] Allocated size of ghost_type bridge_type and num_layers. */
    int* ghost_rules_size, 
        /**< [out] Occupied size of ghost_type, bridge_type and num_layers
        equal to the number of ghosting rules currently registered in the
        partition. */
    int** ghost_type, 
        /**< [out] Entity type of ghost entities for each rule. */
    int** bridge_type, 
        /**< [out] Entity type of bridge entities for each rule. */
    int** num_layers, 
        /**< [out] Number of layers of ghosts in each rule. */
    int* err  
        /**< [out] Returned Error status (see iBase_ErrorType) */
);

/**\} */  /* end group imeshp_ghost */

/**\} */  /* end group imeshp_comm */

/**\defgroup imeshp_file File I/O
 *\{
 */

/***************************************************************************//**
 * \brief Populate a mesh instance and a partition by reading data from files.
 *
 *  Before calling iMeshP_loadAll, the application creates both a mesh 
 *  instance and a partition handle.  iMeshP_loadAll then reads the
 *  specified file, inserts entities into the mesh instance, constructs
 *  parts within the partition, and inserts entities into the parts.
 *  Options allow n>=1 files on p processes.
 *  Optional capabilities of iMeshP_loadAll include computing an initial
 *  partition (e.g., if a serial mesh file without part assignments is read)
 *  and creating ghost entities as requested by the application; the
 *  availability of these options is implementation dependent.
 *
 *  \xrefitem comm "Communication" "Communication" Collective.
 ******************************************************************************/
void iMeshP_loadAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The newly populated partition. */
    const iBase_EntitySetHandle entity_set, 
        /**< [in] Set to which the mesh will be added. */
    const char* name, 
        /**< [in] File name from which mesh data is read. */
    const char* options, 
        /**< [in] Implementation-specific options string. */
    int* err, 
        /**< [out] Returned Error status (see iBase_ErrorType) */
    int name_len, 
        /**< [in] Length of the file name character string. */
    int options_len  
        /**< [in] Length of the options character string */
);

/***************************************************************************//**
 * \brief Write data from a mesh instance and a partition to files.
 *
 ******************************************************************************/
void iMeshP_saveAll(
    iMesh_Instance instance, 
        /**< [in] iMesh instance handle */
    const iMeshP_PartitionHandle partition, 
        /**< [in] The partition being saved. */
    const iBase_EntitySetHandle entity_set, 
        /**< [in] Set from which data will be saved. */
    const char* name, 
        /**< [in] File name to which mesh data is written. */
    const char* options, 
        /**< [in] Implementation-specific options string. */
    int* err, 
        /**< [out] Returned Error status (see iBase_ErrorType) */
    const int name_len, 
        /**< [in] Length of the file name character string. */
    int options_len  
        /**< [in] Length of the options character string. */
);

/**\} */  /* end group imeshp_file */

/*
------------------------------------------------
Major Items left to do:
-  Support for multiple partitions.
   We discussed designating a given partition as 
   the "active" partition; i.e., the partition that is actually used in 
   the distribution of mesh data in distributed memory.  We were concerned 
   that when multiple partitions were used, multiple copies of mesh 
   entities would be needed to fully support multiple partitions at the 
   same time.  Designating one partition as "active" would store data 
   with respect to only one partition.

-  File I/O support.
   Need common set of options to allow interoperability.
   Support single files, N << P files on P processes, and P files.
   Support reading and writing partition information.
   Support initial parallel partitioning of serial file data.
   Support storing mapping of parts to processes in files.

------------------------------------------------
Minor Items left to do:
-  Determine which capabilities need both "getNumX" and "getX" functions.
   That is, when would an application need "getNumX" to allocate memory
   for "getX" or separately from "getX".  When could we use only "getX"
   and return numX as a byproduct.

-  Determine with functions need "Ent" and "EntArr" versions, or whether
   we should adopt only the more general "EntArr" version.

-  Determine whether to revise iMeshP_createPartition to make it less MPI 
   specific.  We don't want to require applications to link with MPI if the 
   implementation doesn't require it.  We may define an ITAPS_Comm name 
   typedef'ed appropriately.

-  iMeshP_getOwnerCopy could be achieved by calling iMeshP_getOwnerPart
   followed by iMeshP_getCopyOnPart.  Do we want to keep iMeshP_getOwnerCopy?

-  We could enhance the functions to update tag data from shared 
   entities in owner to perform mathematical operations (similar to MPI_SUM).
   From Carl:  "getTag*Operate: Again, we haven't got this in serial.  Does 
   the existence of such operations imply that we expect to implement 
   fields as tags? (Because that wasn't what I was assuming about field 
   implementations at all, personally...)  Note that I'm not opposed to 
   this sort of global reduction operation, I just wonder whether it'll see 
   use outside of field-like situations.  If not, then it should be in 
   parallel fields, not parallel mesh, and usage for 
   fields-implemented-as-tags should be handled there."

------------------------------------------------
Comments and resolved questions:  

- For now, we will not include an iterator over local (to a
  process) parts within a partition.  If it is needed, it can be added
  later.

- Currently, iMesh doesn't have the functionality to get entities or 
  entity sets by type and tag in serial.  Should it?  
  Many people said it would be useful; others said it could be costly
  (in parallel) or numerically difficult (for floating point values).
  This issue is an iMesh issue, not a parallel interface issue, so
  for this document, the issue is resolved.  The resolution:  If 
  iMesh adopts this capability, we will add it to the
  parallel interface.

- We will not include functions that return all entities with 
  given characteristics within a partition; the memory use of these
  functions can be large.  Instead, we will return entity information
  with respect to parts and/or mesh instances.  If the user wants such
  info, he should go through the mechanics of gathering it himself so
  that he is painfully aware of how much memory he is allocating.
  Removed the following global queries:
  + All tag names over the partition;
  + All entities in this partition having a given type, tag and/or 
    tag name.
  + All entity sets in this partition having a given
    type, tag and/or tag name.

- We will not include functions that return information about each
  part and/or process in a partition.  Such functions limit memory 
  scalability for large numbers of parts.  If the user wants such
  info, he should go through the mechanics of gathering it himself so
  that he is painfully aware of how much memory he is allocating.
  Removed the following global queries:
  + The number of entities in each part of the partition;
  + The number of entity sets in each part of the partition;
  + The number of entities with given type, tag, and/or
    tag name in each part of the partition;
  + The number of entity sets with given type, tag, 
    and/or tag name in each part of the partition;
  + All tag names in each part of the partition;

- Part assignments computed with respect to a set of 
  entities induce part assignments to adjacent entities in an
  implementation-dependent fashion.  That is, if a partition is computed
  with respect to regions, queries about ownership of faces and vertices
  are valid.

------------------------------------------------
Discussed but unresolved questions:

- We discussed adding functions that give
  hints to an implementation about which data mappings the application 
  will use, allowing the implementation to pre-compute them if it chooses 
  to.  The example discussed was mapping between entities and parts, but 
  other examples in iMesh may also exist.

- We discussed storing in a partition 
  information about which "objects" were used in computing the partition.  
  These objects can be single entities or groups of entities.
  KDD:  Perhaps this capability should be part of the load-balancing service.

*/

/*--------------------------------*/
/* NOTES FROM BOOTCAMP MARCH 2008 */
/*--------------------------------*/
/*
-  Changed getPartRank to getRankOfPart.  (Carl)
-  Made sure iMeshP_getNumOfTypeAll and iMeshP_getNumOfTopoAll were
   documented as collective operations.  (Carl)
-  Changed suffix "Par" to "All".  (Lori)
-  Added iMeshP_testPart() to test status of part handle, returning
   LOCAL, REMOTE, or INVALID.  (Mark M, Lori).
   6/25/08:  Removed this function since part handles can no longer be remote.
   If an application wants to test the validity of a part handle, it can try
   to compute its Part ID.
-  Changed iMeshP_addCopyOf and iMeshP_rmvCopyOf back to
   iMeshP_addGhostOf and iMeshP_rmvGhostOf.  If we wanted to use these
   functions for adding boundary copies, we'd have to include a list of
   already existing remote copies in the arguments, as well as
   communicate with parts already owning copies to let them know a ghost
   copy has been made.  Actually, this raises an interesting question:
   does a boundary copy need to know about all ghost copies of it?
-  Change getEntParStatus to getEntStatus.  (Lori)
-  Changed sendEntArrToPartsPar to exchEntArrToPartsAll.  (Lori,Tim)


Parts and Processes:
-  Martin argued for consecutive unique Part IDs in addition to or
   instead of Part handles.  He will send use cases.   If we decide to
   add them back to the interface, we could compute them in
   iMeshP_syncPartitionAll rather than in iMeshP_createPart.  That is, an
   application couldn't access them until after iMeshP_syncPartitionAll.
   6/25/08:  On follow-up, Martin couldn't recall why having consecutive
   PartIDs was necessary.  While we all agree they are conceptually nice,
   they are difficult to implement and not really necessary.  Part IDs will
   be globally unique but not necessarily consecutive.
-  Are part handles globally unique?  They probably need to be
   globally unique in order for them to be useful as remote part
   handles.  Also, does the process rank need to be encoded in the part
   handle in order to map from parts to processes for communication?
   6/25/08:  DECIDED:  We will have globally unique part IDs.  Part handles
   will be valid for only local parts.  Accessing remote parts must be done
   via Part IDs.
-  If in iMeshP_syncPartitionAll, we computed a mapping from part
   handles to integers 0,..., k-1, we could store only ranges of
   integers to achieve the part-to-process and process-to-parts mappings;
   this would require O(P) storage per process for P processes.
   6/5/08:  DECIDED:  Do not need getPartOnRank or getNumPartOnRank.  These
   functions were troublesome due to their storage or communication 
   requirements.  We decided to remove them.
-  Alternatively, the mapping of all parts to processes can be stored
   in O(k) total memory, distributed across processors (e.g., a
   distributed data directory) but interrogating the directory requires
   communication.  
   6/5/08:  See note above.
-  iMeshP_getPartsOnRank created discussion and needs to be resolved.
   IMeshP_getPartsOnRank would likely require either O(k) storage per
   process for k parts or communication.  For other points, please see
   Mark M's 3/12/08 email.  
   6/5/08:  See note above.

CreateEnt:
-  Carl asked if we should have a version of createEnt that accepts a
   part handle.  Should this function be used only for creating owned
   entities?   How do you envision creating part boundary entities when a
   parallel mesh is initially loaded?  

Ghost entities:
-  We currently have a mechanism only for pushing ghosts onto other
   parts.  Will we want a mechanism for pulling them, too?  (E.g., a
   part says, "I want ghosts for this entity.")

PartNbor functions:
-  Did we agree to remove the entity type from these functions?  That
   is, do we want to return the part IDs for all parts that have
   any copies?  The entity type was intended to allow us to get the part
   IDs for all parts that have copies of a given type (perhaps
   ALL_TYPES).  

Functions handling both Parts and Entity Sets:
-  Tim said these function names (e.g., iMeshP_getNumOfType,
   iMeshP_getAllVtxCoord) are too close to existing iMesh function
   names, even though the argument lists would be different.  He agreed
   to email suggestions for better names.

Copies:
-  Functions getNumCopies, getCopies, getCopyParts, and getCopyOnPart
   have different behavior for ghost and part-boundary copies.  Ghosts
   will return only itself and its owner in getCopies; part-boundary
   entities will return copies on other parts as well.
-  Tim envisions applications (e.g., FETI methods) updating tag data
   in their copies that they would like to accumulate back to the
   owner.  Xiaolin said that he writes in his ghosts, but doesn't send
   those values back to the owner.  Currently, we have the ability 
   to send tag data only from owners to ghosts.  Tim will look at this issue
   and propose a solution.

Communication:
-  Although we should think in terms of parts, communication really
   occurs with respect to processes.  We need to make sure all our
   communication routines make sense with respect to both processes and
   parts, and perhaps, revise their descriptions.  Also, if we send to
   parts, the implementation really does need the mapping of parts to
   processes. 

Entity Owner/Status Queries:
-  Should we combine functions getEntOwnerPart and getEntStatus into
   one function?  Or should we combine functions getOwnerCopy and
   getEntOwner into one function?  Or should we remove getOwnerCopy and
   make applications call getOwnerPart followed by getCopyOnPart?

Reducing storage:
-  Mark Miller proposed allowing the user to specify the amount of 
   copying done by the implementation, depending on applications' needs.
   For example, for a static viz application, every boundary entity may not
   need to know where all its copies are, so the implementation would not
   have to store them.  Can the implementations accept a flag advising them how
   much copying is needed?  If so, the implementations could choose to 
   optimize storage or ignore the flag.
*/



/*--------------------------------------------------------------------
   SVN File Information
  
     $svn:author$
     $svn:date$
     $svn:revision$
 *--------------------------------------------------------------------*/


#ifdef __cplusplus
} /*  extern "C"  */
#endif

/** \} */ /* end group IMESHP */

#endif /* !defined(_ITAPS_iMeshP) */
