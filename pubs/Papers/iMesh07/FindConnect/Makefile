#include /home/cfog/ITAPS/MOAB-repos/trunk/tools/iMesh/iMesh-Defs.inc
include /home/cfog/ITAPS/refImpl/iMesh-Defs.inc

FC = gfortran -fcray-pointer -m32
CXX = g++
CC = gcc

CXXFLAGS = -g -static
CFLAGS = -g
FFLAGS = -g 
FLFLAGS = -g  ${IMESH_FCFLAGS} 


FindConnectS: FindConnectS.o
	$(CXX)  $(CXXFLAGS) -o $@ FindConnectS.o ${IMESH_SIDL_LIBS} 

FindConnectC: FindConnectC.o
	$(CXX)  $(CXXFLAGS) -o $@ FindConnectC.o ${IMESH_LIBS} 

FindConnectF: FindConnectF.o
	$(CXX) -m32 -lgfortran -lgfortranbegin -o $@ FindConnectF.o ${IMESH_LIBS} ${IMESH_CXX_LIBS}

.cpp.o:
	${CXX} -c ${CXXFLAGS} ${IMESH_INCLUDES} $(IMESH_SIDL_INCLUDES) $<

.cc.o:
	${CXX} -c ${CFLAGS} ${IMESH_INCLUDES} $(IMESH_SIDL_INCLUDES) $<

.c.o:
	$(CC) -c $(CFLAGS) $(IMESH_INCLUDES) $(IMESH_SIDL_INCLUDES) $<

.F.o:
	${FC} -c ${FFLAGS} ${IMESH_INCLUDES} $<

