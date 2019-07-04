AC_INIT(iZoltanSerial, 0.9, vjleung@sandia.gov)
AC_CONFIG_FILES(Makefile)
AC_PROG_CXX(g++)

#
# Setup IMESH_DEFS variable
#
AC_ARG_VAR([IMESH_DEFS], [Full path-qualified name of iMesh-Defs.inc] )
if test -z "$IMESH_DEFS"; then
   AC_MSG_ERROR(IMESH_DEFS variable not specified)
else
   if ! test -r $IMESH_DEFS; then
      AC_MSG_ERROR([The file specified for IMESH_DEFS, "$IMESH_DEFS", either does not exists of is not readable])
   fi
fi

AC_SUBST(with_zoltan_include)
AC_ARG_WITH(zoltan_include,
  AS_HELP_STRING(--with-zoltan-include,
    [directory for Zoltan include, defaults to ../Zoltan_v3.3/src/include]),
  if test "x$with_zoltan_include" = "xyes"; then with_zoltan_include=../Zoltan_v3.3/src/include
    fi,
  with_zoltan_include=../Zoltan_v3.3/src/include)

AC_SUBST(with_zoltan_lib)
AC_ARG_WITH(zoltan_lib,
  AS_HELP_STRING(--with-zoltan-lib,
    [directory for Zoltan lib, defaults to ../Zoltan_v3.3/src/Obj_linux64]),
  if test "x$with_zoltan_lib" = "xyes"; then with_zoltan_lib=../Zoltan_v3.3/src/Obj_linux64
    fi,
  with_zoltan_lib=../Zoltan_v3.3/src/Obj_linux64)

AC_SUBST(with_parmetis)
AC_ARG_WITH(parmetis,
  AS_HELP_STRING(--with-parmetis,
    [directory for ParMETIS3.1 library, defaults to ../ParMETIS3_1]),
  if test "x$with_parmetis" = "xyes"; then with_parmetis=../ParMETIS3_1
    fi
  with_parmetis="-L$with_parmetis -lparmetis -lmetis",
  with_parmetis=)

AC_SUBST(with_mpi_include)
AC_ARG_WITH(mpi_include,
  AS_HELP_STRING(--with-mpi-include,
    [directory for MPI include, defaults to ../Zoltan_v3.3/siMPI/pyMPI/siMPI]),
  if test "x$with_mpi_include" = "xyes"; then with_mpi_include=../Zoltan_v3.3/siMPI/pyMPI/siMPI
    fi,
  with_mpi_include=../Zoltan_v3.3/siMPI/pyMPI/siMPI)

AC_SUBST(with_mpi_lib)
AC_ARG_WITH(mpi_lib,
  AS_HELP_STRING(--with-mpi-lib,
    [directory for MPI lib, defaults to ../Zoltan_v3.3/siMPI/pyMPI/siMPI]),
  if test "x$with_mpi_lib" = "xyes"; then with_mpi_lib=../Zoltan_v3.3/siMPI/pyMPI/siMPI
    fi,
  with_mpi_lib=../Zoltan_v3.3/siMPI/pyMPI/siMPI)

AM_INIT_AUTOMAKE(foreign)
AC_OUTPUT
