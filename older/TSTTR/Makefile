include /home/cfog/TSTT/repository/Implementations/GRUMMP/TSTTB-Defs.inc
include /home/cfog/TSTT/repository/Implementations/GRUMMP/TSTTM-Defs.inc
include /home/cfog/TSTT/repository/Implementations/GRUMMP/TSTTR-Defs.inc

ALL_INCLUDES=\
-I/home/cfog/TSTT/repository/Implementations/GRUMMP/TSTTR_GRUMMP/server \
-I/home/cfog/TSTT/repository/Implementations/GRUMMP/TSTTM_GRUMMP/server \
$(TSTTR_CLIENT_CXX_INCLUDES) \
$(TSTTM_CLIENT_CXX_INCLUDES) \
$(TSTTB_CLIENT_CXX_INCLUDES) \
$(SIDL_INCLUDES) \
$(TSTTR_SERVER_INCLUDES) \
$(TSTTM_SERVER_INCLUDES) \
$(TSTTB_SERVER_INCLUDES) \
$(TSTTM_BACKEND_INCLUDES)

LIBS=\
$(TSTTR_LIB_DIR) \
$(TSTTM_LIB_DIR) \
$(TSTTB_LIB_DIR) \
$(TSTTR_ALL_CXX_LIBS) \
$(TSTTM_ALL_CXX_LIBS) \
$(TSTTB_ALL_CXX_LIBS) \
$(SIDL_LIBS) \
$(TSTTM_BACKEND_LIBS) \
-ldl -lxml2 -lz  -L/usr/local/lib

DEPS=\
$(TSTTR_CLIENT_CXX_LIBFILE) \
$(TSTTR_SERVER_LIBFILE) \
$(TSTTM_CLIENT_CXX_LIBFILE) \
$(TSTTM_SERVER_LIBFILE) \
$(TSTTB_CLIENT_CXX_LIBFILE) \
$(TSTTB_SERVER_LIBFILE) \
$(SIDL_LIBFILE) \
$(TSTTM_BACKEND_LIBFILE)

.cc.o:
	g++ $(IMPL_FLAGS) $(ALL_INCLUDES) -g -c $<

TSTTR_unitTest: TSTTR_unitTest.o $(DEPS)
	g++ -static -o TSTTR_unitTest TSTTR_unitTest.o ${LIBS}


