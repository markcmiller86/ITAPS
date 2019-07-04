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



//  A function which clocks cpu cycles on a pentium or higher x86 machine.
extern __inline__ unsigned long long int rdtsc()
   {
     unsigned long long int x;
     __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
     return x;
   }

// The user must include TSTTB.hh and TSTTM.hh include files;
#include <iostream>
#include "TSTTB.hh"
#include "TSTTM.hh"

using std::cout;
using std::endl;

int main(int argv, char** argc) 
{
  // create a TSTT mesh factory
  TSTTM::Factory factory = TSTTM::Factory::_create();
  // create a new mesh interface instance from the factory
  TSTTM::Mesh mesh = factory.newMesh("GRUMMP_3D");
  void* RootSet = mesh.getRootSet();
  // Load the vtk mesh from a file.
  try {
    mesh.load(RootSet,"../../MeshFiles/tire");
  } catch (TSTTB::Error err) {
    err.echo("Failure loading the mesh");
    return(0);
  }


 TSTTM::Entity EntMesh = mesh; 
 int num_vtx = mesh.getNumOfType(RootSet, TSTTM::EntityType_VERTEX);
 RootSet = EntMesh.getRootSet();
 void* entIter;
 void* entity;
 double Coords[3];
 int counter=0,done_yet;
 char N_R_E;
 
 
 /////////////////  PART 1: SINGLE ENTITY ITERATOR EXAMPLE ////////////
 // this example initializes a single entity iterator and will iterate 
 // through the all verticies in the root set.
 
 // initEntIter initializes the iterator to iterate through verticies.
 EntMesh.initEntIter(RootSet, 
		     TSTTM::EntityType_VERTEX, 
		     TSTTM::EntityTopology_ALL_TOPOLOGIES, 
		     entIter);

 done_yet=1;
 
 printf("Iterating through all verticies...\n");
 
 while(done_yet==1) // iterators are usually placed a loop.
 {
    // getNextEntIter gets the next vertex. 
    done_yet=EntMesh.getNextEntIter(entIter, entity);
 } 
 EntMesh.endEntIter(entIter); // end the iterator above

 
 
 
 
 
 //////////////////// PART 2: SIMPLE ARR ITERATOR EXAMPLE////////////
 // this example initializes an array iterator and will iterate 
 // through the all verticies in the root set.
 TSTTM::Arr ArrMesh = mesh; 
 int size;
 sidl::array<void*> entities; // the array is left un initialized see 
                              // the array example for more info on 
			      // tstt arrays.
 
 // Initialize a array entity iterator. The iterator below will iterate 
 // through the all verticies in the root set.
 ArrMesh.initEntArrIter(RootSet, 
		     TSTTM::EntityType_VERTEX, 
		     TSTTM::EntityTopology_ALL_TOPOLOGIES, 
		     size,
		     entIter);

 printf("\nIterating through verticies with an Array iterator.... ");
 done_yet=1;
 int arr_size=0;
 while(done_yet==1)
 {
    // getNextEntArrIter gets the next block of verticies, 
    // and places it in the entities array. 
     done_yet=ArrMesh.getNextEntArrIter(entIter,entities,arr_size);	
 }
 ArrMesh.endEntArrIter(entIter); // end the iterator above





 
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
 EntMesh.initEntIter(RootSet, 
		     TSTTM::EntityType_ALL_TYPES, 
		     TSTTM::EntityTopology_ALL_TOPOLOGIES, 
		     entIter);
 done_yet=1;

 printf("Iterating through all entites...\n");
 
printf("\n\n****Point Coordinates for first 10 verticies****\n");
 while(done_yet==1 && counter < 10)
 {
    // getNextEntIter gets the next entity. 
    done_yet=EntMesh.getNextEntIter(entIter, entity);
 
    // getEntType is a single entity query, 
    // it simply returns the entity type.
    if(EntMesh.getEntType(entity)==TSTTM::EntityType_VERTEX)
    {
	counter++;
        // getVtxCoord is another single entity query. 
        // It gets the corridinates of a vertex.
        EntMesh.getVtxCoord(entity, Coords[0], Coords[1], Coords[2]);
        
	// printing the coordinate info
	printf( "* %f %f %f \n", Coords[0], Coords[1], Coords[2]);
    }			
 }

printf("********************************\n\n");
printf("reseting the iterator!. \n");
// resetEntIter resets the iterator to the first entity in the iterator 
EntMesh.resetEntIter(entIter);

printf("\n\n****Point Coordinates for first 20 verticies****\n");
 done_yet==1;
 counter=0;
 while(done_yet==1 && counter < 20)
 {
    // getNextEntIter gets the next entity. 
    done_yet=EntMesh.getNextEntIter(entIter, entity);
 
    // getEntType is a single entity query, 
    // it simply returns the entity type.
    if(EntMesh.getEntType(entity)==TSTTM::EntityType_VERTEX)
    {
	counter++;
        // getVtxCoord is another singe entity query. 
        // It gets the corridinates of a vertex.
        EntMesh.getVtxCoord(entity, Coords[0], Coords[1], Coords[2]);
        
	// printing the coordinate info
	printf( "* %f %f %f \n", Coords[0], Coords[1], Coords[2]);
     }				
 }
 
EntMesh.endEntIter(entIter);






/////////////////////////// PART 3: TIMING SECTION ///////////////////////////////// 
printf("\n\n****Starting the timing results****\n");
   RootSet = EntMesh.getRootSet();
   long int  start,end;
   void* ArrIter;

 // Initialize a single entity iterator. The iterator below will iterate 
 // through the all verticies in the root set.
 EntMesh.initEntIter(RootSet, 
		     TSTTM::EntityType_VERTEX, 
		     TSTTM::EntityTopology_ALL_TOPOLOGIES, 
		     entIter);
 
 done_yet=1;
 start=rdtsc(); // start timer
 while(done_yet==1)
 {
    done_yet=EntMesh.getNextEntIter(entIter, entity); // getNextEntIter gets the next entity. 
 }
 EntMesh.endEntIter(entIter); // end the current single entity iterator
 end=rdtsc(); //stop timer 
 printf("*Single Iterator    =   %li \n",end-start); 
 
 
 
 RootSet = ArrMesh.getRootSet();
 for(size=1;size< 2000;size=size*2)
 {
     // Initialize a array entity iterator. The iterator below will iterate 
     // through the all verticies in array blocks that are the size 
     // of the varible "size", in the loop above.
     sidl::array<void*> entities;
     ArrMesh.initEntArrIter(RootSet,
		        TSTTM::EntityType_VERTEX, 
		        TSTTM::EntityTopology_ALL_TOPOLOGIES,
		        size,
                        ArrIter);
     done_yet=1;
     int zoot=0;
     start=rdtsc(); // start timer
     while(done_yet==1)
     {
          // getNextEntIter gets the block of entities. 
          done_yet=ArrMesh.getNextEntArrIter(ArrIter,entities,zoot);
     }
    ArrMesh.endEntArrIter(ArrIter);
    end=rdtsc(); // stop timer
    printf("*Array Iterator (%i) =\t%li \n",size,end-start); 
 
 }

printf("\n\n****Done with the timing results****\n");
  printf("\n \n\nThanks for running the tsttm_iter_exmpl program.\n"
            "I hope you learned something!  =-) \n\n\n");

}
