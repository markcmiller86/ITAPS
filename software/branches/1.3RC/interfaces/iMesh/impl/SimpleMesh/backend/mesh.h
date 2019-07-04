#ifndef MESH_H
#define MESH_H

#include "TSTTmesh.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Private definitions */
#define XDIR 0
#define YDIR 1

#define NUM_DIM 2
#define NUM_OF_VERTS 3

/* tag data structure */

typedef struct __Tag {
  int       id;
  char      name[128];
  int       size;
  TagValueType tag_type;
  int       num_entities;
  int       num_entity_sets;
  int       num_mesh;
  struct    __Entity_list *entity_list;
  struct    __EntitySet_list *entityset_list;
} Tag;

typedef struct __Tag_list {
  Tag *tag;
  struct __Tag_list *next;
} Tag_list;

typedef struct __Tag_Instance {
  Tag       *parent_tag;
  void      *value;
} Tag_Instance;

typedef struct __Tag_Instance_list {
  Tag_Instance *tag_instance;
  struct __Tag_Instance_list *next;
} Tag_Instance_list;

typedef struct __Int_Tag_Instance {
  Tag       *parent_tag;
  int       value;
} Int_Tag_Instance;

typedef struct __Int_Tag_Instance_list {
  Int_Tag_Instance *tag_instance;
  struct __Int_Tag_Instance_list *next;
} Int_Tag_Instance_list;

typedef struct __Dbl_Tag_Instance {
  Tag       *parent_tag;
  double    value;
} Dbl_Tag_Instance;

typedef struct __Dbl_Tag_Instance_list {
  Dbl_Tag_Instance *tag_instance;
  struct __Dbl_Tag_Instance_list *next;
} Dbl_Tag_Instance_list;

typedef struct __TagRepository {
  int next_ID;
  int num_tags;
  Tag_list *tag_list;
} TagRepository;


typedef struct __EntityHandle {
  EntityType entity_type;
  EntityTopology entity_topology;
  void *entity_ptr;
  Tag_Instance_list  *tag_instance_list;
  Int_Tag_Instance_list  *int_tag_instance_list;
  Dbl_Tag_Instance_list  *dbl_tag_instance_list;
  int num_tags;
  int num_int_tags;
  int num_dbl_tags;
} EntityHandle;

typedef struct __Entity_list {
  EntityHandle *entity;
  struct __Entity_list *next;
} Entity_list;

/* mesh data structure */
typedef struct __Vtx {
  double   coord[NUM_DIM];
  int      id;
  EntityHandle *entity_handle;
} Vtx;

typedef struct __Vtx_list {
  Vtx *vtx;
  struct __Vtx_list *next;
} Vtx_list;

typedef struct __Elem {
  Vtx    *vtx[NUM_OF_VERTS];
  int    id;
  EntityHandle *entity_handle;
} Elem;

typedef struct __Elem_list {
  Elem *elem;
  struct __Elem_list *next;
} Elem_list;

typedef struct __EntitySet {
  int isList;
  int dimension;         /* the dimension of the mesh */
  int full_mesh;
  int num_vtx;           /* the number of vertices in the mesh */
  int num_elem;          /* the number of elements in the mesh */
  int num_entitysets;
  Vtx_list *vtx_list;
  Elem_list *elem_list;
  struct __EntitySet_list *entity_set_list; /* a list containing entitysets */
  int num_tags;
  int num_int_tags;
  int num_dbl_tags;
  Tag_Instance_list *tag_instance_list;
  Int_Tag_Instance_list  *int_tag_instance_list;
  Dbl_Tag_Instance_list  *dbl_tag_instance_list;
} EntitySet;

typedef struct __EntitySet_list {
  EntitySet *entity_set;
  struct __EntitySet_list *next;
} EntitySet_list;

typedef struct __Workset_Iterator {
  EntityType entity_type;
  EntityTopology entity_topology;
  int requested_workset_size;
  void *list;
  void *list_start;
} Workset_Iterator;

typedef struct __Iterator {
  EntityType entity_type;
  EntityTopology entity_topology;
  void *list;
  void *list_start;
} Iterator;



/* private functions */
Vtx *getVtxFromID(Vtx_list *vtx_list, int id);
Elem *getElemFromID(Elem_list *elem_list, int id);
Tag_list *create_tag_list(char *name, int size, void *value);
Tag_list *delete_tag(Tag_list *tag_list, char *name);
void init_entity_handle(EntityHandle *entity_handle, EntityType entity_type, EntityTopology entity_topology);

/* mesh macros */
#define ADD_TO_LIST(list,item) {\
  (item)->next = (list); \
  (list) = (item); \
}

#define ADV_ELEM_LIST(list,item) {\
  (list) = (list)->next; \
  (item) = (list)->elem; \
}

#define ADV_VTX_LIST(list,item) {\
  (list) = (list)->next; \
  (item) = (list)->vtx; \
}

#define ADV_TAG_LIST(list,item) {\
  (list) = (list)->next; \
  (item) = (list)->tag; \
}

#ifndef DO_TIMING
#define PRINT_VTX_LIST(list) {\
  Vtx *vtx99;\
  Vtx_list *vtx_list99;\
  vtx_list99 = (list);\
  vtx99 = vtx_list99->vtx; \
  while(vtx99!=NULL){\
     printf("vtx %d: x %f  y %f\n",vtx99->id,vtx99->coord[XDIR],vtx99->coord[YDIR]); \
    ADV_VTX_LIST(vtx_list99,vtx99);\
  }\
}
#else /* !DO_TIMING */
#define PRINT_VTX_LIST(list) {\
  Vtx *vtx99;\
  Vtx_list *vtx_list99;\
  vtx_list99 = (list);\
  vtx99 = vtx_list99->vtx; \
  while(vtx99!=NULL){\
    ADV_VTX_LIST(vtx_list99,vtx99);\
  }\
}
#endif

#define COMPUTE_TRI_AREA(elem,area) {\
  double x0_99, x1_99, x2_99; \
  double y0_99, y1_99, y2_99; \
  x0_99=(elem)->vtx[0]->coord[0]; \
  y0_99=(elem)->vtx[0]->coord[1]; \
  x1_99=(elem)->vtx[1]->coord[0]; \
  y1_99=(elem)->vtx[1]->coord[1]; \
  x2_99=(elem)->vtx[2]->coord[0]; \
  y2_99=(elem)->vtx[2]->coord[1]; \
  area = .5*(x1_99*y2_99-x1_99*y0_99-x0_99*y2_99-x2_99*y1_99+x2_99*y0_99+x0_99*y1_99);\
}

#ifndef DO_TIMING
#define PRINT_ENTITY_HANDLE(a) { \
   printf("Entity Type = %d \n", a->entity_type); \
   printf("Entity Topology = %d \n", a->entity_topology); \
   printf("Entity Ptr = %g \n", a->entity_ptr); \
}
#endif /* !DO_TIMING */

/* Error Macros */
#define CHK_NULL(a,b,s) { \
 if ((a)==NULL) {\
    (b)->code = 1;\
    sprintf((b)->msg,"NULL %s handle in %s\n",s,__FUNC__);\
 }\
}

#define CHK_SUPPORTED_TYPE(a,b) {\
  if ( (a!=VERTEX) && (a!=FACE) ) { \
    (b)->code = 1;\
    sprintf((b)->msg,"Type %d not supported in %s \n",a,__FUNC__);\
  }\
}

#define CHK_SUPPORTED_TOPO(a,b) {\
  if ( (a!=POINT) && (a!=TRIANGLE) ) { \
    (b)->code = 1;\
    sprintf((b)->msg,"Topology %d not supported in %s \n",a,__FUNC__);\
  }\
}

#define CHK_TYPE_TOPO_COMPATIBILITY(a,b,e) {\
  switch(a) { \
  case VERTEX: \
    if (b!=POINT) (e)->code=1; break; \
  case EDGE:  \
    if (b!=LINE_SEGMENT) (e)->code=1; break; \
  case FACE: \
    if ( (b!=TRIANGLE) && (b!=QUADRILATERAL) && (b!=POLYGON) ) \
      (e)->code = 1;\
    break;\
  case REGION: \
    if ( (b!=TETRAHEDRON) && (b!=HEXAHEDRON) && (b!=POLYHEDRON) && \
         (b!=SEPTAHEDRON) && (b!=PRISM) && (b!=PYRAMID) ) \
       (e)->code = 1;\
    break;\
  }\
  if ((e)->code == 1) {\
    sprintf((e)->msg,"Incompatible Type(%d) and Topology (%d) in %s \n",a,b,__FUNC__);\
  }\
}

#define CHK_STORAGE_ORDER_OPTION(a,b) {\
  if ( (a!=UNDETERMINED) && (a!=BLOCKED) && (a!=INTERLEAVED) ) { \
    (b)->code = 1;\
    sprintf((b)->msg,"Storage order %d not supported in %s \n",a,__FUNC__);\
  }\
}

#define CHK_TAG_TYPE(a,b,c) {\
  if  (a!=b) { \
    (c)->code = 1;\
    sprintf((c)->msg,"Incompatible Tag Type %d in Function %s \n",a,__FUNC__);\
  }\
}

#define CHK_RTN(e) { if ( (e)->code!=0 ) return; }
#define CHK_RTNV(e) { if ( (e)->code!=0 ) return(0); }


#ifdef __cplusplus__
}
#endif

#endif
