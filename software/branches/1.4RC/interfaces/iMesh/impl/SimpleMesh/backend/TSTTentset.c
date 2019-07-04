#include <stdio.h>
#include <string.h>
#include "mesh.h"

void print_entity_set(void *entity_set1);

#undef __FUNC__
#define __FUNC__ "TSTT_createEntSet"
void TSTT_createEntSet(int isList, void **entity_set1, Error *err)
{
  EntitySet *entity_set;

  /* initialize all the entity set data */
  entity_set = (EntitySet *)malloc(sizeof(EntitySet));
  entity_set->num_vtx = 0;
  entity_set->vtx_list = (Vtx_list *)malloc(sizeof(Vtx_list));
  entity_set->vtx_list->vtx = NULL;
  entity_set->num_elem = 0;
  entity_set->elem_list = (Elem_list *)malloc(sizeof(Elem_list));
  entity_set->elem_list->elem = NULL;
  entity_set->num_entitysets = 0;
  entity_set->entity_set_list = (EntitySet_list *)malloc(sizeof(EntitySet_list));
  entity_set->entity_set_list->entity_set = NULL;

  entity_set->num_tags = 0;
  entity_set->num_int_tags = 0;
  entity_set->num_dbl_tags = 0;
  entity_set->tag_instance_list = (Tag_Instance_list *)malloc(sizeof(Tag_Instance_list));
  entity_set->tag_instance_list->tag_instance = NULL;
  entity_set->int_tag_instance_list = (Int_Tag_Instance_list *)malloc(sizeof(Int_Tag_Instance_list));
  entity_set->int_tag_instance_list->tag_instance = NULL;
  entity_set->dbl_tag_instance_list = (Dbl_Tag_Instance_list *)malloc(sizeof(Dbl_Tag_Instance_list));
  entity_set->dbl_tag_instance_list->tag_instance = NULL;

  entity_set->isList = isList;

  (*entity_set1) = (void *)entity_set;
}

#undef __FUNC__
#define __FUNC__ "TSTT_destroyEntSet"
void TSTT_destroyEntSet(void *entity_set1, Error *err) 
{
  EntitySet *entity_set;
  Tag_Instance_list *tag_instance_list, *tag_tmp;
  Vtx_list *vtx_list, *vtx_tmp;
  int i;

  entity_set = (EntitySet *)entity_set1;

  /* delete the tags and remove one instance counter from the parent */
  tag_instance_list = entity_set->tag_instance_list;
  for (i=0;i<entity_set->num_tags;i++) {
    tag_tmp = tag_instance_list->next;
    free(tag_instance_list);
    tag_instance_list = tag_tmp;
  }
  free(tag_tmp);

  /* delete the entities */
  vtx_list = entity_set->vtx_list;
  for (i=0;i<entity_set->num_vtx;i++) {
    vtx_tmp = vtx_list->next;
    free(vtx_list);
    vtx_list = vtx_tmp;
  }
  free(vtx_tmp);
}

#undef __FUNC__
#define __FUNC__ "TSTT_isList"
int TSTT_isList(void *entity_set1, Error *err)
{
  return(((EntitySet *)entity_set1)->isList);
}

#undef __FUNC__
#define __FUNC__ "TSTT_addEntToSet"
void TSTT_addEntToSet(void *entity_handle1, void *entity_set1, Error *err)
{
  EntitySet *entity_set;
  EntityHandle *entity_handle;
  Vtx_list *vtx_list;
  Elem_list *elem_list;

  entity_set = (EntitySet *)entity_set1;
  entity_handle = (EntityHandle *)entity_handle1;

  if (entity_handle->entity_type == VERTEX) {
    entity_set->num_vtx++;
    vtx_list = (Vtx_list *) malloc(sizeof(Vtx_list));
    vtx_list->vtx = (Vtx *)entity_handle->entity_ptr;
    ADD_TO_LIST(entity_set->vtx_list, vtx_list);

  } else if (entity_handle->entity_type == FACE) {
    entity_set->num_elem++;
    elem_list = (Elem_list *) malloc(sizeof(Elem_list));
    elem_list->elem = (Elem *)entity_handle->entity_ptr;
    ADD_TO_LIST(entity_set->elem_list, elem_list);
  }
}

void print_entity_set(void *entity_set1)
{
  EntitySet *entity_set;
  entity_set = (EntitySet *)entity_set1;
  printf("Entity Set:  \n");
  printf("   num_vtx = %d\n",entity_set->num_vtx);
  printf("   num_elem = %d\n",entity_set->num_elem);
  printf("   num_ent_sets = %d\n",entity_set->num_entitysets);
  printf("   num_tags = %d\n",entity_set->num_tags);
  printf("   num_int_tags = %d\n",entity_set->num_int_tags);
  printf("   num_dbl_tags = %d\n",entity_set->num_dbl_tags);

}
