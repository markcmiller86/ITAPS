#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(iMesh Unit Test, 1.0, cfog@mech.ubc.ca)
AC_CONFIG_SRCDIR([iMesh_unitTest.hh])
AC_CONFIG_HEADER([iMesh_unitTest_config.h])

AC_CONFIG_AUX_DIR(conf)
AC_CANONICAL_HOST

dnl Setup IMESH_DEFS variable
AC_ARG_VAR(IMESH_DEFS, [Full path-qualified name of iMesh-Defs.inc] )
if test -z "$IMESH_DEFS"; then
    AC_MSG_ERROR(IMESH_DEFS variable not specified)
else
    if ! test -r $IMESH_DEFS; then
        AC_MSG_ERROR([The file specified for IMESH_DEFS, "$IMESH_DEFS", either does not exist or is not readable])
    fi
fi

dnl These are needed by later function calls
AC_PROG_EGREP

dnl Must always test for a C compiler
AC_PROG_CPP
AC_PROG_CC
AC_PROG_CC_C_O

dnl ************************************************************
dnl
dnl Checks for header files, typedefs, structures, and compiler 
dnl characteristics..   Some implementations may need more 
dnl than these; none will need less, as the auto-client code needs 
dnl this much 

AC_HEADER_STDC
AC_C_CONST
AC_C_INLINE

AC_PROG_CXXCPP
AC_PROG_CXX

AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

ITAPS_PROG_BUILD_LIB

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
