AC_INIT(iplex,0.0,[kraftche@cae.wisc.edu])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_LIBTOOL
AC_CHECK_HEADERS( iMesh.h, ,AC_MSG_ERROR([No iMesh.h header found. Try specifying CPPFLAGS]))
AC_CHECK_HEADERS( ltdl.h, ,AC_MSG_ERROR([ltdl (included with libtool) required]))
AC_CHECK_LIB( ltdl, lt_dlopenext, , AC_MSG_ERROR([ltdl (included with libtool) required]))
AC_CONFIG_FILES( [Makefile] )
AC_OUTPUT
