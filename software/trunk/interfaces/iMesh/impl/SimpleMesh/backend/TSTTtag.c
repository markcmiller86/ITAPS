#define DO_TIMING
 
#include <stdio.h>
#include <string.h>
#include "mesh.h"

void print_tag(Tag *tag);
void print_tag_repository();

int RepositoryCreated = 0;
TagRepository *tag_repository;

#undef __FUNC__
#define __FUNC__ "TSTT_createTag"
void TSTT_createTag(char *name, int size, TagValueType tag_type,
                    void **tag_handle, Error *err)
{
  Tag *tag;
  Tag_list *tag_list;
  Tag_list *repository_tag_list;
  Tag *repository_tag;
  int i;

  if (RepositoryCreated == 0) {
    /* create repository */
    tag_repository = (TagRepository *)malloc(sizeof(TagRepository));
    tag_repository->tag_list = (Tag_list *)malloc(sizeof(Tag_list));
    tag_repository->num_tags = 0;
    tag_repository->next_ID=0;
    tag_repository->tag_list->tag = NULL;    
    RepositoryCreated=1;
  }

  /* check that the name is unique */
  repository_tag_list = tag_repository->tag_list;
  for (i=0;i<tag_repository->num_tags;i++) {
    repository_tag = repository_tag_list->tag;
    if (strcmp(name, repository_tag->name)==0) {
      err->code = 1;
      sprintf(err->msg,"Tag name %s not unique; tag not created",name);
      return;
    }
    repository_tag_list = repository_tag_list->next;
  }

  /* create tag */
  tag = (Tag *)malloc(sizeof(Tag));
  tag->id = tag_repository->next_ID;
  sprintf(tag->name,name);
  tag->size = size;
  tag->tag_type = tag_type;
  tag->num_entities = 0;
  tag->num_entity_sets = 0;
  tag->num_mesh = 0;

  /* update the repository */
  tag_repository->num_tags++;
  tag_repository->next_ID++;
  tag_list = (Tag_list *)malloc(sizeof(Tag_list));
  tag_list->tag=tag;
  ADD_TO_LIST(tag_repository->tag_list,tag_list);

  /*  print_tag(tag); 
      print_tag_repository();  */

  (*tag_handle) = (void *)tag;  
}

#undef __FUNC__
#define __FUNC__ "print_tag"
void print_tag(Tag *tag)
{
  printf("Tag Name = %s\n",tag->name);
  printf("  Tag ID = %d \n",tag->id);
  printf("  Tag size = %d\n",tag->size);
  if (  (strcmp(tag->name,"Boundary")==0)
     || (strcmp(tag->name,"Elem_ID")==0) 
     || (strcmp(tag->name,"int_tag")==0) 
     || (strcmp(tag->name,"User_Data")==0) 
     || (strcmp(tag->name,"Vtx_ID")==0)  ) {
  } else if (  (strcmp(tag->name,"Area")==0)
            || (strcmp(tag->name,"double_tag")==0)  ) {
  }
  printf("  Tag num_entities = %d\n",tag->num_entities);
  printf("  Tag num_entity_sets = %d\n",tag->num_entity_sets);
  printf("  Tag num_mesh = %d\n",tag->num_mesh);
}

#undef __FUNC__
#define __FUNC__ "print_tag_repository"
void print_tag_repository()
{
  int i;
  Tag *tag;
  Tag_list *tag_list;
  printf("Tag Repository\n");

  tag_list=tag_repository->tag_list;
  for (i=0;i<tag_repository->num_tags;i++) {
    tag = tag_list->tag;
    printf("  Tag Name: %s, ID: %d\n",tag->name,tag->id);
    tag_list=tag_list->next;
  }
}


#undef __FUNC__
#define __FUNC__ "TSTT_destroyTag"
void TSTT_destroyTag(void *tag_handle, int forced, Error *err)
{
  int i;
  Tag_list *repository_tag_list;
  Tag_list *prev_tag_list;
  Tag *tag_to_delete;
  Tag *tag;

  CHK_NULL(tag_handle,err,"tag_handle");

  /*  print_tag_repository(); */

  tag_to_delete = (Tag *)tag_handle;

  repository_tag_list = tag_repository->tag_list;
  for (i=0;i<tag_repository->num_tags;i++) {
    tag = repository_tag_list->tag;
    if (tag->id == tag_to_delete->id) {
      /* remove the tag from the repository list */
      if (i!=0) {
        prev_tag_list->next = repository_tag_list->next;
      } else {
        tag_repository->tag_list = repository_tag_list->next;
      }
      tag_repository->num_tags--;
      free(repository_tag_list->tag);
      free(repository_tag_list);
      /*      print_tag_repository(); */
      return;
    }
    /* check the next element in the list */
    prev_tag_list = repository_tag_list;
    repository_tag_list = repository_tag_list->next; 
  }

}

#undef __FUNC__
#define __FUNC__ "TSTT_getTagName"
char* TSTT_getTagName(void *tag_handle, Error *err)
{
  CHK_NULL(tag_handle,err,"tag_handle");
  return(((Tag *)tag_handle)->name);
}

#undef __FUNC__
#define __FUNC__ "TSTT_getTagSize"
int TSTT_getTagSize(void *tag_handle, Error *err)
{
  CHK_NULL(tag_handle,err,"tag_handle");
  return(((Tag *)tag_handle)->size);
}


#undef __FUNC__
#define __FUNC__ "TSTT_getTagHandle"
void *TSTT_getTagHandle(char *name, Error *err)
{
  Tag_list *repository_tag_list;
  Tag *tag;
  int i;

  /* check that the name is unique */
  repository_tag_list = tag_repository->tag_list;
  for (i=0;i<tag_repository->num_tags;i++) {
    tag = repository_tag_list->tag;
    if (strcmp(name, tag->name)==0) {
      return((void *)tag);
    }
    repository_tag_list = repository_tag_list->next; 
  }

  err->code = TAG_NOT_FOUND;
  sprintf(err->msg,"Tag not found");
  return(0);
}

#undef __FUNC__
#define __FUNC__ "TSTT_getTagType"
TagValueType TSTT_getTagType(void *tag_handle, Error *err)
{
  CHK_NULL(tag_handle,err,"tag_handle");
  return(((Tag *)tag_handle)->tag_type);
}


#undef __FUNC__
#define __FUNC__ "TSTT_rmvTag"
void TSTT_rmvTag(void *entity_handle1, void *tag_handle1, Error *err)
{
  EntityHandle *entity_handle;
  Tag_Instance_list *tag_instance_list;
  Tag_Instance_list *prev_tag_instance_list;
  Tag *tag_to_remove;
  int i,j,k;
  Entity_list *entity_list;
  Entity_list *prev_entity_list;
  Vtx *vtx1, *vtx2;
  Elem *elem1, *elem2;
  int entity_found;
  EntityType entity_type;

  CHK_NULL(tag_handle1,err,"tag_handle");
  CHK_NULL(entity_handle1,err,"entity_handle");

  tag_to_remove = (Tag *)tag_handle1;

  entity_handle = (EntityHandle *)entity_handle1;
  tag_instance_list = entity_handle->tag_instance_list;
  for (i=0;i<entity_handle->num_tags;i++) {

      /* remove the tag instance from each entity */
      if (tag_to_remove->id == tag_instance_list->tag_instance->parent_tag->id) {
        if (i!=0) {
          prev_tag_instance_list->next = tag_instance_list->next;
        } else {
          entity_handle->tag_instance_list = tag_instance_list->next;
        }
        entity_handle->num_tags--;
        free(tag_instance_list->tag_instance);
        free(tag_instance_list);

	/* remove the entity reference from the parent tag */
        entity_type = entity_handle->entity_type;
        entity_list = tag_to_remove->entity_list;
        for (k=0;k<tag_to_remove->num_entities;k++) {
          entity_found = 0;
          if (entity_list->entity->entity_type == entity_type) {
            if (entity_type == VERTEX) {
              vtx1 = (Vtx *)entity_handle->entity_ptr;
              vtx2 = (Vtx *)entity_list->entity->entity_ptr;
              if ( vtx1->id == vtx2->id) {
                entity_found = 1;
              }
	    } else if (entity_type == FACE) {
              elem1 = (Elem *)entity_handle->entity_ptr;
              elem2 = (Elem *)entity_list->entity->entity_ptr;
              if (elem1->id == elem2->id) {
                entity_found = 1;
              }
            }
          }
	  if (entity_found) {
            if (k!=0) {
              prev_entity_list->next = entity_list->next;
            } else {
              tag_to_remove->entity_list = entity_list->next;
            }
            tag_to_remove->num_entities--;
            free(entity_list);
	  }
          prev_entity_list = entity_list;
          entity_list = entity_list->next;
        }
      }
      /* check the next element in the list */
      prev_tag_instance_list = tag_instance_list;
      tag_instance_list = tag_instance_list->next; 
    }
}


#undef __FUNC__
#define __FUNC__ "TSTT_rmvArrTag"
void TSTT_rmvArrTag(void **entity_handles, int num_entities,
                          void *tag_handle, Error *err)
{
  int j;
  CHK_NULL(tag_handle,err,"tag_handle");
  CHK_NULL(entity_handles,err,"entity_handle");
  for (j=0;j<num_entities;j++) {
    TSTT_rmvTag(entity_handles[j],tag_handle,err);
  }
}

#undef __FUNC__
#define __FUNC__ "TSTT_getAllTags"
void TSTT_getAllTags(void *entity_handle1, void ***tag_handles1, 
	  	     int *num_tags, Error *err)
{
  EntityHandle *entity_handle;
  Tag_Instance_list *tag_instance_list;
  Int_Tag_Instance_list *int_tag_instance_list;
  Dbl_Tag_Instance_list *dbl_tag_instance_list;
  Tag **tag_handles;
  int i;
  int tag_count;

  CHK_NULL(entity_handle1,err,"entity_handle");

  entity_handle = (EntityHandle *)entity_handle1;
  (*num_tags) = entity_handle->num_tags +
                entity_handle->num_int_tags +
                entity_handle->num_dbl_tags;

  tag_handles = (Tag **)malloc(sizeof(Tag *)*(*num_tags));
  (*tag_handles1) = (void **)malloc(sizeof(void *)*(*num_tags));

  tag_count = 0;

  tag_instance_list = entity_handle->tag_instance_list;
  for (i=0;i<entity_handle->num_tags;i++) {
    tag_handles[tag_count] = tag_instance_list->tag_instance->parent_tag;
    (*tag_handles1)[tag_count] = (void *)tag_handles[tag_count];
    tag_instance_list = tag_instance_list->next;
    tag_count++;
  }

  int_tag_instance_list = entity_handle->int_tag_instance_list;
  for (i=0;i<entity_handle->num_int_tags;i++) {
    tag_handles[tag_count] = int_tag_instance_list->tag_instance->parent_tag;
    (*tag_handles1)[tag_count] = (void *)tag_handles[tag_count];
    int_tag_instance_list = int_tag_instance_list->next;
    tag_count++;
  }

  dbl_tag_instance_list = entity_handle->dbl_tag_instance_list;
  for (i=0;i<entity_handle->num_dbl_tags;i++) {
    tag_handles[tag_count] = dbl_tag_instance_list->tag_instance->parent_tag;
    (*tag_handles1)[tag_count] = (void *)tag_handles[tag_count];
    dbl_tag_instance_list = dbl_tag_instance_list->next;
    tag_count++;
  }

}

#undef __FUNC__
#define __FUNC__ "TSTT_getData"
void TSTT_getData(void *entity_handle1, void *tag_handle1,
                  void **tag_value, int *tag_size, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  Tag_Instance_list *tag_instance_list;
  int j;

  CHK_NULL(entity_handle1,err,"entity_handles");
  CHK_NULL(tag_handle1,err,"tag_handles");
  tag_handle  = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,OPAQUE,err);

  entity_handle = (EntityHandle *)entity_handle1;

  (*tag_size) = tag_handle->size;
   tag_instance_list = entity_handle->tag_instance_list;
   for (j=0;j<entity_handle->num_tags;j++) {
      if (tag_instance_list->tag_instance->parent_tag->id == tag_handle->id) {
	(*tag_value) = tag_instance_list->tag_instance->value;
      }
      tag_instance_list = tag_instance_list->next;
   }
}

#undef __FUNC__
#define __FUNC__ "TSTT_getIntData"
int TSTT_getIntData( void *entity_handle1, void *tag_handle1, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  Int_Tag_Instance_list *int_tag_instance_list;
  int j;

  CHK_NULL(entity_handle1,err,"entity_handles");
  CHK_NULL(tag_handle1,err,"tag_handle");
  tag_handle  = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,INTEGER,err);

  entity_handle = (EntityHandle *)entity_handle1;

  int_tag_instance_list = entity_handle->int_tag_instance_list;
  for (j=0;j<entity_handle->num_int_tags;j++) {
     if (int_tag_instance_list->tag_instance->parent_tag->id == tag_handle->id) {
        return(int_tag_instance_list->tag_instance->value);
      }
      int_tag_instance_list = int_tag_instance_list->next;
   }
}

#undef __FUNC__
#define __FUNC__ "TSTT_getDblData"
double TSTT_getDblData( void *entity_handle1, void *tag_handle1, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  Dbl_Tag_Instance_list *dbl_tag_instance_list;
  int j;

  CHK_NULL(entity_handle1,err,"entity_handles");
  CHK_NULL(tag_handle1,err,"tag_handles");
  tag_handle  = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,DOUBLE,err);

  entity_handle = (EntityHandle *)entity_handle1;

  dbl_tag_instance_list = entity_handle->dbl_tag_instance_list;
  for (j=0;j<entity_handle->num_dbl_tags;j++) {
     if (dbl_tag_instance_list->tag_instance->parent_tag->id == tag_handle->id) {
        return(dbl_tag_instance_list->tag_instance->value);
      }
      dbl_tag_instance_list = dbl_tag_instance_list->next;
   }
}

#undef __FUNC__
#define __FUNC__ "TSTT_getEHData"
void **TSTT_getEHData(void *entity_handle1, void *tag_handle1, Error *err)
{
  void *tag_value;
  int tag_size;

  /* not tested*/

  TSTT_getData(entity_handle1, tag_handle1, &tag_value, &tag_size, err);
  return(tag_value);
}

#undef __FUNC__
#define __FUNC__ "TSTT_getArrData"
void TSTT_getArrData(void **entity_handles, int num_entities, void *tag_handle1,
                     void **tag_values, int *tag_value_size, int *tag_size, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  Tag_Instance_list *tag_instance_list;
  int i,j;


  CHK_NULL(entity_handles,err,"entity_handles");
  CHK_NULL(tag_handle1,err,"tag_handles");
  tag_handle  = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,OPAQUE,err);

  (*tag_size) = tag_handle->size;

  for (i=0;i<num_entities;i++) {
    entity_handle = (EntityHandle *)entity_handles[i];

    /* find the right tag */
    tag_instance_list = entity_handle->tag_instance_list;
    for (j=0;j<entity_handle->num_tags;j++) {
      if (tag_instance_list->tag_instance->parent_tag->id == tag_handle->id) {
	tag_values[i] = tag_instance_list->tag_instance->value;
      }
      tag_instance_list = tag_instance_list->next;
    }
  }
  (*tag_value_size)=num_entities;
}

#undef __FUNC__
#define __FUNC__ "TSTT_getIntArrData"
void TSTT_getIntArrData(void **entity_handles1, int num_entities,
                        void *tag_handle1, int **tag_values,
                        int *tag_value_size, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  Int_Tag_Instance_list *tag_instance_list;
  int i,j;

  CHK_NULL(entity_handles1,err,"entity_handles");
  CHK_NULL(tag_handle1,err,"tag_handles");

  tag_handle  = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,INTEGER,err);

  for (i=0;i<num_entities;i++) {
    entity_handle = (EntityHandle *)entity_handles1[i];

    /* find the right tag */
    tag_instance_list = entity_handle->int_tag_instance_list;
    for (j=0;j<entity_handle->num_tags;j++) {
      if (tag_instance_list->tag_instance->parent_tag->id == tag_handle->id) {
	(*tag_values)[i] = tag_instance_list->tag_instance->value;
      }
      tag_instance_list = tag_instance_list->next;
    }
  }
  (*tag_value_size)=num_entities;
}


#undef __FUNC__
#define __FUNC__ "TSTT_getDblArrData"
void TSTT_getDblArrData(void **entity_handles1, int num_entities,
                        void *tag_handle1, double **tag_values,
                        int *tag_value_size, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  Dbl_Tag_Instance_list *tag_instance_list;
  int i,j;

  CHK_NULL(entity_handles1,err,"entity_handles");
  CHK_NULL(tag_handle1,err,"tag_handles");

  tag_handle  = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,DOUBLE,err);

  for (i=0;i<num_entities;i++) {
    entity_handle = (EntityHandle *)entity_handles1[i];

    /* find the right tag */
    tag_instance_list = entity_handle->dbl_tag_instance_list;
    for (j=0;j<entity_handle->num_tags;j++) {
      if (tag_instance_list->tag_instance->parent_tag->id == tag_handle->id) {
	(*tag_values)[i] = tag_instance_list->tag_instance->value;
      }
      tag_instance_list = tag_instance_list->next;
    }
  }
  (*tag_value_size)=num_entities;
}

#undef __FUNC__
#define __FUNC__ "TSTT_getEHArrData"
void TSTT_getEHArrData(void **entity_handles, int num_entities, void *tag_handle1,
                     void **tag_values, int *tag_value_size, Error *err)
{
  int tag_size;

  TSTT_getArrData(entity_handles, num_entities, tag_handle1, tag_values,
                  tag_value_size, &tag_size, err);
}


#undef __FUNC__
#define __FUNC__ "TSTT_setData"
void TSTT_setData(void *entity_handle1, void *tag_handle1, 
                  void *tag_value, int tag_size, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  int i,j;
  Tag_Instance_list *tag_instance_list;
  Tag_Instance_list *ent_tag_list;
  Entity_list *entity_list;
  int tag_found;

  CHK_NULL(entity_handle1,err,"entity_handles");
  CHK_NULL(tag_handle1,err,"tag_handles");

  tag_handle = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,OPAQUE,err);

  tag_instance_list = (Tag_Instance_list *)malloc(sizeof(Tag_Instance_list));
  entity_list = (Entity_list *)malloc(sizeof(Entity_list));

  entity_handle = (EntityHandle *)entity_handle1;

  /* if the tag is already on the entity, set the value */
  tag_found = 0;
  ent_tag_list = entity_handle->tag_instance_list;
  for (j=0;j<entity_handle->num_tags;j++) {
     if (ent_tag_list->tag_instance->parent_tag->id == tag_handle->id) {
        ent_tag_list->tag_instance->value = tag_value;
        tag_found = 1;
     }
     ent_tag_list = ent_tag_list->next;
  }

    /* otherwise add the tag to the entity and set the value */
  if (tag_found == 0) {
      tag_instance_list->tag_instance = (Tag_Instance *)malloc(sizeof(Tag_Instance));
      tag_instance_list->tag_instance->parent_tag = tag_handle;
      tag_instance_list->tag_instance->value = tag_value;
      ADD_TO_LIST(entity_handle->tag_instance_list,tag_instance_list);
      entity_handle->num_tags++;

      tag_handle->num_entities++;
      entity_list=(Entity_list *)malloc(sizeof(Entity_list));
      entity_list->entity=entity_handle;
      ADD_TO_LIST(tag_handle->entity_list,entity_list);
  }

}
#undef __FUNC__
#define __FUNC__ "TSTT_setIntData"
void TSTT_setIntData(void *entity_handle1, void *tag_handle1, 
                     int tag_value, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  int i,j;
  Int_Tag_Instance_list *int_tag_instance_list;
  Int_Tag_Instance_list *ent_tag_list;
  Entity_list *entity_list;
  int tag_found;

  if (tag_handle1==NULL) {
    err->code = 1;
    sprintf(err->msg,"NULL tag handle");
    return;
  }
  if (entity_handle1==NULL) {
    err->code = 1;
    sprintf(err->msg,"NULL entity handles");
    return;
  }

  tag_handle = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,INTEGER,err);

  entity_handle = (EntityHandle *)entity_handle1;

  /* if the tag is already on the entity, set the value */
  tag_found = 0;
  ent_tag_list = entity_handle->int_tag_instance_list;
  for (j=0;j<entity_handle->num_int_tags;j++) {
     if (ent_tag_list->tag_instance->parent_tag->id == tag_handle->id) {
        ent_tag_list->tag_instance->value = tag_value;
        tag_found = 1;
     }
     ent_tag_list = ent_tag_list->next;
  }

    /* otherwise add the tag to the entity and set the value */
  if (tag_found == 0) {
      int_tag_instance_list = (Int_Tag_Instance_list *)malloc(sizeof(Int_Tag_Instance_list));
      entity_list = (Entity_list *)malloc(sizeof(Entity_list));

      int_tag_instance_list->tag_instance = (Int_Tag_Instance *)malloc(sizeof(Int_Tag_Instance));
      int_tag_instance_list->tag_instance->parent_tag = tag_handle;
      int_tag_instance_list->tag_instance->value = tag_value;
      ADD_TO_LIST(entity_handle->int_tag_instance_list,int_tag_instance_list);
      entity_handle->num_int_tags++;

      tag_handle->num_entities++;
      entity_list=(Entity_list *)malloc(sizeof(Entity_list));
      entity_list->entity=entity_handle;
      ADD_TO_LIST(tag_handle->entity_list,entity_list);
  }

}


#undef __FUNC__
#define __FUNC__ "TSTT_setDblData"
void TSTT_setDblData(void *entity_handle1, void *tag_handle1, 
                     double tag_value, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  int i,j;
  Dbl_Tag_Instance_list *dbl_tag_instance_list;
  Dbl_Tag_Instance_list *ent_tag_list;
  Entity_list *entity_list;
  int tag_found;

  CHK_NULL(tag_handle1,err,"tag_handle");
  CHK_NULL(entity_handle1,err,"entity_handle");

  tag_handle = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,DOUBLE,err);

  dbl_tag_instance_list = (Dbl_Tag_Instance_list *)malloc(sizeof(Dbl_Tag_Instance_list));
  entity_list = (Entity_list *)malloc(sizeof(Entity_list));

  entity_handle = (EntityHandle *)entity_handle1;

  /* if the tag is already on the entity, set the value */
  tag_found = 0;
  ent_tag_list = entity_handle->dbl_tag_instance_list;
  for (j=0;j<entity_handle->num_dbl_tags;j++) {
     if (ent_tag_list->tag_instance->parent_tag->id == tag_handle->id) {
        ent_tag_list->tag_instance->value = tag_value;
        tag_found = 1;
     }
     ent_tag_list = ent_tag_list->next;
  }

    /* otherwise add the tag to the entity and set the value */
  if (tag_found == 0) {
      dbl_tag_instance_list->tag_instance = (Dbl_Tag_Instance *)malloc(sizeof(Dbl_Tag_Instance));
      dbl_tag_instance_list->tag_instance->parent_tag = tag_handle;
      dbl_tag_instance_list->tag_instance->value = tag_value;
      ADD_TO_LIST(entity_handle->dbl_tag_instance_list,dbl_tag_instance_list);
      entity_handle->num_dbl_tags++;

      tag_handle->num_entities++;
      entity_list=(Entity_list *)malloc(sizeof(Entity_list));
      entity_list->entity=entity_handle;
      ADD_TO_LIST(tag_handle->entity_list,entity_list);
  }
}


#undef __FUNC__
#define __FUNC__ "TSTT_setEHData"
void TSTT_setEHData(void *entity_handle1, void *tag_handle1, 
                  void *tag_value, Error *err)
{
  /* not tested, tag_value needs to be array of char */
  TSTT_setData(entity_handle1, tag_handle1, tag_value, sizeof(void *), err);
}

#undef __FUNC__
#define __FUNC__ "TSTT_setArrData"
void TSTT_setArrData(void **entity_handles, int num_entities, void *tag_handle1,
			void **tag_values, int tag_values_size, 
                        int tag_size, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  int i,j;
  Tag_Instance_list **tag_instance_list;
  Tag_Instance_list *ent_tag_list;
  Entity_list **entity_list;
  int tag_found;

  CHK_NULL(tag_handle1,err,"tag_handle");
  CHK_NULL(entity_handles,err,"entity_handles");

  tag_handle = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,OPAQUE,err);

  tag_instance_list = (Tag_Instance_list **)malloc(sizeof(Tag_Instance_list)*
						   num_entities);
  entity_list = (Entity_list **)malloc(sizeof(Entity_list *)*num_entities);

  for (i=0;i<num_entities;i++) {
    entity_handle = (EntityHandle *)entity_handles[i];

  /* if the tag is already on the entity, set the value */
    tag_found = 0;
    ent_tag_list = entity_handle->tag_instance_list;
    for (j=0;j<entity_handle->num_tags;j++) {
      if (ent_tag_list->tag_instance->parent_tag->id == tag_handle->id) {
	ent_tag_list->tag_instance->value = tag_values[i];
        tag_found = 1;
      }
      ent_tag_list = ent_tag_list->next;
    }

    /* otherwise add the tag to the entity and set the value */
    if (tag_found == 0) {
      tag_instance_list[i] = (Tag_Instance_list *)malloc(sizeof(Tag_Instance_list));
      tag_instance_list[i]->tag_instance = (Tag_Instance *)malloc(sizeof(Tag_Instance));
      tag_instance_list[i]->tag_instance->parent_tag = tag_handle;
      tag_instance_list[i]->tag_instance->value = tag_values[i];
      ADD_TO_LIST(entity_handle->tag_instance_list,tag_instance_list[i]);
      entity_handle->num_tags++;

      tag_handle->num_entities++;
      entity_list[i]=(Entity_list *)malloc(sizeof(Entity_list));
      entity_list[i]->entity=entity_handle;
      ADD_TO_LIST(tag_handle->entity_list,entity_list[i]);
    }
  }
}

#undef __FUNC__
#define __FUNC__ "TSTT_setIntArrData"
void TSTT_setIntArrData(void **entity_handles, int num_entities, void *tag_handle1,
			int *tag_values, int tag_values_size, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  int i,j;
  Int_Tag_Instance_list **tag_instance_list;
  Int_Tag_Instance_list *ent_tag_list;
  Entity_list **entity_list;
  int tag_found;

  CHK_NULL(tag_handle,err,"tag_handle");
  CHK_NULL(entity_handles,err,"entity_handles");

  tag_handle = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,INTEGER,err);

  for (i=0;i<num_entities;i++) {
    entity_handle = (EntityHandle *)entity_handles[i];

  /* if the tag is already on the entity, set the value */
    tag_found = 0;
    ent_tag_list = entity_handle->int_tag_instance_list;
    for (j=0;j<entity_handle->num_int_tags;j++) {
      if (ent_tag_list->tag_instance->parent_tag->id == tag_handle->id) {
	ent_tag_list->tag_instance->value = tag_values[i];
        tag_found = 1;
      }
      ent_tag_list = ent_tag_list->next;
    }

    /* otherwise add the tag to the entity and set the value */
    if (tag_found == 0) {
      tag_instance_list = 
         (Int_Tag_Instance_list **)malloc(sizeof(Int_Tag_Instance_list)*
					   num_entities);
      entity_list = (Entity_list **)malloc(sizeof(Entity_list *)*num_entities);

      tag_instance_list[i] = (Int_Tag_Instance_list *)malloc(sizeof(Int_Tag_Instance_list));
      tag_instance_list[i]->tag_instance = (Int_Tag_Instance *)malloc(sizeof(Int_Tag_Instance));
      tag_instance_list[i]->tag_instance->parent_tag = tag_handle;
      tag_instance_list[i]->tag_instance->value = tag_values[i];
      ADD_TO_LIST(entity_handle->int_tag_instance_list,tag_instance_list[i]);
      entity_handle->num_int_tags++;

      tag_handle->num_entities++;
      entity_list[i]=(Entity_list *)malloc(sizeof(Entity_list));
      entity_list[i]->entity=entity_handle;
      ADD_TO_LIST(tag_handle->entity_list,entity_list[i]);
    }
  }
}


#undef __FUNC__
#define __FUNC__ "TSTT_setDblArrData"
void TSTT_setDblArrData(void **entity_handles, int num_entities, void *tag_handle1,
			double *tag_values, int tag_values_size, Error *err)
{
  EntityHandle *entity_handle;
  Tag *tag_handle;
  int i,j;
  Dbl_Tag_Instance_list **tag_instance_list;
  Dbl_Tag_Instance_list *ent_tag_list;
  Entity_list **entity_list;
  int tag_found;

  CHK_NULL(tag_handle,err,"tag_handle");
  CHK_NULL(entity_handles,err,"entity_handles");

  tag_handle = (Tag *)tag_handle1;
  CHK_TAG_TYPE(tag_handle->tag_type,DOUBLE,err);

  for (i=0;i<num_entities;i++) {
    entity_handle = (EntityHandle *)entity_handles[i];

  /* if the tag is already on the entity, set the value */
    tag_found = 0;
    ent_tag_list = entity_handle->dbl_tag_instance_list;
    for (j=0;j<entity_handle->num_dbl_tags;j++) {
      if (ent_tag_list->tag_instance->parent_tag->id == tag_handle->id) {
	ent_tag_list->tag_instance->value = tag_values[i];
        tag_found = 1;
      }
      ent_tag_list = ent_tag_list->next;
    }

    /* otherwise add the tag to the entity and set the value */
    if (tag_found == 0) {
      tag_instance_list = 
         (Dbl_Tag_Instance_list **)malloc(sizeof(Dbl_Tag_Instance_list)*
					   num_entities);
      entity_list = (Entity_list **)malloc(sizeof(Entity_list *)*num_entities);

      tag_instance_list[i] = (Dbl_Tag_Instance_list *)malloc(sizeof(Dbl_Tag_Instance_list));
      tag_instance_list[i]->tag_instance = (Dbl_Tag_Instance *)malloc(sizeof(Dbl_Tag_Instance));
      tag_instance_list[i]->tag_instance->parent_tag = tag_handle;
      tag_instance_list[i]->tag_instance->value = tag_values[i];
      ADD_TO_LIST(entity_handle->dbl_tag_instance_list,tag_instance_list[i]);
      entity_handle->num_dbl_tags++;

      tag_handle->num_entities++;
      entity_list[i]=(Entity_list *)malloc(sizeof(Entity_list));
      entity_list[i]->entity=entity_handle;
      ADD_TO_LIST(tag_handle->entity_list,entity_list[i]);
    }
  }
}

#undef __FUNC__
#define __FUNC__ "TSTT_setEHArrData"
void TSTT_setEHArrData(void **entity_handles, int num_entities, void *tag_handle1,
			void **tag_values, int tag_values_size, 
                        Error *err)
{
  TSTT_setArrData(entity_handles,num_entities,tag_handle1,tag_values,
                  tag_values_size, sizeof(void *), err);
}
