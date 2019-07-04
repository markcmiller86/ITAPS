appc:	iFooc.c iFoo.h iFoo_mangle.h
	gcc -D'IFOO_MANGLE(A)=implc_##A' -g -c -I. iFooc.c
	rm -f libFooc.a
	ar q libFooc.a iFooc.o
	gcc -D'IFOO_MANGLE(A)=implc_##A' -g -o appc -I. app.c -L. -lFooc

appC:	iFooCXX.C iFoo.h iFoo_mangle.h
	g++ -D'IFOO_MANGLE(A)=implC_##A' -g -c -I. iFooCXX.C
	rm -f libFooC.a
	ar q libFooC.a iFooCXX.o
	gcc -D'IFOO_MANGLE(A)=implC_##A' -g -o appC -I. app.c -L. -lFooC -lstdc++

# note: 'implF77' also gets mangled by Fortran compiler. So, you need to have
# it automatically do that for you.
appF77:	iFooF77.F iFoo_f.h iFoo.h iFoo_mangle.h
	cpp -DPOINTER_SIZE=4 -D'IFOO_MANGLE_F_LIB(A)=implf77_##A' -I. iFooF77.F iFooF77_.F
	g77 -g -c iFooF77_.F
	mv iFooF77_.o iFooF77.o
	rm -f libFooF77.a
	ar q libFooF77.a iFooF77.o
	gcc -D'IFOO_MANGLE_F_APP(name,NAME)=implf77_##name##__' -g -o appF77 -I. app.c -L. -lFooF77

mux:	iFooMux.c iFoo_mangle.h
	gcc -DHAVE_implc -g -c -I. iFooMux.c

all: appc appC appF77

clean:
	rm -f iFooc.o libFooc.a iFooCXX.o libFooC.a iFooF77.o iFooF77_.F iFooF77_.o libiFooF77.a iFooMux.o appc appC appF77
