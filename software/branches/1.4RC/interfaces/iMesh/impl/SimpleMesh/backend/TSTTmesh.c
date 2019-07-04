#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "mesh.h"

EntitySet *mesh;
Vtx *getVtxFromID(Vtx_list *vtx_list, int id);

#undef __FUNC__
#define __FUNC__ "TSTT_create"
void TSTT_create(void **mesh1, Error *err)
{
  mesh = (EntitySet *) malloc(sizeof(EntitySet));
  mesh->full_mesh=1;
  (*mesh1) = (void *)mesh;
}

#undef __FUNC__
#define __FUNC__ "TSTT_load"
void TSTT_load(void *mesh1, const char *name, Error *err)
{
  int  i, j, id;
  int  vtx_id0, vtx_id1, vtx_id2;
  int corner_tag, boundary_tag;
  int  dim, att, bndy;
  double dummy;
  int  num_vtx, num_elem;
  Vtx_list *vtx_list;
  Elem_list *elem_list;
  Tag_list *tag_list;
  char node_file[128], tri_file[128];
  FILE *node_fp, *tri_fp;
  int *value;
  Tag *bndy_tag_handle;
  void *bndy_tag;
  void *vtx_id_tag;
  void *user_data_tag;
  void *elem_id_tag;
  void *area_tag;
  int zero=0;
  double dzero=0;
  void *tag_entity_handle;
  void **tag_value;
  void **tag_valuer;
  double **dtag_value;
  int **itag_value;
  double area;

  Elem  *elem;

  CHK_NULL(mesh1,err,"mesh");
  CHK_RTN(err);

  mesh =(EntitySet *)mesh1;
  tag_entity_handle = (void **)malloc(sizeof(void *));

  sprintf(node_file,"%s.%s",name,"node");
  sprintf(tri_file,"%s.%s",name,"ele");

  /*  printf("\nInitializing the Triangle mesh: %s \n", filebase); */
  if ((node_fp =  fopen(node_file,"r")) == NULL) {
    printf("cannot open node_file\n");
    exit(0);
  }
  if ((tri_fp =  fopen(tri_file,"r")) == NULL) {
    printf("cannot open tri_file\n");
    exit(0);
  }

  fscanf(node_fp,"%d %d %d %d", &num_vtx,&dim,&att,&bndy);
  if (dim != 2) {
    printf("Bad number of dimensions\n"); exit(0);
  }
  if (bndy != 1) {
    printf("No boundary information\n"); exit(0);
  }

  mesh->dimension = 2;

  fscanf(tri_fp,"%d %d %d", &num_elem,&id,&id);
  mesh->num_vtx = num_vtx;
  mesh->num_elem = num_elem;
  mesh->vtx_list = (Vtx_list *) malloc(sizeof(Vtx_list));
  mesh->vtx_list->vtx = NULL;
  mesh->elem_list = (Elem_list *) malloc(sizeof(Elem_list));
  mesh->elem_list->elem = NULL;
  mesh->entity_set_list = (EntitySet_list *) malloc(sizeof(EntitySet_list));
  mesh->entity_set_list->entity_set = NULL;

  /* Create and add the vertices */

  TSTT_createTag("Boundary",sizeof(int),INTEGER, &bndy_tag,err); CHK_ERR(*err);
  TSTT_createTag("Vtx_ID",sizeof(int),INTEGER,&vtx_id_tag,err); CHK_ERR(*err);

  for (i=0;i<num_vtx;i++) {
    vtx_list = (Vtx_list *) malloc(sizeof(Vtx_list));
    vtx_list->vtx = (Vtx *) malloc(sizeof(Vtx));
    vtx_list->vtx->entity_handle = (EntityHandle *)malloc(sizeof(EntityHandle));
    vtx_list->vtx->entity_handle->entity_ptr = (void *)vtx_list->vtx;
    init_entity_handle(vtx_list->vtx->entity_handle,VERTEX, POINT);

 
    fscanf(node_fp,"%d %lf %lf",&id,
                                &(vtx_list->vtx->coord[XDIR]),
                   	        &(vtx_list->vtx->coord[YDIR]));

    vtx_list->vtx->id = id-1;    
    for (j=0;j<att;j++) fscanf(node_fp,"%d",&corner_tag);
    fscanf(node_fp,"%d",&boundary_tag);

    tag_value = (void **)malloc(sizeof(void *));

    tag_entity_handle = vtx_list->vtx->entity_handle;

    /* add the boundary, ID, and user data tags */
    TSTT_setIntData(tag_entity_handle, bndy_tag,
                    boundary_tag,err); CHK_ERR(*err);

    TSTT_setIntData(tag_entity_handle, vtx_id_tag,
                    vtx_list->vtx->id,err); CHK_ERR(*err);

    ADD_TO_LIST(mesh->vtx_list, vtx_list);
  }

  /* Create and add the elements */

  TSTT_createTag("Area",sizeof(double),DOUBLE, &area_tag,err); CHK_ERR(*err);
  TSTT_createTag("Elem_ID",sizeof(int),INTEGER, &elem_id_tag,err); CHK_ERR(*err);

  for (i=0;i<num_elem;i++) {
    elem_list = (Elem_list *) malloc(sizeof(Elem_list));
    elem_list->elem = (Elem *) malloc(sizeof(Elem));
    elem_list->elem->entity_handle = (EntityHandle *)malloc(sizeof(EntityHandle));
    elem_list->elem->entity_handle->entity_ptr = (void *)elem_list->elem;
    init_entity_handle(elem_list->elem->entity_handle,FACE,TRIANGLE);
 
    fscanf(tri_fp,"%d %d %d %d",&(elem_list->elem->id),
                   &vtx_id0, &vtx_id1, &vtx_id2);
    vtx_id0--;     vtx_id1--;     vtx_id2--; 

    elem_list->elem->vtx[0]=getVtxFromID(mesh->vtx_list,vtx_id0);
    elem_list->elem->vtx[1]=getVtxFromID(mesh->vtx_list,vtx_id1);
    elem_list->elem->vtx[2]=getVtxFromID(mesh->vtx_list,vtx_id2);
    elem_list->elem->id = i;

    /* compute the area of the element */
    COMPUTE_TRI_AREA(elem_list->elem,area);
    if (area <= 0) printf("Element %d Area < 0\n",elem_list->elem->id);

    /* add the area and ID tag */
    tag_entity_handle = (void *)elem_list->elem->entity_handle;
    TSTT_setDblData(tag_entity_handle,area_tag,area,err); CHK_ERR(*err);
    TSTT_setIntData(tag_entity_handle,elem_id_tag,elem_list->elem->id, err); 
                    CHK_ERR(*err);

    ADD_TO_LIST(mesh->elem_list, elem_list);
  }  

}

void init_entity_handle(EntityHandle *entity_handle, EntityType entity_type, EntityTopology entity_topology)
{
    entity_handle->entity_type = entity_type;
    entity_handle->entity_topology = entity_topology;
    entity_handle->tag_instance_list = (Tag_Instance_list *) malloc(sizeof(Tag_Instance_list));
    entity_handle->int_tag_instance_list = (Int_Tag_Instance_list *) malloc(sizeof(Int_Tag_Instance_list));
    entity_handle->dbl_tag_instance_list = (Dbl_Tag_Instance_list *) malloc(sizeof(Dbl_Tag_Instance_list));
    entity_handle->tag_instance_list->tag_instance = NULL;
    entity_handle->int_tag_instance_list->tag_instance = NULL;
    entity_handle->dbl_tag_instance_list->tag_instance = NULL;
    entity_handle->num_tags=0;
    entity_handle->num_int_tags=0;
    entity_handle->num_dbl_tags=0;
}

Vtx *getVtxFromID(Vtx_list *vtx_list, int id)
{
  Vtx *vtx;
  vtx = vtx_list->vtx;
  while (vtx!=NULL) {
    if (vtx->id == id) return vtx;
    ADV_VTX_LIST(vtx_list,vtx);
  }
  return NULL;
}

#undef __FUNC__
#define __FUNC__ "TSTT_save"
void TSTT_save(void *mesh1, const char *name, Error *err)
{
 int  i, j, id;
  int  vtx_id0, vtx_id1, vtx_id2;
  int  dim, att, bndy;
  double dummy;
  int  num_vtx, num_elem;
  Vtx_list *vtx_list;
  Elem_list *elem_list;
  Tag_list *tag_list;
  char node_file[128], tri_file[128];
  FILE *node_fp, *tri_fp;
  int *value;
  double **dtag_value;
  int **itag_value;
  void *bndy_tag;
  void *vtx_id_tag;
  void *user_data_tag;
  void *elem_id_tag;
  void *area_tag;
  int zero=0;
  double dzero=0;
  void **tag_entity_handle;
  void **tag_value;
  Elem  *elem;
  double **coords;
  int *corner_tag, *boundary_tag;
  int **vtx_ind;
  void *bndy_tag_handle;

  CHK_NULL(mesh1,err,"mesh");
  CHK_RTN(err);

  mesh =(EntitySet *)mesh1;

  sprintf(node_file,"%s.%s",name,"node");
  sprintf(tri_file,"%s.%s",name,"ele");

  if ((node_fp =  fopen(node_file,"w")) == NULL) {
    printf("cannot open node_file\n");
    exit(0);
  }
  if ((tri_fp =  fopen(tri_file,"w")) == NULL) {
    printf("cannot open tri_file\n");
    exit(0);
  }

  fprintf(node_fp,"%d %d %d %d \n", mesh->num_vtx,mesh->dimension,1,1);
  fprintf(tri_fp,"%d %d %d \n", mesh->num_elem,3,0);

  /* triangle requires that the nodes be ordered from 1 to N
     not N down to 1 */
  coords = (double **)malloc(sizeof(double *)*mesh->num_vtx);
  corner_tag = (int *)malloc(sizeof(int)*mesh->num_vtx);
  boundary_tag = (int *)malloc(sizeof(int)*mesh->num_vtx);
  
  vtx_list = mesh->vtx_list;

  bndy_tag_handle = TSTT_getTagHandle("Boundary",err);

  for (i=mesh->num_vtx-1;i>-1;i--) {
   coords[i] = (double *)malloc(sizeof(double)*mesh->dimension);
   coords[i][0] = vtx_list->vtx->coord[XDIR];
   coords[i][1] = vtx_list->vtx->coord[YDIR];
   corner_tag[i] = 0;
   boundary_tag[i] = TSTT_getIntData((void *)vtx_list->vtx->entity_handle,
                                     bndy_tag_handle,err);
   vtx_list = vtx_list->next;
  }

  vtx_list = mesh->vtx_list;
  for (i=0;i<mesh->num_vtx;i++) {
    fprintf(node_fp,"%d %lf %lf %d %d \n",i+1,
	    coords[i][0],coords[i][1],corner_tag[i],boundary_tag[i]);
  }

  vtx_ind = (int **)malloc(sizeof(int *)*mesh->num_elem);
  elem_list = mesh->elem_list;
  for (i=mesh->num_elem-1;i>-1;i--) {
    vtx_ind[i] = (int *)malloc(sizeof(int)*3);
    vtx_ind[i][0] = elem_list->elem->vtx[0]->id+1;
    vtx_ind[i][1] = elem_list->elem->vtx[1]->id+1;
    vtx_ind[i][2] = elem_list->elem->vtx[2]->id+1;
    elem_list = elem_list->next;
  }

  for (i=0;i<mesh->num_elem;i++) {  
    fprintf(tri_fp,"%d %d %d %d \n",i+1,vtx_ind[i][0],
                    vtx_ind[i][1],vtx_ind[i][2]);
  }  

}

#undef __FUNC__
#define __FUNC__ "TSTT_getGeometricDim"
int TSTT_getGeometricDim(void *mesh1, Error *err)
{
  CHK_NULL(mesh1,err,"mesh"); 
  CHK_RTNV(err);

  mesh =(EntitySet *)mesh1;
  return(mesh->dimension);
}

#undef __FUNC__
#define __FUNC__ "TSTT_getRootSet"
void *TSTT_getRootSet(void *mesh1, Error *err)
{
  CHK_NULL(mesh1,err,"mesh"); 
  CHK_RTN(err);

  mesh =(EntitySet *)mesh1;
  return((void *)mesh);
}

#undef __FUNC__
#define __FUNC__ "TSTT_getAllVtxCoords"
void TSTT_getAllVtxCoords(void *entity_set1, double **coords, int *n, 
        int **in_entity_set, int *m, StorageOrder *storage_order,
        Error *err)
{
  EntitySet *entity_set;
  Vtx_list *vtx_list;
  Vtx *vtx;
  int i, j, k;
  int num_vtx;

  CHK_NULL(entity_set1,err,"entity set"); 
  CHK_STORAGE_ORDER_OPTION((*storage_order),err);
  CHK_RTN(err);

  entity_set = (EntitySet *)entity_set1;

  num_vtx = entity_set->num_vtx;
  (*n) = num_vtx*entity_set->dimension;
  (*m) = num_vtx;
  vtx_list=entity_set->vtx_list;

  if ((*storage_order) == UNDETERMINED) {
    (*storage_order) = INTERLEAVED;
  }

  (*coords) = (double *) malloc(sizeof(double)*(*n)); 
  (*in_entity_set) = (int *) malloc(sizeof(int)*num_vtx);

  switch((*storage_order)) {
  case INTERLEAVED:
    k=0;
    for (i=0;i<num_vtx;i++) {
      (*in_entity_set)[i] = 1;
      for (j=0;j<mesh->dimension;j++) {
        (*coords)[k] = vtx_list->vtx->coord[j];
        k++;
      }
      vtx_list = vtx_list->next;
    }
    break;
  case BLOCKED:
    k=0;
    for (i=0;i<num_vtx;i++) {
      vtx = vtx_list->vtx;
      (*in_entity_set)[i] = 1;
      (*coords)[k] = vtx->coord[0];
      (*coords)[k+num_vtx] = vtx->coord[1];
      k++;
      vtx_list = vtx_list->next;
    }
    break;
  default:
    err->code = 1;
    sprintf(err->msg,"Storage order undefined");    
  }

  return;
}

#undef __FUNC__
#define __FUNC__ "TSTT_getVtxCoordIndex"
void TSTT_getVtxCoordIndex(void *entity_set1, 
		           EntityType requested_entity_type,
			   EntityTopology requested_entity_topology,
			   EntityType entity_adjacency_type,
			   int **offset, int *offset_size,
                           int **index, int *index_size,
			   EntityTopology **entity_topologies,
                           int *entity_topologies_size,
			   Error *err)
{
  int i;
  int num_elem;
  Elem_list *elem_list;
  Elem *elem;
  int index_count;
  EntitySet *entity_set;

  CHK_NULL(entity_set1,err,"entity set"); 
  CHK_RTN(err);
  entity_set = (EntitySet *)entity_set1;

  num_elem = entity_set->num_elem;
  elem_list=entity_set->elem_list;

  (*offset) = (int *)malloc(sizeof(int)*(num_elem+1));
  (*offset_size) = num_elem+1;
  (*index) = (int *)malloc(sizeof(int)*num_elem*3);
  (*index_size) = num_elem*3;
  (*entity_topologies) = (EntityTopology *)malloc(sizeof(EntityTopology)*num_elem);
  (*entity_topologies_size) = num_elem;

  elem = elem_list->elem;
  index_count = 0;
  (*offset)[0]=0;
  for (i=0;i<num_elem;i++) {
    (*index)[index_count] = elem->vtx[0]->id;
    (*index)[index_count+1] = elem->vtx[1]->id;
    (*index)[index_count+2] = elem->vtx[2]->id;
   
    index_count+=3;
    (*offset)[i+1]=index_count;
    (*entity_topologies)[i] = TRIANGLE;
    elem_list=elem_list->next;
    elem=elem_list->elem;
  }
}

#undef __FUNC__
#define __FUNC__ "TSTT_getNumOfType"
int TSTT_getNumOfType(void *entity_set1, EntityType entity_type, 
		      Error *err)
{
  EntitySet *entity_set;
 
  CHK_NULL(entity_set1,err,"entity set"); 
  CHK_SUPPORTED_TYPE(entity_type,err);
  CHK_RTNV(err);
 
  entity_set = (EntitySet *)entity_set1;

  /* return the information for the whole mesh */
  switch (entity_type) {
  case VERTEX:
      return(entity_set->num_vtx);  break;
  case FACE:
      return(entity_set->num_elem);  break;
  }
}

#undef __FUNC__
#define __FUNC__ "TSTT_getNumOfTopo"
int TSTT_getNumOfTopo(void *entity_set1, 
		      EntityTopology entity_topology, Error *err)
{
  EntitySet *entity_set;

  CHK_NULL(entity_set1,err,"entity set"); 
  CHK_SUPPORTED_TOPO(entity_topology,err);
  CHK_RTNV(err);

  entity_set = (EntitySet *)entity_set1;
  switch (entity_topology) {
  case POINT:
    return(entity_set->num_vtx);  break;
  case TRIANGLE:
    return(entity_set->num_elem);  break;
  }
}


#undef __FUNC__
#define __FUNC__ "TSTT_getEntities"
void TSTT_getEntities(void *entity_set1, EntityType entity_type,
			       EntityTopology entity_topology, 
                               void ***entity_handles1, int *num_entities, 
                               Error *err)
{
  EntityHandle **entity_handles;
  Vtx_list *vtx_list;
  Vtx *vtx;
  Elem_list *elem_list;
  Elem *elem;
  int i;

  EntitySet *entity_set;
  CHK_NULL(entity_set1,err,"entity set"); 
  CHK_SUPPORTED_TYPE(entity_type,err);
  CHK_SUPPORTED_TOPO(entity_topology,err);
  CHK_TYPE_TOPO_COMPATIBILITY(entity_type,entity_topology,err);
  CHK_RTN(err);

  entity_set = (EntitySet *)entity_set1;

  switch (entity_type) {

  case VERTEX:
      (*num_entities) = entity_set->num_vtx;
      vtx_list = entity_set->vtx_list;

      entity_handles = (EntityHandle **)malloc(sizeof(EntityHandle *)*(*num_entities));
      (*entity_handles1) = (void **)malloc(sizeof(void *)*(*num_entities));
  
      for (i=0;i<(*num_entities);i++) {
        entity_handles[i] = vtx_list->vtx->entity_handle;
        (*entity_handles1)[i] = (void *)entity_handles[i];
        vtx_list = vtx_list->next;
      }
      (*entity_handles1) = (void *) entity_handles;

    break;
  
  case FACE:
      /* return informatin associated with the entity set */
      (*num_entities) = entity_set->num_elem;
      elem_list = entity_set->elem_list; 

      entity_handles = (EntityHandle **)malloc(sizeof(EntityHandle *)*(*num_entities));
      (*entity_handles1) = (void **)malloc(sizeof(void *)*(*num_entities));
  
      for (i=0;i<(*num_entities);i++) {
        entity_handles[i] = elem_list->elem->entity_handle;
        (*entity_handles1)[i] = (void *)entity_handles[i];
        elem_list = elem_list->next;
      }
      (*entity_handles1) = (void *) entity_handles;

    break;
 
  }
}

#undef __FUNC__
#define __FUNC__ "TSTT_getVtxArrCoords"
void TSTT_getVtxArrCoords(void **entity_handles1, int num_entities,
  		    StorageOrder *storage_order, double **coords, 
                    int *coord_size, Error *err)
{
  int i;

  EntityHandle **entity_handles;
  EntityHandle *entity_handle;
  Vtx *vtx;
 
  CHK_NULL(entity_handles,err,"entity handles"); 
  CHK_STORAGE_ORDER_OPTION((*storage_order),err);
  CHK_RTN(err);

  if ((*storage_order) == UNDETERMINED) {
    (*storage_order) = INTERLEAVED;
  }
  
  entity_handles = (EntityHandle **)entity_handles1;

  (*coords) = (double *)malloc(sizeof(double)*num_entities*mesh->dimension);
  (*coord_size) = num_entities*mesh->dimension;
  for (i=0;i<num_entities;i++) {
    entity_handle = (EntityHandle *) entity_handles[i];
    if (entity_handle->entity_type != VERTEX) {
      err->code = 1;
      sprintf(err->msg,"non vertex in entityGetVertexCoordinates");
      free((*coords));
      return;
    } else {
      vtx = (Vtx *) entity_handle->entity_ptr;
      switch ((*storage_order)) {
      case INTERLEAVED:
        (*coords)[2*i] = vtx->coord[0];
        (*coords)[2*i+1] = vtx->coord[1];
        break;
      case BLOCKED:
        (*coords)[i] = vtx->coord[0];
        (*coords)[i+num_entities] = vtx->coord[1];
        break;
      }
    }
  }
}

#undef __FUNC__
#define __FUNC__ "TSTT_entitysetGetAdjacentEntities"
void TSTT_entitysetGetAdjacentEntities(void *entity_set1,
                    EntityType entity_type_requestor, 
                    EntityTopology entity_topology_requestor,
                    EntityType entity_type_requested,
                    void ***adj_entity_handles, int **offset,
                    int **in_entity_set, int* num_adj_entities,
                    Error *err)
{
  int num_elem;
  Elem_list *elem_list;
  Elem *elem;
  int i;
    
  EntitySet *entity_set;
  CHK_NULL(entity_set,err,"entity set"); 
  CHK_RTN(err);
  entity_set = (EntitySet *)entity_set1;

  if ((entity_type_requestor != FACE) || 
      (entity_topology_requestor != TRIANGLE) ||
      (entity_type_requested != POINT)) {
    err->code = 1;
    sprintf(err->msg,"Do not support requestor type, topology or requested type");
    return;
  }

  num_elem = entity_set->num_elem;
  elem_list= entity_set->elem_list;

  (*num_adj_entities) = 3*num_elem;
  (*adj_entity_handles) = (void **)malloc(sizeof(void *)*(*num_adj_entities));
  (*offset) = (int *)malloc(sizeof(int)*(num_elem+1));
  (*in_entity_set) = (int *)malloc(sizeof(int)*num_elem);
  (*offset)[0] = 0;

  elem = elem_list->elem;
  for (i=0;i<num_elem;i++) {
    (*adj_entity_handles)[3*i] = (void *)elem->vtx[0]->entity_handle;
    (*adj_entity_handles)[3*i+1] = (void *)elem->vtx[1]->entity_handle;
    (*adj_entity_handles)[3*i+2] = (void *)elem->vtx[2]->entity_handle;
    (*offset)[i+1] = (*offset)[i]+3;
    (*in_entity_set)[i] = 1;
    elem_list = elem_list->next;
    elem = elem_list->elem;
  }
}

#undef __FUNC__
#define __FUNC__ "TSTT_getAdjEntities"
void TSTT_getAdjEntities(void *entity_set1,
                         EntityType entity_type_requestor, 
                         EntityTopology entity_topology_requestor,
                         EntityType entity_type_requested,
                         void ***adj_entity_handles, 
                         int *num_adj_entities,
                         int **offset,
                         int *offset_size,
                         int **in_entity_set,
                         int *in_entity_set_size,Error *err)
{
  int num_elem;
  Elem_list *elem_list;
  Elem *elem;
  int i;
  EntityHandle *entity_handle;
  int num_entities;
  EntitySet *entity_set;

  entity_set = (EntitySet *)entity_set1;
    
  CHK_NULL(entity_set,err,"entity set");
  CHK_SUPPORTED_TYPE(entity_type_requestor,err);
  CHK_SUPPORTED_TOPO(entity_topology_requestor,err);
  CHK_SUPPORTED_TYPE(entity_type_requested,err);
  CHK_TYPE_TOPO_COMPATIBILITY(entity_type_requestor,entity_topology_requestor,err);
  CHK_RTN(err);

  if (entity_type_requested != POINT) {
    err->code = 1;
    sprintf(err->msg,"Do not adjacency information for requested type");
    return;
  }

  num_entities = entity_set->num_elem;
  (*adj_entity_handles) = (void **)malloc(sizeof(void *)*num_entities*3);
  (*num_adj_entities) = 0;
  (*offset) = (int *)malloc(sizeof(int)*(num_entities+1));
  (*offset_size)=num_entities+1;
  (*in_entity_set) = (int *)malloc(sizeof(int)*num_entities*3);
  (*in_entity_set_size)=num_entities*3;


  (*offset)[0] = 0;

  elem_list = entity_set->elem_list;
  for (i=0;i<num_entities;i++) {
    elem = elem_list->elem;
    (*adj_entity_handles)[3*i] = (void *)elem->vtx[0]->entity_handle;
    (*adj_entity_handles)[3*i+1] = (void *)elem->vtx[1]->entity_handle;
    (*adj_entity_handles)[3*i+2] = (void *)elem->vtx[2]->entity_handle;
    (*offset)[i+1] = (*offset)[i]+3;
    (*in_entity_set)[i]=1;
    (*num_adj_entities) += 3;
    elem_list = elem_list->next;
  }
}

#undef __FUNC__
#define __FUNC__ "TSTT_getDfltStorage"
StorageOrder TSTT_getDfltStorage(void *mesh1, Error *err)
{
  CHK_NULL(mesh1,err,"mesh"); 
  CHK_RTNV(err);
  mesh =(EntitySet *)mesh1;
  return(INTERLEAVED);
  
}

#undef __FUNC__
#define __FUNC__ "TSTT_getAdjTable"
void TSTT_getAdjTable(void *mesh1, AdjacencyCost **table, int *table_size, Error *err)
{

  CHK_NULL(mesh1,err,"mesh"); 
  CHK_RTN(err);

  mesh =(EntitySet *)mesh1;
  /* the only adjacency supported at this time is elements point to vertices */
  (*table) = (AdjacencyCost *)malloc(sizeof(AdjacencyCost)*16);
  (*table)[0] = IMMEDIATE;  
  (*table)[1] = UNAVAILABLE;  
  (*table)[2] = UNAVAILABLE;  
  (*table)[3] = UNAVAILABLE;
  (*table)[4] = UNAVAILABLE;  
  (*table)[5] = IMMEDIATE;  
  (*table)[6] = UNAVAILABLE;  
  (*table)[7] = UNAVAILABLE;
  (*table)[8] = UNAVAILABLE;  
  (*table)[9] = UNAVAILABLE;  
  (*table)[10]= IMMEDIATE;  
  (*table)[11]= UNAVAILABLE;
  (*table)[12]= IMMEDIATE;  
  (*table)[13]= UNAVAILABLE;  
  (*table)[14]= UNAVAILABLE;  
  (*table)[15]= IMMEDIATE;
  *table_size = 16;
}

#undef __FUNC__
#define __FUNC__ "TSTT_getEntArrTopo"
void TSTT_getEntArrTopo(void **entity_handles1, int num_entities,
		     EntityTopology **topology, int *num_topo, Error *err)
{
  int i;
  EntityHandle **entity_handles;
  EntityHandle *entity_handle;
 
  CHK_NULL(entity_handles1,err,"entity handles"); 
  CHK_RTN(err);
  entity_handles = (EntityHandle **)entity_handles1;

  (*topology) = (EntityTopology *)malloc(sizeof(EntityTopology)*num_entities);
  for (i=0;i<num_entities;i++) {
    entity_handle = (EntityHandle *) entity_handles[i];
    (*topology)[i] = entity_handle->entity_topology;
  }   
  (*num_topo) = num_entities;
}

#undef __FUNC__
#define __FUNC__ "TSTT_getEntArrType"
void TSTT_getEntArrType(void **entity_handles1, int num_entities,
                                        EntityType **type, int *num_type, Error *err)
{
  int i;
  EntityHandle **entity_handles;
  EntityHandle *entity_handle;
 
  CHK_NULL(entity_handles1,err,"entity handles"); 
  CHK_RTN(err);
  entity_handles = (EntityHandle **)entity_handles1;

  (*type) = (EntityType *)malloc(sizeof(EntityType)*num_entities);
  for (i=0;i<num_entities;i++) {
    entity_handle = (EntityHandle *) entity_handles[i];
    (*type)[i] = entity_handle->entity_type;
  }   
  (*num_type) = num_entities;
}


#undef __FUNC__
#define __FUNC__ "TSTT_getAdjEntities"
void TSTT_getEntArrAdj(void **entity_handles1, int num_entities,
                         EntityType entity_type_requested,
                         void ***adj_entity_handles, 
                         int *num_adj_entities,
                         int **offset,
                         int *offset_size,
                         Error *err)
{
  int num_elem;
  Elem_list *elem_list;
  Elem *elem;
  int i;
  EntityHandle *entity_handle;
  EntitySet *entity_set;

  CHK_NULL(entity_handles1,err,"entity handles");
  CHK_SUPPORTED_TYPE(entity_type_requested,err);
  CHK_RTN(err);

  if (entity_type_requested != POINT) {
    err->code = 1;
    sprintf(err->msg,"Do not adjacency information for requested type");
    return;
  }

  (*adj_entity_handles) = (void **)malloc(sizeof(void *)*num_entities*3);
  (*num_adj_entities) = 0;
  (*offset) = (int *)malloc(sizeof(int)*(num_entities+1));
  (*offset_size)=num_entities+1;
  (*offset)[0] = 0;

  for (i=0;i<num_entities;i++) {
    entity_handle = (EntityHandle *)entity_handles1[i];
    elem = (Elem *)entity_handle->entity_ptr;
    (*adj_entity_handles)[3*i] = (void *)elem->vtx[0]->entity_handle;
    (*adj_entity_handles)[3*i+1] = (void *)elem->vtx[1]->entity_handle;
    (*adj_entity_handles)[3*i+2] = (void *)elem->vtx[2]->entity_handle;
    (*offset)[i+1] = (*offset)[i]+3;
    (*num_adj_entities) += 3;
  }
}

#undef __FUNC__
#define __FUNC__ "TSTT_getEntTopo"
EntityTopology TSTT_getEntTopo(void *entity_handle1,
		     Error *err)
{
  EntityHandle *entity_handle;
 
  CHK_NULL(entity_handle1,err,"entity handle"); 
  CHK_RTN(err);
  entity_handle = (EntityHandle *)entity_handle1;

  return(entity_handle->entity_topology);
}

#undef __FUNC__
#define __FUNC__ "TSTT_getEntType"
EntityType TSTT_getEntType(void *entity_handle1,
                            Error *err)
{
  EntityHandle *entity_handle;
 
  CHK_NULL(entity_handle1,err,"entity handle"); 
  CHK_RTN(err);
  entity_handle = (EntityHandle *)entity_handle1;

  return(entity_handle->entity_type);
}

#undef __FUNC__
#define __FUNC__ "TSTT_getVtxCoord"
void TSTT_getVtxCoord(void *entity_handle1, double **coords, int *coords_size, 
        Error *err)
{
  EntityHandle *entity_handle;
  Vtx *vtx;

  CHK_NULL(entity_handle1,err,"entity handle"); 
  CHK_RTN(err);

  entity_handle = (EntityHandle *)entity_handle1;

  (*coords_size) = mesh->dimension;
  (*coords) = (double *) malloc(sizeof(double)*(*coords_size)); 
  if (entity_handle->entity_type != VERTEX) {
    err->code = 1;
    sprintf(err->msg,"Must pass in a vertex in getVtxCoords\n");
  }
  
  vtx = (Vtx *)entity_handle->entity_ptr;
  (*coords)[0] = vtx->coord[0];
  (*coords)[1] = vtx->coord[1];
}

#undef __FUNC__
#define __FUNC__ "TSTT_getEntAdj"
void TSTT_getEntAdj(void *entity_handle1, 
                         EntityType entity_type_requested,
                         void ***adj_entity_handles, 
                         int *num_adj_entities,
                         Error *err)
{
  int num_elem;
  Elem_list *elem_list;
  Elem *elem;
  int i;
  EntityHandle *entity_handle;
  EntitySet *entity_set;

  CHK_NULL(entity_handle1,err,"entity handle");
  CHK_SUPPORTED_TYPE(entity_type_requested,err);
  CHK_RTN(err);

  if (entity_type_requested != POINT) {
    err->code = 1;
    sprintf(err->msg,"Do not adjacency information for requested type");
    return;
  }

  (*adj_entity_handles) = (void **)malloc(sizeof(void *)*3);
  (*num_adj_entities) = 3;

  entity_handle = (EntityHandle *)entity_handle1;
  elem = (Elem *)entity_handle->entity_ptr;
  (*adj_entity_handles)[0] = (void *)elem->vtx[0]->entity_handle;
  (*adj_entity_handles)[1] = (void *)elem->vtx[1]->entity_handle;
  (*adj_entity_handles)[2] = (void *)elem->vtx[2]->entity_handle;
}

#undef __FUNC__
#define __FUNC__ "TSTT_initEntArrIter"
int TSTT_initEntArrIter(void *entity_set1,
                      EntityType requested_entity_type, 
                      EntityTopology requested_entity_topology,
                      int requested_workset_size,
                      void **workset_iterator1, Error *err)
{
  Workset_Iterator *workset_iterator;
  EntitySet *entity_set;
  int num_entities;
  
  CHK_NULL(entity_set1,err,"entity set"); 
  CHK_SUPPORTED_TYPE(requested_entity_type,err);
  CHK_SUPPORTED_TOPO(requested_entity_topology,err);
  CHK_TYPE_TOPO_COMPATIBILITY(requested_entity_type,requested_entity_topology,err);
  CHK_RTN(err);

  entity_set = (EntitySet *)entity_set1;

  workset_iterator = (Workset_Iterator *)malloc(sizeof(Workset_Iterator));
  workset_iterator->entity_type = requested_entity_type;
  workset_iterator->entity_topology = requested_entity_topology;
  workset_iterator->requested_workset_size = requested_workset_size;

  switch (requested_entity_type) {
  case VERTEX:
    workset_iterator->list = (void *)entity_set->vtx_list;
    num_entities = entity_set->num_vtx;
    break;
  case FACE:
    workset_iterator->list = (void *)entity_set->elem_list;
    num_entities = entity_set->num_elem;
    break;
  }
  workset_iterator->list_start = workset_iterator->list;
  (*workset_iterator1) = (void *)workset_iterator;

  return(num_entities);
}

#undef __FUNC__
#define __FUNC__ "TSTT_getEntArrNextIter"
int TSTT_getEntArrNextIter(void *workset_iterator1, void ***entity_handles, 
                     int *num_entities, Error *err)
{
  Workset_Iterator *workset_iterator;
  Vtx_list *vtx_list;
  Elem_list *elem_list;
  int i;
  int done;

  CHK_NULL(workset_iterator1,err,"workset iterator"); 
  CHK_RTNV(err);
  workset_iterator = (Workset_Iterator *)workset_iterator1;
  
  if ((*entity_handles) == NULL) {
    /* malloc the entity_handle array */
    (*entity_handles) = (void **)malloc(sizeof(void *)*workset_iterator->requested_workset_size);
  }
  
  (*num_entities)=0;
  if (workset_iterator->entity_type == VERTEX) {
    vtx_list = (Vtx_list *)workset_iterator->list;
    for (i=0;i<workset_iterator->requested_workset_size;i++) {
      if (vtx_list->vtx!=NULL) {
        (*entity_handles)[i] = (void *)vtx_list->vtx->entity_handle;
        vtx_list = vtx_list->next;
        (*num_entities)++;
      }
    }
    workset_iterator->list = (void *)vtx_list;
  } else if (workset_iterator->entity_type == FACE) {
    elem_list = (Elem_list *)workset_iterator->list;
    for (i=0;i<workset_iterator->requested_workset_size;i++) {
      if (elem_list->elem!=NULL) {
        (*entity_handles)[i] = (void *)elem_list->elem->entity_handle;
        elem_list = elem_list->next;
        (*num_entities)++;
      }
    }
    workset_iterator->list = (void *)elem_list;
  }
  if ((*num_entities)<workset_iterator->requested_workset_size) {
    done = 1;
  } else { 
    done =0 ; 
  }
  return(done);
}

#undef __FUNC__
#define __FUNC__ "TSTT_resetEntArrIter"
void TSTT_resetEntArrIter(void *workset_iterator1, Error *err)
{
  Workset_Iterator *workset_iterator;
  CHK_NULL(workset_iterator1,err,"workset iterator"); 
  workset_iterator = (Workset_Iterator *)workset_iterator1;
  workset_iterator->list = workset_iterator->list_start;
}

#undef __FUNC__
#define __FUNC__ "TSTT_endEntArrIter"
void TSTT_endEntArrIter(void *workset_iterator1, Error *err)
{
  Workset_Iterator *workset_iterator;
  CHK_NULL(workset_iterator1,err,"workset iterator"); 
  CHK_RTN(err);
  workset_iterator = (Workset_Iterator *)workset_iterator1;
  free(workset_iterator); 
}


#undef __FUNC__
#define __FUNC__ "TSTT_initEntIter"
int TSTT_initEntIter(void *entity_set1,
                      EntityType requested_entity_type, 
                      EntityTopology requested_entity_topology,
                      void **iterator1, Error *err)
{
  Iterator *iterator;
  EntitySet *entity_set;
  int num_entities;
  
  CHK_NULL(entity_set1,err,"entity set"); 
  CHK_SUPPORTED_TYPE(requested_entity_type,err);
  CHK_SUPPORTED_TOPO(requested_entity_topology,err);
  CHK_TYPE_TOPO_COMPATIBILITY(requested_entity_type,requested_entity_topology,err);
  CHK_RTN(err);

  entity_set = (EntitySet *)entity_set1;

  iterator = (Iterator *)malloc(sizeof(Iterator));
  iterator->entity_type = requested_entity_type;
  iterator->entity_topology = requested_entity_topology;

  switch (requested_entity_type) {
  case VERTEX:
    iterator->list = (void *)entity_set->vtx_list;
    num_entities = entity_set->num_vtx;
    break;
  case FACE:
    iterator->list = (void *)entity_set->elem_list;
    num_entities = entity_set->num_elem;
    break;
  }
  iterator->list_start = iterator->list;
  (*iterator1) = (void *)iterator;

  return(num_entities);
}

#undef __FUNC__
#define __FUNC__ "TSTT_getEntNextIter"
int TSTT_getEntNextIter(void *iterator1, void **entity_handle, 
                       Error *err)
{
  Iterator *iterator;
  Vtx_list *vtx_list;
  Elem_list *elem_list;
  int i;
  int done;

  CHK_NULL(iterator1,err,"iterator"); 
  CHK_RTNV(err);
  iterator = (Iterator *)iterator1;
  
  if (iterator->entity_type == VERTEX) {

    vtx_list = (Vtx_list *)iterator->list;
    if (vtx_list->vtx!=NULL) {
      (*entity_handle) = (void *)vtx_list->vtx->entity_handle;
      iterator->list = (void *)(vtx_list->next);
      done=0;
    } else {
      done=1;
    }

  } else if (iterator->entity_type == FACE) {

    elem_list = (Elem_list *)iterator->list;
    if (elem_list->elem!=NULL) {
      (*entity_handle) = (void *)elem_list->elem->entity_handle;
      iterator->list = (void *)(elem_list->next);
      done=0;
    } else {
      done=1;
    }
  }
  return(done);
}

#undef __FUNC__
#define __FUNC__ "TSTT_resetEntIter"
void TSTT_resetEntIter(void *iterator1, Error *err)
{
  Iterator *iterator;
  CHK_NULL(iterator1,err,"iterator"); 
  CHK_RTN(err);

  iterator = (Iterator *)iterator1;
  iterator->list = iterator->list_start;
}

#undef __FUNC__
#define __FUNC__ "TSTT_endEntIter"
void TSTT_endEntIter(void *iterator1, Error *err)
{
  Iterator *iterator;
  CHK_NULL(iterator1,err,"iterator"); 
  CHK_RTN(err);

  iterator = (Iterator *)iterator1;
  free(iterator);
}

