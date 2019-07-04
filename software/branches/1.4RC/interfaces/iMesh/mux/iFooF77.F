#include "iFoo_mangle.h"

      subroutine iFoo_newMesh(opts, inst, err, opts_len)
      implicit none
#include "iFoo_f.h"
      character*80 opts
      iFoo_Instance inst 
      integer opts_len, err

      return
      end 

      subroutine iFoo_getRootSet(inst, rset, err)
      implicit none
#include "iFoo_f.h"
      iFoo_Instance inst
      iFoo_EntitySetHandle rset
      integer err

      return
      end 

      subroutine iFoo_setGeometricDimension(inst, n, err)
      implicit none
#include "iFoo_f.h"
      iFoo_Instance inst
      integer n, err

      return
      end 

      subroutine iFoo_createVtx(inst, x, y, z, vhdl, err)
      implicit none
#include "iFoo_f.h"
      iFoo_Instance inst
      real*8 x, y, z
      iFoo_EntityHandle vhdl
      integer err

      return
      end 
