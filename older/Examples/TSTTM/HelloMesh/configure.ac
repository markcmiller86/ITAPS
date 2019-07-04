dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(TSTTM Hello Mesh test, 0.7, diachin2@llnl.gov)
AC_CONFIG_SRCDIR([C/hello_mesh.c])

AC_CONFIG_AUX_DIR(../../conf)

TSTT_CLIENT_C=yes
TSTT_CLIENT_CXX=yes
TSTT_CLIENT_F77=yes
TSTT_CLIENT_F90=yes

CLIENT_LANGS="C C++ F77 F90"

TSTT_PROG_CHECKS

TSTT_PATH_TSTTB
TSTT_PATH_TSTTM

dnl Checks for header files.

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.

AC_CONFIG_FILES([Makefile C/Makefile C++/Makefile F77/Makefile F90/Makefile])
AC_OUTPUT
