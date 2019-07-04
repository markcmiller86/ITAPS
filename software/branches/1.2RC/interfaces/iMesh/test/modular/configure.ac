#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(iMesh modular unit testing, API v. 1.0, cfog@mech.ubc.ca)
AC_CONFIG_SRCDIR([iMeshTester.hh])
#AC_CONFIG_HEADER([iMesh_unitTest_config.h])

AC_CONFIG_AUX_DIR(../conf)
AC_CANONICAL_HOST

ITAPS_PROG_CHECKS
ITAPS_PROG_BUILD_LIB

ITAPS_PATH_iMesh

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([malloc.h stddef.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_SETVBUF_REVERSED
AC_FUNC_STAT
AC_CHECK_FUNCS([sqrt])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
