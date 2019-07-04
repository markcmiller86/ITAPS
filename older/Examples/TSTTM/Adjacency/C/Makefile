
include /home/freitag/TSTT/Repository/TSTT/Implementations/SimpleMesh/TSTTB-Defs.inc
include /home/freitag/TSTT/Repository/TSTT/Implementations/SimpleMesh/TSTTM-Defs.inc

INCLUDES = $(TSTTB_SERVER_INCLUDES) $(TSTTM_SERVER_INCLUDES) \
	   $(TSTTM_BACKEND_INCLUDES) $(SIDL_INCLUDES) \
	   $(TSTTB_CLIENT_C_INCLUDES) $(TSTTM_CLIENT_C_INCLUDES)
LIBS = $(TSTTM_LIB_DIR) $(TSTTM_SERVER_LIBS) $(TSTTB_LIB_DIR) $(TSTTB_SERVER_LIBS) \
	$(TSTTM_BACKEND_LIBS) $(TSTTM_CLIENT_C_LIBS) $(TSTTB_CLIENT_C_LIBS) \
	$(TSTTM_CLIENT_CXX_LIBS) $(TSTTB_CLIENT_CXX_LIBS)\
	$(SIDL_LIBS)

C_OBJS = adj_example.o 

all: adj_example 

adj_example: $(C_OBJS)
	gcc -g $(C_OBJS) -o $@ $(LIBS)

clean:
	rm -rf *.o *~

distclean:
	rm -rf *.o *~ array_example

.c.o:
	echo Compiling *.c
	gcc -g $(INCLUDES) -c $< 
