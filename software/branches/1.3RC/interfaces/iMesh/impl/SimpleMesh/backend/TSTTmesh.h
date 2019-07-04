#ifndef TSTT_MESH_H
#define TSTT_MESH_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  SUCCESS,	
  MESH_ALREADY_LOADED,	
  NO_MESH_DATA,	
  FILE_NOT_FOUND,	
  FILE_WRITE_ERROR,	
  NIL_ARRAY,	
  BAD_ARRAY_SIZE,	
  BAD_ARRAY_DIMENSION,	
  INVALID_ENTITY_HANDLE,	
  INVALID_ENTITY_COUNT,	
  INVALID_ENTITY_TYPE,	
  INVALID_ENTITY_TOPOLOGY,
  BAD_TYPE_AND_TOPO,	
  ENTITY_CREATION_ERROR,
  INVALID_TAG_HANDLE,
  TAG_NOT_FOUND,	
  TAG_ALREADY_EXISTS,
  TAG_IN_USE,
  INVALID_ENTITYSET_HANDLE,	
  INVALID_ITERATOR_HANDLE,	
  INVALID_ARGUMENT,	
  MEMORY_ALLOCATION_FAILED,
  NOT_SUPPORTED,	
  FAILURE	
} ErrorType; 

typedef enum {
  VERTEX,
  EDGE,
  FACE,
  REGION,
  NUMBER_OF_ENTITY_TYPES,
  ALL_TYPES
} EntityType;

typedef enum {
  POINT,
  LINE_SEGMENT,
  POLYGON,
  TRIANGLE,
  QUADRILATERAL,
  POLYHEDRON,
  TETRAHEDRON,
  HEXAHEDRON,
  PRISM,
  PYRAMID,
  SEPTAHEDRON,
  UNDEFINED,
  ALL_TOPOLOGIES
} EntityTopology;

typedef enum {
  BLOCKED,
  INTERLEAVED,
  UNDETERMINED
} StorageOrder;

typedef enum {
  UNAVAILABLE,          /**< Adjacency information not supported */
  IMMEDIATE,            /**< No mesh traversal required */
  LOCAL_TRAVERSAL,      /**< Must traverse entity's neighborhood */
  GLOBAL_TRAVERSAL      /**< Must traverse entire mesh */
} AdjacencyCost ;

typedef enum {
  SILENT,
  WARN_ONLY,
  ABORT_ON_ERROR,
  PRINT_AND_THROW_ERROR,
  THROW_ERROR
} ErrorActions;    

typedef enum {
  INTEGER,
  DOUBLE,
  ENTITY_HANDLE,
  OPAQUE 
} TagValueType;

typedef struct {
  int code;
  char msg[128];
} Error;

#define CHK_ERR(a) {\
  if ((a).code != 0) { \
    printf("%s\n",(a).msg); \
    return; \
  }\
}
  

#define CHK_ERR_R(a) {\
  if ((a).code != 0) { \
    printf("%s\n",(a).msg); \
    return ((a).code); \
  }\
}
  

/* public interface functions */
void TSTT_create(void **mesh, Error *err);
void TSTT_load(void *mesh, const char *name, Error *err);
void TSTT_save(void *mesh, const char *name, Error *err);
void *TSTT_getRootSet(void *mesh1, Error *err);
int TSTT_getGeometricDim(void *mesh, Error *err);
StorageOrder TSTT_getDfltStorage(void *mesh, Error *err);
void TSTT_getAdjTable(void *mesh, AdjacencyCost **table, 
                      int *table_size, Error *err);
int  TSTT_getNumOfType(void *entity_set1, EntityType entity_type, 
		       Error *err);
int  TSTT_getNumOfTopo(void *entity_set1, EntityTopology entity_topology, 
                       Error *err);


void TSTT_getAllVtxCoords(void *entity_set, double **coords, int *n, 
			  int **in_entity_set, int *m,
 			  StorageOrder *storage_order, 
			  Error *err);

void TSTT_getVtxCoordIndex(void *entity_set, 
		           EntityType requested_entity_type,
			   EntityTopology requested_entity_topology,
			   EntityType entity_adjacency_type,
			   int **offset, int *offset_size,
                           int **index, int *index_size,
			   EntityTopology **entity_topologies,
                           int *entity_topologies_size,
			   Error *err);

void TSTT_getEntities(void *entity_set1, EntityType entity_type,
	              EntityTopology entity_topology, 
                      void ***entity_handles1, int *num_entities, 
		      Error *err);

void TSTT_getVtxArrCoords(void **entity_handles, int num_entities,
   		    StorageOrder *storage_order, double **coords, 
                    int *coord_size, Error *err);

void TSTT_getAdjEntities(void *entity_set,
                         EntityType entity_type_requestor, 
                         EntityTopology entity_topology_requestor,
                         EntityType entity_type_requested,
                         void ***adj_entity_handles, 
                         int *adj_entity_handles_size,
                         int **offset, int *offset_size,
                         int **in_entity_set, int *in_entity_set_size, Error *err);

/* array iterator functions */
int TSTT_initEntArrIter(void *entity_set_handle,
                      EntityType requested_entity_type, 
                      EntityTopology requested_entity_topology,
                      int requested_workset_size,
                      void **workset_iterator, Error *err);

int TSTT_getEntArrNextIter(void *workset_iterator, void ***entity_handles, 
                      int *num_entities, Error *err);

void TSTT_resetEntArrIter(void *workset_iterator, Error *err);

void TSTT_endEntArrIter(void *workset_iterator, Error *err);

void TSTT_getEntArrTopo(void **entity_handles1, int num_entities,
		     EntityTopology **topology, int *num_topo, Error *err);
void TSTT_getEntArrType(void **entity_handles1, int num_entities,
		     EntityType **type, int *num_type, Error *err);
void TSTT_getEntArrAdj(void **entity_handles1, int num_entities,
                         EntityType entity_type_requested,
                         void ***adj_entity_handles, 
                         int *num_adj_entities,
                         int **offset,
                         int *offset_size,
		       Error *err);

/* single entity iterators */
int TSTT_initEntIter(void *entity_set_handle,
                      EntityType requested_entity_type, 
                      EntityTopology requested_entity_topology,
                      void **iterator, Error *err);

int TSTT_getEntNextIter(void *iterator, void **entity_handle, 
                       Error *err);

void TSTT_resetEntIter(void *iterator, Error *err);

void TSTT_endEntIter(void *iterator, Error *err);

EntityTopology TSTT_getEntTopo(void *entity_handle, Error *err);
EntityType TSTT_getEntType(void *entity_handle, Error *err);
void TSTT_getVtxCoord(void *entity_handle, double **coords, int *coords_size, 
                      Error *err);
void TSTT_getEntAdj(void *entity_handle1, 
                         EntityType entity_type_requested,
                         void ***adj_entity_handles, 
                         int *num_adj_entities,
		    Error *err);

/* tag functions */
void TSTT_createTag(char *name, int size, TagValueType tag_type,
                    void **tag_handle, Error *err);
void TSTT_destroyTag(void *tag_handle, int forced, Error *err);
char* TSTT_getTagName(void *tag_handle, Error *err);
int TSTT_getTagSize(void *tag_handle, Error *err);
TagValueType TSTT_getTagType(void *tag_handle, Error *err);
void *TSTT_getTagHandle(char *name, Error *err);

/* entity tag functions */
void   TSTT_getAllTags( void *entity_handle, void ***tag_handles,
                      int *tag_handles_size, Error *err);
/* LORI this needs to be an array of char */
void   TSTT_getData(void *entity_handle, void *tag_handle,
                  void **tag_value, int *tag_size, Error *err);
int    TSTT_getIntData(void *entity_handle, void *tag_handle, Error *err);
double TSTT_getDblData(void *entity_handle, void *tag_handle, Error *err);
void  **TSTT_getEHData(void *entity_handle, void *tag_handle, Error *err);
/* LORI this needs to be an array of char */
void   TSTT_setData(void *entity_handle, void *tag_handle, 
                  void *tag_value, int tag_size, Error *err);
void   TSTT_setIntData(void *entity_handle1, void *tag_handle1, 
                     int tag_value, Error *err);
void   TSTT_setDblData(void *entity_handle1, void *tag_handle1, 
                     double tag_value, Error *err);
void   TSTT_setEHData(void *entity_handle, void *tag_handle, 
                  void *tag_value, Error *err);
void   TSTT_rmvTag(void *entity_handle, void *tag_handle, Error *err);

/* entity array tag functions */
void TSTT_rmvArrTag(void **entity_handles, int num_entities, 
			  void *tag_handle, Error *err);
/*LORI this needs to be array of char */
void TSTT_getArrData(void **entity_handles, int num_entities, void *tag_handle,
		     void **tag_values, int *tag_value_size, int *tag_size, Error *err);
void TSTT_getIntArrData(void **entity_handles, int num_entities,
                        void *tag_handle, int **tag_value,
                        int *tag_value_size, Error *err);
void TSTT_getDblArrData(void **entity_handles1, int num_entities,
                        void *tag_handle1, double **tag_values,
                        int *tag_value_size, Error *err);
void TSTT_getEHArrData(void **entity_handles, int num_entities, void *tag_handle,
		     void **tag_values, int *tag_value_size, Error *err);
/*LORI  needs to be an array of char */
void TSTT_setArrData(void **entity_handles, int num_entities, void *tag_handle,
		     void **tag_values, int tag_value_size, 
                     int tag_size, Error *err);
void TSTT_setIntArrData(void **entity_handles, int num_entities, void *tag_handle1,
			int *tag_values, int tag_values_size, Error *err);
void TSTT_setDblArrData(void **entity_handles, int num_entities, void *tag_handle1,
			double *tag_values, int tag_values_size, Error *err);
void TSTT_setEHArrData(void **entity_handles, int num_entities, void *tag_handle,
		     void **tag_values, int tag_value_size, Error *err);


/* entity set functions */
void TSTT_createEntSet(int isList, void **entity_set1, Error *err);
void TSTT_destroyEntSet(void *entity_set1, Error *err) ;
int TSTT_isList(void *entity_set1, Error *err);
void TSTT_addEntToSet(void *entity_handle, void *entity_set, Error *err);

#ifdef __cplusplus__
}
#endif

#endif

