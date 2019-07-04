//*******************************************************
// A simple example that shows how to user iterators.
// This example has 4 parts.
// 
// Part 1: a simple example using single entity iterators
//
// Part 2: a simple example using entity array iterators
//
// Part 3: a more advanced example using single entity iterators
//
// Part 4: a timing comparison of single vs array iterators 
//********************************************************

#define CHECK(a) if (iBase_SUCCESS != err) std::cout << a << std::endl


//  A function which clocks cpu cycles on a pentium or higher x86 machine.
extern __inline__ unsigned long long int rdtsc()
   {
     unsigned long long int x;
     __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
     return x;
   }

#include <iostream>
#include <cstdlib>
#include "iMesh.h"

int main(int argc, char* argv[]) 
{
  // create a new mesh interface instance from the factory
  const int resilient = 0;
  int err;
  iMesh_Instance mesh;
  iMesh_newMesh("", &mesh, &err, 0);
  CHECK("Didn't make new mesh instance.");

  iBase_EntitySetHandle root_set;
  iMesh_getRootSet(mesh, &root_set, &err);
  CHECK("Didn't get valid root set.");

  std::string fname = "../MeshFiles/125hex.vtk";
  if (argc > 1) fname = std::string(argv[1]);
  
  // Load the vtk mesh from a file.
  iMesh_load(mesh, root_set, fname.c_str(), "", &err, fname.size(), 0);
  CHECK("Failed to load mesh file.");

  int num_vtx;
  iMesh_getNumOfType(mesh, root_set, iBase_VERTEX, &num_vtx, &err);
  CHECK("Didn't get number of vertices.");

  iBase_EntityIterator entIter;
  iBase_EntityArrIterator entArrIter;
  iBase_EntityHandle entity;
  double Coords[3];
  int counter=0,done_yet;
 
 /////////////////  PART 1: SINGLE ENTITY ITERATOR EXAMPLE ////////////
 // this example initializes a single entity iterator and will iterate 
 // through the all verticies in the root set.
 
 // initEntIter initializes the iterator to iterate through verticies.
  iMesh_initEntIter(mesh, root_set, 
                    iBase_VERTEX, iMesh_ALL_TOPOLOGIES, 
                    resilient, &entIter, &err);
  CHECK("Failed to get iterator.");

  done_yet=1;
 
  std::cout << "Iterating through all verticies..." << std::endl;
 
  while(done_yet==1) // iterators are usually placed a loop.
  {
      // getNextEntIter gets the next vertex. 
    iMesh_getNextEntIter(mesh, entIter, &entity, &done_yet, &err);
    CHECK("Failed in getting next entity from iterator.");
  } 

  iMesh_endEntIter(mesh, entIter, &err); // end the iterator above
  CHECK("Problem destroying iterat.");
 
 
 
    //////////////////// PART 2: SIMPLE ARR ITERATOR EXAMPLE////////////
    // this example initializes an array iterator and will iterate 
    // through the all verticies in the root set.
  int size = 10;
  iBase_EntityHandle *entities = NULL; 
  int allocated = 0;
 
    // Initialize a array entity iterator. The iterator below will iterate 
    // through the all verticies in the root set.
  iMesh_initEntArrIter(mesh, root_set, 
                       iBase_VERTEX, iMesh_ALL_TOPOLOGIES, 
                       size, resilient, &entArrIter, &err);
  CHECK("Array iterator initialization failed.");

  std::cout << std::endl << "Iterating through verticies with an Array iterator.... " << std::endl;
  done_yet=1;
  while(done_yet==1)
  {
      // getNextEntArrIter gets the next block of verticies, 
      // and places it in the entities array. 
    iMesh_getNextEntArrIter(mesh, entArrIter, 
                            &entities, &allocated, &size, 
                            &done_yet, &err);
    CHECK("Failed to get next block of entities.");
  }
  free(entities);
  iMesh_endEntArrIter(mesh, entArrIter, &err); // end the iterator above
  CHECK("Failed to release iterator.");

 
    ////////// PART 3: ADVANCED SINGLE ENTITY ITERATOR EXAMPLE ////////////
    // In addition to using:  initEntIter
    //                        getNextEntIter
    //                        endEntIter
    //
    // This example shows how to use: getEntType  
    //                                 getVtxCoord
    //                                 resetEntIter
    //
    // this example will iterate through ALL entities. By using getEntType,
    // it will deterime which entities are are verticies. It will print 
    // the first 10 verticies coordinates, then reset the iterator. It will 
    // then restart from the beginging and iterate through the first 20 verticies. 

 
    // initEntIter initializes the iterator to iterate through all entities.
  iMesh_initEntIter(mesh, root_set, 
                    iBase_ALL_TYPES, iMesh_ALL_TOPOLOGIES, 
                    resilient, &entIter, &err);
  CHECK("Failed to initialize advanced entity iterator.");

  done_yet=1;

  std::cout << "Iterating through all entites..." << std::endl;
 
  std::cout << std::endl << std::endl
            << "****Point Coordinates for first 10 verticies****" << std::endl;
  while(done_yet==1 && counter < 10)
  {
      // getNextEntIter gets the next entity. 
    iMesh_getNextEntIter(mesh, entIter, &entity, &done_yet, &err);
    CHECK("Failed to get next entity.");
 
      // getEntType is a single entity query, 
      // it simply returns the entity type.
    int ent_type;
    iMesh_getEntType(mesh, entity, &ent_type, &err);
    CHECK("Failed to get entity type.");
    
    if(ent_type == iBase_VERTEX)
    {
      counter++;
        // getVtxCoord is another single entity query. 
        // It gets the corridinates of a vertex.
      iMesh_getVtxCoord(mesh, entity, Coords, Coords+1, Coords+2, &err);
      CHECK("Failed to get vtx coords.");
        
        // printing the coordinate info
      std::cout << "* " << Coords[0] << " " << Coords[1] << " " 
                << Coords[2] << std::endl;
    }			
  }

  std::cout << "********************************" << std::endl << std::endl;
  std::cout << "reseting the iterator!." << std::endl;
// resetEntIter resets the iterator to the first entity in the iterator 
  iMesh_resetEntIter(mesh, entIter, &err);
  CHECK("Failed to reset iterator.");

  std::cout << std::endl << std::endl 
            << "****Point Coordinates for first 20 verticies****"
            << std::endl;
  
  done_yet=1;
  counter=0;
  while(done_yet==1 && counter < 20)
  {
      // getNextEntIter gets the next entity. 
    iMesh_getNextEntIter(mesh, entIter, &entity, &done_yet, &err);
    CHECK("Failed to get next entity.");
 
      // getEntType is a single entity query, 
      // it simply returns the entity type.
    int ent_type;
    iMesh_getEntType(mesh, entity, &ent_type, &err);
    CHECK("Failed to get entity type.");
    if(ent_type == iBase_VERTEX)
    {
      counter++;
        // getVtxCoord is another singe entity query. 
        // It gets the corridinates of a vertex.
      iMesh_getVtxCoord(mesh, entity, Coords, Coords+1, Coords+2,
                        &err);
      CHECK("Failed to get vtx coords.");
        
        // printing the coordinate info
      std::cout << "* " << Coords[0] << " " << Coords[1] << " " 
                << Coords[2] << std::endl;
    }				
  }
 
  iMesh_endEntIter(mesh, entIter, &err);
  CHECK("Failed to destroy iterator.");



/////////////////////////// PART 3: TIMING SECTION ///////////////////////////////// 
  std::cout << std::endl << std::endl
            << "****Starting the timing results****" << std::endl;
  long int start,end;
  iBase_EntityArrIterator ArrIter;

    // Initialize a single entity iterator. The iterator below will iterate 
    // through the all verticies in the root set.
  iMesh_initEntIter(mesh, root_set, 
                    iBase_VERTEX, iMesh_ALL_TOPOLOGIES, 
                    resilient, &entIter, &err);
  CHECK("Failed to get iterator.");
 
  done_yet=1;
  start=rdtsc(); // start timer
  while(done_yet==1)
  {
    iMesh_getNextEntIter(mesh, entIter, &entity, &done_yet, &err); // getNextEntIter gets the next entity. 
    CHECK("Failed in getNextEntIter.");
  }
  iMesh_endEntIter(mesh, entIter, &err); // end the current single entity iterator
  CHECK("Failed to destroy iterator.");
  end=rdtsc(); //stop timer 
  std::cout << "*Single Iterator    =   " << end-start << std::endl; 
 
  for(size=1;size< 2000;size=size*2)
  {
      // Initialize a array entity iterator. The iterator below will iterate 
      // through the all verticies in array blocks that are the size 
      // of the varible "size", in the loop above.
    iBase_EntityHandle *entities = NULL;
    allocated = 0;
    iMesh_initEntArrIter(mesh, root_set,
                         iBase_VERTEX, iMesh_ALL_TOPOLOGIES,
                         size, resilient, &ArrIter, &err);
    CHECK("Failed to get array iterator.");
    
    done_yet=1;
    int zoot=0;
    start=rdtsc(); // start timer
    while(done_yet==1)
    {
        // getNextEntIter gets the block of entities. 
      iMesh_getNextEntArrIter(mesh, ArrIter, 
                              &entities, &allocated, &zoot, &done_yet, &err);
      CHECK("Failed to get next block of entities.");
    }
    iMesh_endEntArrIter(mesh, ArrIter, &err);
    CHECK("Failed to destroy iterator.");
    end=rdtsc(); // stop timer
    std::cout << "*Array Iterator (" << size << ") =  " 
              <<  end-start << std::endl; 
    free(entities);
 
  }

  iMesh_dtor(mesh, &err);

  std::cout << std::endl << std::endl
            << "****Done with the timing results****" << std::endl;
  std::cout << std::endl << std::endl << std::endl
            << "Thanks for running the iMesh iterator example program." << std::endl
            << "I hope you learned something!  =-) " << std::endl << std::endl
            << std::endl;
}
