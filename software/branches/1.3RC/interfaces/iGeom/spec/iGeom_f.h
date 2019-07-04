#ifndef IGEOM_F_H
#define IGEOM_F_H

#ifdef POINTER_SIZE
#if POINTER_SIZE == 8
#define iGeom_Instance integer*8
#elif POINTER_SIZE == 4
#define iGeom_Instance integer*4
#else
#define iGeom_Instance integer
#endif
#else
#define iGeom_Instance integer
#endif
#endif

#include "iBase_f.h"
