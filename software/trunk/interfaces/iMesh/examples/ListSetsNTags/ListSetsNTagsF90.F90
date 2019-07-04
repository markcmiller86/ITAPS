  ! ListSetsNTags: list sets & tags from a mesh
  ! 
  ! This program shows how to read and list sets and tags from a mesh
  !
  ! Usage: SetsNTags <mesh_file_name>
  
  
  
#define ERRORR(a) if (0 .ne. err) print *, a
  
  program ListSetsNTags

#include "iBase_f.h"
#include "iMesh_f.h"

    iMesh_Instance mesh
    iBase_EntitySetHandle root_set
    integer err

    IBASE_HANDLE_T rpsets, rptags
    pointer (rpsets, sets(0:*))
    pointer (rptags, tags(0:*))
    iBase_EntitySetHandle sets
    iBase_TagHandle tags
    integer sets_alloc, sets_size, tags_alloc, tags_size

    real*8 dbl_val
    integer int_val, tag_type
    character tname*128

    integer i, j, num_hops

    ! status info
    print *, "Running ListSetsNTags with ../MeshFiles/125hex.vtk"

    ! create the Mesh instance
    call iMesh_newMesh("", mesh, err)
    ERRORR("Error creating new mesh.")


    call iMesh_getRootSet(%VAL(mesh), root_set, err)
    ERRORR("Couldn't get root set.")

    ! load the mesh
    call iMesh_load(%VAL(mesh), %VAL(root_set), &
         "../MeshFiles/125hex.vtk", "", err)
    ERRORR("Couldn't load mesh.")

    ! get all sets
    sets_alloc = 0
    num_hops = 1
    call iMesh_getEntSets(%VAL(mesh), %VAL(root_set), %VAL(num_hops), &
         rpsets, sets_alloc, sets_size, err)
    ERRORR("Couldn't get all sets.")

    ! iterate through them, checking whether they have tags
    do i = 0, sets_size-1
       ! get connectivity
       tags_alloc = 0
       call iMesh_getAllEntSetTags(%VAL(mesh), %VAL(sets(i)), &
            rptags, tags_alloc, tags_size, err)
       ERRORR("Failed to get ent set tags.")

       if (0 .ne. tags_size) then
          print *, "Set ", sets(i), " Tags:"
       end if

       ! list tag names on this set
       do j = 0, tags_size-1
          call iMesh_getTagName(%VAL(mesh), %VAL(tags(j)), tname, err)
          print *, tname
          call iMesh_getTagType(%VAL(mesh), %VAL(tags(j)), tag_type, err)
          ERRORR("Failed to get tag type.")
          if (iBase_INTEGER .eq. tag_type) then
             call iMesh_getEntSetIntData(%VAL(mesh), %VAL(sets(i)), &
                  %VAL(tags(j)), int_val, err)
             ERRORR("Failed to get int data type.")
             print *, "(val = ", int_val, ") "
          else if (iBase_DOUBLE .eq. tag_type) then
             call iMesh_getEntSetDblData(%VAL(mesh), %VAL(sets(i)), &
                  %VAL(tags(j)), dbl_val, err)
             print *, "(val = ", dbl_val, ") "
          end if

       end do

       if (tags_size .ne. 0) call free(rptags)
       tags_alloc = 0
    end do

    if (sets_size .ne. 0) call free(rpsets)

    call iMesh_dtor(%VAL(mesh), err)
    ERRORR("Failed to destruct interface.")

    ! status info
    print *, "Done running ListSetsNTags"

    return
  end program ListSetsNTags

