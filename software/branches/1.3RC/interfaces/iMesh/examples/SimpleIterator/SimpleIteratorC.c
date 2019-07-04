/********************************************************
 * A simple example that shows how to user iterators.
 * This example has 4 parts.
 * 
 * Part 1: a simple example using single entity iterators
 * 
 * Part 2: a simple example using entity array iterators
 *
 * Part 3: a more advanced example using single entity iterators
 * 
 * Part 4: a timing comparison of single vs array iterators
 ********************************************************* 
 */

#define CHECK(a) if (iBase_SUCCESS != err) printf("%s\n", a)


/*  A function which clocks cpu cycles on a pentium or higher x86 machine. */
extern __inline__ unsigned long long int rdtsc()
   {
     unsigned long long int x;
     __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
     return x;
   }

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "iMesh.h"

int main(int argv, char** argc) 
{
    /* create a new mesh interface instance from the factory */
  int err;
  iMesh_Instance mesh;
  iBase_EntitySetHandle root_set;
  iMesh_EntityIterator entIter;
  iBase_EntityHandle entity;
  double Coords[3];
  int counter,done_yet;
  int size;
  iBase_EntityHandle *entities; 
  int allocated;
    int ent_type;
  time_t start,end;
  iMesh_EntityArrIterator ArrIter;
  int zoot;
 
  iMesh_newMesh("", &mesh, &err, 0);
  CHECK("Didn't make new mesh instance.");

  iMesh_getRootSet(mesh, &root_set, &err);
  CHECK("Didn't get valid root set.");
  
    /* Load the vtk mesh from a file. */
  iMesh_load(mesh, root_set, "../MeshFiles/125hex.vtk", "", &err, 23, 0);
  CHECK("Failed to load mesh file.");

  int num_vtx;
  iMesh_getNumOfType(mesh, root_set, iBase_VERTEX, &num_vtx, &err);
  CHECK("Didn't get number of vertices.");

  counter = 0;
  
    /*///////////////  PART 1: SINGLE ENTITY ITERATOR EXAMPLE //////////// */
    /* this example initializes a single entity iterator and will iterate  */
    /* through the all verticies in the root set. */
 
    /* initEntIter initializes the iterator to iterate through verticies. */
  iMesh_initEntIter(mesh, root_set, 
                    iBase_VERTEX, iMesh_ALL_TOPOLOGIES, 
                    &entIter, &err);
  CHECK("Failed to get iterator.");

  done_yet=1;
 
  printf("Iterating through all verticies...\n");
 
  while(done_yet == 1) /* iterators are usually placed a loop. */
  {
      /* getNextEntIter gets the next vertex.  */
    iMesh_getNextEntIter(mesh, entIter, &entity, &done_yet, &err);
    CHECK("Failed in getting next entity from iterator.");
  } 

  iMesh_endEntIter(mesh, entIter, &err); /* end the iterator above */
  CHECK("Problem destroying iterat.");
 
 
 
    /*////////////////// PART 2: SIMPLE ARR ITERATOR EXAMPLE//////////// */
    /* this example initializes an array iterator and will iterate  */
    /* through the all verticies in the root set. */
  size = 10;
  entities = NULL; 
  allocated = 0;
 
    /* Initialize a array entity iterator. The iterator below will iterate  */
    /* through the all verticies in the root set. */
  iMesh_initEntArrIter(mesh, root_set, 
                       iBase_VERTEX, iMesh_ALL_TOPOLOGIES, 
                       size, &entIter, &err);
  CHECK("Array iterator initialization failed.");

  printf("\nIterating through verticies with an Array iterator.... \n");
  done_yet=1;
  while(done_yet==1)
  {
      /* getNextEntArrIter gets the next block of verticies,  */
      /* and places it in the entities array.  */
    iMesh_getNextEntArrIter(mesh, entIter, 
                            &entities, &allocated, &size, 
                            &done_yet, &err);
    CHECK("Failed to get next block of entities.");
  }
  free(entities);
  iMesh_endEntArrIter(mesh, entIter, &err); /* end the iterator above */
  CHECK("Failed to release iterator.");

 
    /*//////// PART 3: ADVANCED SINGLE ENTITY ITERATOR EXAMPLE //////////// */
    /* In addition to using:  initEntIter */
    /*                        getNextEntIter */
    /*                        endEntIter */
    /* */
    /* This example shows how to use: getEntType   */
    /*                                 getVtxCoord */
    /*                                 resetEntIter */
    /* */
    /* this example will iterate through ALL entities. By using getEntType, */
    /* it will deterime which entities are are verticies. It will print  */
    /* the first 10 verticies coordinates, then reset the iterator. It will  */
    /* then restart from the beginging and iterate through the first 20 verticies.  */

 
    /* initEntIter initializes the iterator to iterate through all entities. */
  iMesh_initEntIter(mesh, root_set, 
                    iBase_ALL_TYPES, iMesh_ALL_TOPOLOGIES, 
                    &entIter, &err);
  CHECK("Failed to initialize advanced entity iterator.");

  done_yet=1;

  printf("Iterating through all entites...\n");
 
  printf("\n\n****Point Coordinates for first 10 verticies****\n");
  while(done_yet==1 && counter < 10)
  {
      /* getNextEntIter gets the next entity.  */
    iMesh_getNextEntIter(mesh, entIter, &entity, &done_yet, &err);
    CHECK("Failed to get next entity.");
 
      /* getEntType is a single entity query,  */
      /* it simply returns the entity type. */
    iMesh_getEntType(mesh, entity, &ent_type, &err);
    CHECK("Failed to get entity type.");
    
    if(ent_type == iBase_VERTEX)
    {
      counter++;
        /* getVtxCoord is another single entity query.  */
        /* It gets the corridinates of a vertex. */
      iMesh_getVtxCoord(mesh, entity, Coords, Coords+1, Coords+2, &err);
      CHECK("Failed to get vtx coords.");
        
        /* printing the coordinate info */
      printf("* %f %f %f\n", Coords[0], Coords[1], Coords[2]);
    }			
  }

    printf("********************************\n\n");
    printf("reseting the iterator!.\n");
/* resetEntIter resets the iterator to the first entity in the iterator  */
  iMesh_resetEntIter(mesh, entIter, &err);
  CHECK("Failed to reset iterator.");

  printf("\n\n****Point Coordinates for first 20 verticies****\n");
  
  done_yet=1;
  counter=0;
  while(done_yet==1 && counter < 20)
  {
      /* getNextEntIter gets the next entity.  */
    iMesh_getNextEntIter(mesh, entIter, &entity, &done_yet, &err);
    CHECK("Failed to get next entity.");
 
      /* getEntType is a single entity query,  */
      /* it simply returns the entity type. */
    iMesh_getEntType(mesh, entity, &ent_type, &err);
    CHECK("Failed to get entity type.");
    if(ent_type == iBase_VERTEX)
    {
      counter++;
        /* getVtxCoord is another singe entity query.  */
        /* It gets the corridinates of a vertex. */
      iMesh_getVtxCoord(mesh, entity, Coords, Coords+1, Coords+2,
                        &err);
      CHECK("Failed to get vtx coords.");
        
        /* printing the coordinate info */
      printf("* %f %f %f\n", Coords[0], Coords[1], Coords[2]);
    }				
  }
 
  iMesh_endEntIter(mesh, entIter, &err);
  CHECK("Failed to destroy iterator.");



/*///////////////////////// PART 3: TIMING SECTION /////////////////////////////////  */
  printf("\n\n****Starting the timing results****\n");
    /* Initialize a single entity iterator. The iterator below will iterate  */
    /* through the all verticies in the root set. */
  iMesh_initEntIter(mesh, root_set, 
                    iBase_VERTEX, iMesh_ALL_TOPOLOGIES, 
                    &entIter, &err);
  CHECK("Failed to get iterator.");
 
  done_yet=1;
  start=time(NULL); /* start timer */
  while(done_yet==1)
  {
    iMesh_getNextEntIter(mesh, entIter, &entity, &done_yet, &err); /* getNextEntIter gets the next entity.  */
    CHECK("Failed in getNextEntIter.");
  }
  iMesh_endEntIter(mesh, entIter, &err); /* end the current single entity iterator */
  CHECK("Failed to destroy iterator.");
  end=time(NULL); /*stop timer  */
  printf("*Single Iterator    =   %ld\n", end-start); 
 
  for(size=1;size< 2000;size=size*2)
  {
      /* Initialize a array entity iterator. The iterator below will iterate  */
      /* through the all verticies in array blocks that are the size  */
      /* of the varible "size", in the loop above. */
    entities = NULL;
    iMesh_initEntArrIter(mesh, root_set,
                         iBase_VERTEX, iMesh_ALL_TOPOLOGIES,
                         size, &ArrIter, &err);
    CHECK("Failed to get array iterator.");
    
    done_yet=1;
    zoot=0;
    start=time(NULL); /* start timer */
    while(done_yet==1)
    {
        /* getNextEntIter gets the block of entities.  */
      iMesh_getNextEntArrIter(mesh, ArrIter, 
                              &entities, &allocated, &zoot, &done_yet, &err);
      CHECK("Failed to get next block of entities.");
    }
    iMesh_endEntArrIter(mesh, ArrIter, &err);
    CHECK("Failed to destroy iterator.");
    end=time(NULL); /* stop timer */
    printf("*Array Iterator (%d) = %ld\n", size, end-start);
  }

  printf("\n\n****Done with the timing results****\n");
  printf("\n\n\nThanks for running the tsttm_iter_exmpl program.\n");
  printf("I hope you learned something!  =-)\n\n\n ");

  return 0;
}
