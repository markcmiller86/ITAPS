  !*******************************************************
  !     A simple example that shows how to user iterators.
  !     This example has 4 parts.
  !     
  !     Part 1: a simple example using single entity iterators
  !     
  !     Part 2: a simple example using entity array iterators
  !     
  !     Part 3: a more advanced example using single entity iterators
  !     
  !     Part 4: a timing comparison of single vs array iterators 
  !********************************************************
  
#define CHECK(a) if (0 .ne. err) print *, a 
  
  
  !     A function which clocks cpu cycles on a pentium or higher x86 machine.
  
  program SimpleIteratorF90

    implicit none

#include "iBase_f.h"
#include "iMesh_f.h"

    !     create a new mesh interface instance from the factory
    integer err
    iMesh_Instance mesh
    integer num_vtx
    iBase_EntityIterator entIter
    iBase_EntityHandle entity, entities
    iBase_EntitySetHandle root_set
    pointer (rpentities, entities(0:*))
    real*8 Coords(0:2)
    integer counter, done_yet, size
    integer allocated, ent_type, zoot, start, iend
    iBase_EntityArrIterator ArrIter
    integer time

    call iMesh_newMesh("", mesh, err)
    CHECK("Didn't make new mesh instance.")

    call iMesh_getRootSet(%VAL(mesh), root_set, err)
    CHECK("Didn't get valid root set.")

    !     Load the vtk mesh from a file.
    call iMesh_load(%VAL(mesh), %VAL(root_set), &
         "../MeshFiles/125hex.vtk", "", err)
    CHECK("Failed to load mesh file.")

    call iMesh_getNumOfType(%VAL(mesh), %VAL(root_set), &
         %VAL(iBase_VERTEX), num_vtx, err)
    CHECK("Didn't get number of vertices.")

    counter = 0
    !ccccccc/  PART 1: SINGLE ENTITY ITERATOR EXAMPLE cccccc
    !     this example initializes a single entity iterator and will iterate 
    !     through the all verticies in the root set.
    !     initEntIter initializes the iterator to iterate through verticies.
    call iMesh_initEntIter(%VAL(mesh), %VAL(root_set), &
         %VAL(iBase_VERTEX), %VAL(iMesh_ALL_TOPOLOGIES), &
         %VAL(0), entIter, err)
    CHECK("Failed to get iterator.")

    done_yet=1

    print *, "Iterating through all verticies..."

    !     iterators are usually placed a loop.
10  continue

    !     getNextEntIter gets the next vertex. 
    call iMesh_getNextEntIter(%VAL(mesh), %VAL(entIter), entity, &
         done_yet, err)
    CHECK("Failed to get next ent from iter.")
    if (done_yet .eq. 1) go to 10

    !     end the iterator above
    call iMesh_endEntIter(%VAL(mesh), %VAL(entIter), err) 
    CHECK("Problem destroying iterat.")



    !cccccccccPART 2: SIMPLE ARR ITERATOR EXAMPLEcccccc
    !     this example initializes an array iterator and will iterate 
    !     through the all verticies in the root set.
    size = 10
    rpentities = 0
    allocated = 0

    !     Initialize a array entity iterator. The iterator below will iterate 
    !     through the all verticies in the root set.
    call iMesh_initEntArrIter(%VAL(mesh), %VAL(root_set), &
         %VAL(iBase_VERTEX), %VAL(iMesh_ALL_TOPOLOGIES), &
         %VAL(size), %VAL(0), entIter, err)
    CHECK("Array iterator initialization failed.")

    print *, "Iterating through verticies with an Array ", &
         "iterator.... "
    done_yet=1
20  continue
    !     getNextEntArrIter gets the next block of verticies, 
    !     and places it in the entities array. 
    call iMesh_getNextEntArrIter(%VAL(mesh), %VAL(entIter), &
         rpentities, allocated, size, &
         done_yet, err)
    CHECK("Failed to get next block of entities.")
    if (done_yet .eq. 1) goto 20

    !     end the iterator above
    call iMesh_endEntArrIter(%VAL(mesh), %VAL(entIter), err) 
    CHECK("Failed to release iterator.")
    call free(rpentities)
    rpentities = 0
    allocated = 0


    !cccc PART 3: ADVANCED SINGLE ENTITY ITERATOR EXAMPLE cccccc
    !     In addition to using:  initEntIter
    !     getNextEntIter
    !     endEntIter
    !     
    !     This example shows how to use: getEntType  
    !     getVtxCoord
    !     resetEntIter
    !     
    !     this example will iterate through ALL entities. By using getEntType,
    !     it will deterime which entities are are verticies. It will print 
    !     the first 10 verticies coordinates, then reset the iterator. It will 
    !     then restart from the beginging and iterate through the first 20 verticies. 
    !     initEntIter initializes the iterator to iterate through all entities.
    call iMesh_initEntIter(%VAL(mesh), %VAL(root_set), &
         %VAL(iBase_ALL_TYPES), %VAL(iMesh_ALL_TOPOLOGIES), &
         %VAL(0), entIter, err)
    CHECK("Failed to init advanced entity iter.")

    done_yet=1

    print *, "Iterating through all entites..."

    print *, "****Point Coordinates for first 10 verticies****"

30  continue

    !     getNextEntIter gets the next entity. 
    call iMesh_getNextEntIter(%VAL(mesh), %VAL(entIter), entity, &
         done_yet, err)
    CHECK("Failed to get next entity.")

    !     getEntType is a single entity query, 
    !     it simply returns the entity type.
    call iMesh_getEntType(%VAL(mesh), %VAL(entity), ent_type, err)
    CHECK("Failed to get entity type.")

    if(ent_type .eq. iBase_VERTEX) then
       counter = counter + 1
       !     getVtxCoord is another single entity query. 
       !     It gets the corridinates of a vertex.
       call iMesh_getVtxCoord(%VAL(mesh), %VAL(entity), Coords(0), &
            Coords(1), Coords(2), err)
       CHECK("Failed to get vtx coords.")

       !     printing the coordinate info
       print *, "* ", Coords(0), " ", Coords(1), " " , Coords(2)
    end if
    if (done_yet .eq. 1 .and. counter .lt. 10) goto 30

    print *, "********************************"
    print *, "reseting the iterator!."
    !     resetEntIter resets the iterator to the first entity in the iterator 
    call iMesh_resetEntIter(%VAL(mesh), %VAL(entIter), err)
    CHECK("Failed to reset iterator.")

    print *, "****Point Coordinates for first 20 verticies****"

    done_yet=1
    counter=0
40  continue
    !     getNextEntIter gets the next entity. 
    call iMesh_getNextEntIter(%VAL(mesh), %VAL(entIter), entity, &
         done_yet, err)
    CHECK("Failed to get next entity.")

    !     getEntType is a single entity query, 
    !     it simply returns the entity type.
    call iMesh_getEntType(%VAL(mesh), %VAL(entity), ent_type, err)
    CHECK("Failed to get entity type.")
    if(ent_type .eq. iBase_VERTEX) then
       counter = counter + 1
       !     getVtxCoord is another singe entity query. 
       !     It gets the corridinates of a vertex.
       call iMesh_getVtxCoord(%VAL(mesh), %VAL(entity), Coords(0), &
            Coords(1), Coords(2), err)
       CHECK("Failed to get vtx coords.")

       !     printing the coordinate info
       print *, "* ", Coords(0), " ", Coords(1), " ", Coords(2)
    end if
    if (done_yet .eq. 1 .and. counter .lt. 20) goto 40

    call iMesh_endEntIter(%VAL(mesh), %VAL(entIter), err)
    CHECK("Failed to destroy iterator.")



    !cccccccccccc/ PART 3: TIMING SECTION cccccccccccccccc/ 
    print *, "****Starting the timing results****"

    !     Initialize a single entity iterator. The iterator below will iterate 
    !     through the all verticies in the root set.
    call iMesh_initEntIter(%VAL(mesh), %VAL(root_set), &
         %VAL(iBase_VERTEX), %VAL(iMesh_ALL_TOPOLOGIES), &
         %VAL(0), entIter, err)
    CHECK("Failed to get iterator.")

    done_yet=1
    !     start timer
    start=time() 
50  continue
    call iMesh_getNextEntIter(%VAL(mesh), %VAL(entIter), entity, &
         done_yet, err) 
    !     getNextEntIter gets the next entity. 
    CHECK("Failed in getNextEntIter.")
    if (done_yet .eq. 1) goto 50

    call iMesh_endEntIter(%VAL(mesh), %VAL(entIter), err) 
    !     end the current single entity iterator
    CHECK("Failed to destroy iterator.")
    !     stop timer 
    iend = time() 
    print *, "*Single Iterator    =   ", iend-start

    do size=1, 2000, 100
       !     Initialize a array entity iterator. The iterator below will iterate 
       !     through the all verticies in array blocks that are the size 
       !     of the varible "size", in the loop above.
       call iMesh_initEntArrIter(%VAL(mesh), %VAL(root_set), &
            %VAL(iBase_VERTEX), %VAL(iMesh_ALL_TOPOLOGIES), &
            %VAL(size), %VAL(0), ArrIter, err)
       CHECK("Failed to get array iterator.")

       done_yet=1
       zoot=0
       !     start timer
       start=time()
       rpentities = 0
       allocated = 0
70     continue

       !     getNextEntIter gets the block of entities. 
       call iMesh_getNextEntArrIter(%VAL(mesh), %VAL(ArrIter), &
            rpentities, allocated, zoot, done_yet, err)
       CHECK("Failed to get next block of ents.")

       if (done_yet .eq. 1) goto 70
       call iMesh_endEntArrIter(%VAL(mesh), %VAL(ArrIter), err)
       CHECK("Failed to destroy iterator.")
       !     stop timer
       iend = time() 
       print *, "*Array Iterator (", size, ") =  ", &
            iend-start
       call free(rpentities)
    end do
    print *, "****Done with the timing results****"
    print *
    print *, "Thanks for running the iMesh iterator example program."
    print *,"I hope you learned something!  =-) "

    call iMesh_dtor(%VAL(mesh), err)
    CHECK("Destructor failed")
    return
  end program SimpleIteratorF90
