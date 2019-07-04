c*******************************************************
c     A simple example that shows how to user iterators.
c     This example has 4 parts.
c     
c     Part 1: a simple example using single entity iterators
c     
c     Part 2: a simple example using entity array iterators
c     
c     Part 3: a more advanced example using single entity iterators
c     
c     Part 4: a timing comparison of single vs array iterators 
c********************************************************

c This macro is tricky. All the instances of it used here
c pass a 47 character string (not including open and closing
c quote chars). So, when the macro expands with the if
c statement preceeding it, the whole line hits 72 char limit
c I needed to compress out some spaces to pass 'err' arg
c without requiring me to touch every instance of use of
c the macro below.
#define CHECK(a) if(0.ne.err)call Ex(a,err)

      subroutine Ex(errmsg,err)
      implicit none
      character*37 errmsg
      integer err
      print *, errmsg
      call exit(err)
      end

c     A function which clocks cpu cycles on a pentium or higher x86 machine.

      program SimpleIteratorF77

      implicit none

#include "iBase_f.h"
#include "iMesh_f.h"

c     create a new mesh interface instance from the factory
      integer err
      iMesh_Instance mesh
      integer num_vtx
      iBase_EntityIterator entIter
      iBase_EntityHandle entity, entities
      iBase_EntitySetHandle root_set
      IBASE_HANDLE_T rpentities
      pointer (rpentities, entities(0:*))
      real*8 Coords(0:2)
      integer counter, done_yet, size
      integer allocated, ent_type, zoot, start, end_time
      iBase_EntityArrIterator ArrIter
      integer time
      character*80 errmsg

      call iMesh_newMesh("", mesh, err)
      CHECK("Didn't make new mesh instance.       ")

      call iMesh_getRootSet(%VAL(mesh), root_set, err)
      CHECK("Didn't get valid root set.           ")
      
c     Load the vtk mesh from a file.
      call iMesh_load(%VAL(mesh), %VAL(root_set), 
     1     "../MeshFiles/125hex.vtk", "", err)
      CHECK("Failed to load mesh file.            ")

      call iMesh_getNumOfType(%VAL(mesh), %VAL(root_set), 
     1     %VAL(iBase_VERTEX), num_vtx, err)
      CHECK("Didn't get number of vertices.       ")

      counter = 0
cccccccc/  PART 1: SINGLE ENTITY ITERATOR EXAMPLE cccccc
c     this example initializes a single entity iterator and will iterate 
c     through the all verticies in the root set.
c     initEntIter initializes the iterator to iterate through verticies.
      call iMesh_initEntIter(%VAL(mesh), %VAL(root_set), 
     1     %VAL(iBase_VERTEX), %VAL(iMesh_ALL_TOPOLOGIES), 
     1     %VAL(0), entIter, err)
      CHECK("Failed to get iterator.              ")

      done_yet=1
      
      print *, "Iterating through all verticies..."
      
c     iterators are usually placed a loop.
 10   continue

c     getNextEntIter gets the next vertex. 
      call iMesh_getNextEntIter(%VAL(mesh), %VAL(entIter), entity, 
     1     done_yet, err)
      CHECK("Failed to get next ent from iter.    ")
      if (done_yet .eq. 1) go to 10

c     end the iterator above
      call iMesh_endEntIter(%VAL(mesh), %VAL(entIter), err) 
      CHECK("Problem destroying iterat.           ")
      
      
      
ccccccccccPART 2: SIMPLE ARR ITERATOR EXAMPLEcccccc
c     this example initializes an array iterator and will iterate 
c     through the all verticies in the root set.
      size = 10
      allocated = 0
      
c     Initialize a array entity iterator. The iterator below will iterate 
c     through the all verticies in the root set.
      call iMesh_initEntArrIter(%VAL(mesh), %VAL(root_set), 
     1     %VAL(iBase_VERTEX), %VAL(iMesh_ALL_TOPOLOGIES), 
     1     %VAL(size), %VAL(0), entIter, err)
      CHECK("Array iterator initialization failed.")

      print *, "Iterating through verticies with an Array ",
     1     "iterator.... "
      done_yet=1
      allocated = 0
      rpentities = 0
 20   continue
c     getNextEntArrIter gets the next block of verticies, 
c     and places it in the entities array. 
      call iMesh_getNextEntArrIter(%VAL(mesh), %VAL(entIter), 
     1     rpentities, allocated, size, 
     1     done_yet, err)
      CHECK("Failed to get next block of entities.")
      if (done_yet .eq. 1) goto 20

      call free(rpentities)
      rpentities = 0
      allocated = 0
c     end the iterator above
      call iMesh_endEntArrIter(%VAL(mesh), %VAL(entIter), err) 
      CHECK("Failed to release iterator.          ")
      
      
ccccc PART 3: ADVANCED SINGLE ENTITY ITERATOR EXAMPLE cccccc
c     In addition to using:  initEntIter
c     getNextEntIter
c     endEntIter
c     
c     This example shows how to use: getEntType  
c     getVtxCoord
c     resetEntIter
c     
c     this example will iterate through ALL entities. By using getEntType,
c     it will deterime which entities are are verticies. It will print 
c     the first 10 verticies coordinates, then reset the iterator. It will 
c     then restart from the beginging and iterate through the first 20 verticies. 
c     initEntIter initializes the iterator to iterate through all entities.
      call iMesh_initEntIter(%VAL(mesh), %VAL(root_set), 
     1     %VAL(iBase_ALL_TYPES), %VAL(iMesh_ALL_TOPOLOGIES), 
     1     %VAL(0), entIter, err)
      CHECK("Failed to init advanced entity iter. ")

      done_yet=1

      print *, "Iterating through all entites..."
      
      print *, "****Point Coordinates for first 10 verticies****"

 30   continue
      
c     getNextEntIter gets the next entity. 
      call iMesh_getNextEntIter(%VAL(mesh), %VAL(entIter), entity,
     1     done_yet, err)
      CHECK("Failed to get next entity.           ")
      
c     getEntType is a single entity query, 
c     it simply returns the entity type.
      call iMesh_getEntType(%VAL(mesh), %VAL(entity), ent_type, err)
      CHECK("Failed to get entity type.           ")
      
      if(ent_type .eq. iBase_VERTEX) then
         counter = counter + 1
c     getVtxCoord is another single entity query. 
c     It gets the corridinates of a vertex.
         call iMesh_getVtxCoord(%VAL(mesh), %VAL(entity), Coords(0),
     1        Coords(1), Coords(2), err)
         CHECK("Failed to get vtx coords.         ")
         
c     printing the coordinate info
         print *, "* ", Coords(0), " ", Coords(1), " " , Coords(2)
      end if
      if (done_yet .eq. 1 .and. counter .lt. 10) goto 30

      print *, "********************************"
      print *, "reseting the iterator!."
c     resetEntIter resets the iterator to the first entity in the iterator 
      call iMesh_resetEntIter(%VAL(mesh), %VAL(entIter), err)
      CHECK("Failed to reset iterator.            ")

      print *, "****Point Coordinates for first 20 verticies****"
      
      done_yet=1
      counter=0
 40   continue
c     getNextEntIter gets the next entity. 
      call iMesh_getNextEntIter(%VAL(mesh), %VAL(entIter), entity,
     1     done_yet, err)
      CHECK("Failed to get next entity.           ")
      
c     getEntType is a single entity query, 
c     it simply returns the entity type.
      call iMesh_getEntType(%VAL(mesh), %VAL(entity), ent_type, err)
      CHECK("Failed to get entity type.           ")
      if(ent_type .eq. iBase_VERTEX) then
         counter = counter + 1
c     getVtxCoord is another singe entity query. 
c     It gets the corridinates of a vertex.
         call iMesh_getVtxCoord(%VAL(mesh), %VAL(entity), Coords(0), 
     1        Coords(1), Coords(2), err)
         CHECK("Failed to get vtx coords.         ")
         
c     printing the coordinate info
         print *, "* ", Coords(0), " ", Coords(1), " ", Coords(2)
      end if
      if (done_yet .eq. 1 .and. counter .lt. 20) goto 40
      
      call iMesh_endEntIter(%VAL(mesh), %VAL(entIter), err)
      CHECK("Failed to destroy iterator.          ")



ccccccccccccc/ PART 3: TIMING SECTION cccccccccccccccc/ 
      print *, "****Starting the timing results****"

c     Initialize a single entity iterator. The iterator below will iterate 
c     through the all verticies in the root set.
      call iMesh_initEntIter(%VAL(mesh), %VAL(root_set), 
     1     %VAL(iBase_VERTEX), %VAL(iMesh_ALL_TOPOLOGIES), 
     1     %VAL(0), entIter, err)
      CHECK("Failed to get iterator.              ")
      
      done_yet=1
c     start timer
      start=time() 
 50   continue
      call iMesh_getNextEntIter(%VAL(mesh), %VAL(entIter), entity, 
     1     done_yet, err) 
c     getNextEntIter gets the next entity. 
      CHECK("Failed in getNextEntIter.            ")
      if (done_yet .eq. 1) goto 50

      call iMesh_endEntIter(%VAL(mesh), %VAL(entIter), err) 
c     end the current single entity iterator
      CHECK("Failed to destroy iterator.          ")
c     stop timer 
      end_time=time() 
      print *, "*Single Iterator    =   ", end_time-start
      
      size = 1
 60   continue
         rpentities = 0
         allocated = 0

c     Initialize a array entity iterator. The iterator below will iterate 
c     through the all verticies in array blocks that are the size 
c     of the varible "size", in the loop above.
         call iMesh_initEntArrIter(%VAL(mesh), %VAL(root_set),
     1        %VAL(iBase_VERTEX), %VAL(iMesh_ALL_TOPOLOGIES),
     1        %VAL(size), %VAL(0), ArrIter, err)
         CHECK("Failed to get array iterator.     ")
         
         done_yet=1
         zoot=0
c     start timer
         start=time()
 70      continue

c     getNextEntIter gets the block of entities. 
         call iMesh_getNextEntArrIter(%VAL(mesh), %VAL(ArrIter), 
     1        rpentities, allocated, zoot, done_yet, err)
         CHECK("Failed to get next block of ents.")

         if (done_yet .eq. 1) goto 70
         call iMesh_endEntArrIter(%VAL(mesh), %VAL(ArrIter), err)
         CHECK("Failed to destroy iterator.       ")
         call free(rpentities)
c     stop timer
         end_time=time() 
         print *, "*Array Iterator (", size, ") =  ",
     1        end_time-start
         
         size = size * 2
      if (size .lt. 2000) goto 60
      print *, "****Done with the timing results****"
      print *
      print *, "Thanks for running the iMesh iterator example program."
      print *,"I hope you learned something!  =-) "

      call iMesh_dtor(%VAL(mesh), err)
      CHECK("Destructor failed")
      return
      end
