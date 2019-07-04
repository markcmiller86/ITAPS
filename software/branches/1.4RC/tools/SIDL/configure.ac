################################################################################
#                                Standard Stuff
################################################################################
AC_INIT(ITAPS_SIDL, 1.00)
AC_CANONICAL_SYSTEM

# Request some extra GCC flags
EXTRA_GNU_FLAGS='-Wall -pipe -pedantic'

################################################################################
#                                     BABEL
################################################################################
AC_ARG_VAR([BABEL],[Babel compiler])
AC_ARG_VAR([BABEL_CONFIG],[Babel configuration query tool])

# if we have babel but not babel-config
if test "x$BABEL" != "x"; then
  if test "x$BABEL_CONFIG" = "x"; then
    dir="`expr "$BABEL" : '\(.*\)/'`"
    if test -x "${BABEL}-config"; then
      BABEL_CONFIG="${BABEL}-config"
    elif test -x "${dir}/babel-config"; then
      BABEL_CONFIG="${dir}/babel-config"
    else
      AC_MSG_RESULT([not found])
      AC_CHECK_PROG([BABEL_CONFIG],[babel-config],[babel-config])
    fi
  fi
# if we have babel-config but not babel
elif test "x$BABEL_CONFIG" != "x"; then
  bindir=`$BABEL_CONFIG --bindir`
  dir="`expr "$BABEL" : '\(.*\)/'`"
  base="`expr "$BABEL" : '\(.*\)-config'`"
  if test -x "${base}"; then
    BABEL="$base"
  elif test -x "${bindir}/babel"; then
    BABEL="${bindir}/babel"
  elif test -x "${dir}/babel"; then
    BABEL="${dir}/babel"
  else
    AC_MSG_RESULT([not found])
    AC_CHECK_PROG([BABEL],[babel],[babel])
  fi
# otherwise hopefully they're in $PATH somewhere
else
  AC_CHECK_PROG([BABEL_CONFIG],[babel-config],[babel-config])
  AC_CHECK_PROG([BABEL],[babel],[babel])
fi
AC_MSG_CHECKING([for babel])
AC_MSG_RESULT([$BABEL])
AC_MSG_CHECKING([for babel-config])
AC_MSG_RESULT([$BABEL_CONFIG])

if test "x$BABEL" = "x"; then
  AC_MSG_ERROR([babel not found.  Try specifying 'BABEL=<path to babel>'.])
fi

if test "x$BABEL_CONFIG" = "x"; then
  AC_MSG_ERROR([babel-config not found.  Try specifying 'BABEL_CONFIG=<path to babel>'.])
fi

BABELFLAGS=--suppress-timestamp
AC_SUBST([BABELFLAGS])

################################################################################
#                           Configure Compilers
################################################################################

AC_ARG_VAR([F77],[Fortran77 compiler to use for building SIDL Fortran bindings and for determining mangling scheme for C API])
AC_ARG_VAR([FC],[Fortran90 compiler to use for building SIDL Fortran bindings and for determining mangling scheme for C API])
AC_ARG_VAR([JAVAC],[Java compiler to use for building SIDL Java bindings])
AC_ARG_VAR([JAR],[Java archive tool])
AC_ARG_VAR([PYTHON],[Python interpreter])
AC_SUBST(F77)
AC_SUBST(FC)
AC_SUBST(JAVAC)
AC_SUBST(JAR)
AC_SUBST(F77FLAGS)
AC_SUBST(JAVACFLAGS)
AC_SUBST(PYTHON)

AC_PROG_SED
AC_PROG_MKDIR_P
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_F77
AC_PROG_FC
if test "x" = "x$PYTHON"; then
  AC_CHECK_PROG([PYTHON],[python],[python])
fi
if test "x" = "x$JAVAC"; then
  AC_CHECK_PROG([JAVAC],[javac],[javac])
elif ! test -x "$JAVAC"; then
  AC_MSG_ERROR( "$JAVAC: not executable" )
fi
if test "x" = "x$MAKE"; then
  AC_CHECK_PROG([MAKE],[make],[make])
fi
if test "x" = "x$JAR"; then
  jar_path=$PATH
  test "x" = "$JAVAC" || jar_path="$PATH:`dirname $JAVAC`"
  AC_CHECK_PROG([JAR],[jar],[jar],[],[$jar_path])
fi

# If we couldn't find a compiler for some language, but babel
# supports that language, ask babel what compiler it used.
AC_DEFUN([CHECK_BABEL_COMPILER],[
if test x = x$2; then
  if $1=`$BABEL_CONFIG --query-var=$1`; then
    test -x $ || $1=
  fi
fi
])
CHECK_BABEL_COMPILER( [CXX], [$CXX] )
CHECK_BABEL_COMPILER( [F77], [$F77] )
CHECK_BABEL_COMPILER( [FC], [$FC] )
CHECK_BABEL_COMPILER( [JAVAC], [$JAVAC] )
CHECK_BABEL_COMPILER( [PYTHON], [$PYTHON] )
CHECK_BABEL_COMPILER( [JAR], [$JAR] )


################################################################################
#                    Check which SIDL bindings we can support
################################################################################
AC_DEFUN([HAVE_LANG],[
  AC_MSG_CHECKING([if we can do $2 bindings])
  if ${BABEL_CONFIG} "--with-$2"; then
    if test "x$1" = "x"; then
      HAVE_$1=no
    else
      HAVE_$1=yes
    fi
  else
    HAVE_$1=no
  fi
  AC_SUBST(HAVE_$1)
  AC_MSG_RESULT([$HAVE_$1])
])
HAVE_LANG([CXX],[c++])
HAVE_LANG([F77],[f77])
HAVE_LANG([FC], [f90])
HAVE_LANG([JAVAC],[java])
HAVE_LANG([PYTHON],[python])
 

################################################################################
#                    Options for Babel language bindins
################################################################################
AC_ARG_ENABLE([c],[AC_HELP_STRING([--enable-c],[Generate SIDL C bindings])],
              [BABEL_C="$enableval"],[BABEL_C=yes])
AC_ARG_ENABLE([cxx],[AC_HELP_STRING([--enable-cxx],[Generate SIDL C++ bindings])],
              [BABEL_CXX="$enableval"],[BABEL_CXX=$HAVE_CXX])
AC_ARG_ENABLE([f77],[AC_HELP_STRING([--enable-f77],[Generate SIDL Fortran 77 bindings])],
              [BABEL_F77="$enableval"],[BABEL_F77=$HAVE_F77])
AC_ARG_ENABLE([f90],[AC_HELP_STRING([--enable-f90],[Generate SIDL Fortran 90 bindings])],
              [BABEL_F90="$enableval"],[BABEL_F90=$HAVE_FC])
AC_ARG_ENABLE([java],[AC_HELP_STRING([--enable-java],[Generate SIDL Java bindings])],
              [BABEL_JAVA="$enableval"],[BABEL_JAVA=$HAVE_JAVAC])
AC_ARG_ENABLE([python],[AC_HELP_STRING([--enable-python],[Generate SIDL python bindings])],
              [BABEL_PYTHON="$enableval"],[BABEL_PYTHON=$HAVE_PYTHON])
case "x$BABEL_C" in
  xno)
    ;;
  xyes)
    LANG_BUILD_SUBDIRS="$LANG_BUILD_SUBDIRS cclient"
    ;;
  *)
    AC_MSG_ERROR("Invalid value for --enable-c: $BABEL_C")
    ;;
esac
AC_SUBST(BABEL_C)

case "x$BABEL_CXX" in
  xno)
    ;;
  xyes)
    if test "xno" = "x$HAVE_CXX"; then
      AC_MSG_ERROR([Cannot renerte or build C++ bindings])
    fi
    LANG_BUILD_SUBDIRS="$LANG_BUILD_SUBDIRS cxxclient"
    ;;
  *)
    AC_MSG_ERROR([Invalid value for --enable-cxx: $BABEL_CXX])
    ;;
esac
AC_SUBST(BABEL_CXX)

case "x$BABEL_F77" in
  xno)
    ;;
  xyes)
    if test "xno" = "x$HAVE_F77"; then
      AC_MSG_ERROR([Cannot renerte or build Fortran77 bindings])
    fi
    LANG_BUILD_SUBDIRS="$LANG_BUILD_SUBDIRS f77client"
    ;;
  *)
    AC_MSG_ERROR([Invalid value for --enable-f77: $BABEL_F77])
    ;;
esac
AC_SUBST(BABEL_F77)

case "x$BABEL_F90" in
  xno)
    ;;
  xyes)
    if test "xno" = "x$HAVE_FC"; then
      AC_MSG_ERROR([Cannot renerte or build Fortran90 bindings])
    fi
    LANG_BUILD_SUBDIRS="$LANG_BUILD_SUBDIRS f90client"
    ;;
  *)
    AC_MSG_ERROR([Invalid value for --enable-f90: $BABEL_F90])
    ;;
esac
AC_SUBST(BABEL_F90)

case "x$BABEL_JAVA" in
  xno)
    ;;
  xyes)
    if test "xno" = "x$HAVE_JAVAC"; then
      AC_MSG_ERROR([Cannot renerte or build Java bindings])
    fi
    LANG_BUILD_SUBDIRS="$LANG_BUILD_SUBDIRS javaclient"
    ;;
  *)
    AC_MSG_ERROR([Invalid value for --enable-java: $BABEL_JAVAC])
    ;;
esac
AC_SUBST(BABEL_JAVA)

case "x$BABEL_PYTHON" in
  xno)
    ;;
  xyes)
    if test "xno" = "x$HAVE_PYTHON"; then
      AC_MSG_ERROR([Cannot renerte or build Python bindings])
    fi
    LANG_BUILD_SUBDIRS="$LANG_BUILD_SUBDIRS pyclient"
    ;;
  *)
    AC_MSG_ERROR([Invalid value for --enable-python: $BABEL_PYTHON])
    ;;
esac
AC_SUBST(BABEL_PYTHON)

AC_SUBST(LANG_BUILD_SUBDIRS)

################################################################################
#                    Configure Libtool and Fortran name mangling
################################################################################


AC_PROG_LIBTOOL
AC_SUBST(LTFLAGS)
INSTALL_STRIP_FLAG=-s
AC_SUBST(INSTALL_STRIP_FLAG)

# Detect Fortran name mangling scheme.  If both F90 and F77 compilers
# are available, use the F90 complier's mangling scheme.
if test "x" != "x$FC"; then
  AC_FC_WRAPPERS
elif test "x" != "x$F77"; then
  AC_F77_WRAPPERS
  FC_FUNC="${F77_FUNC}"
  FC_FUNC_="${F77_FUNC_}"
fi


################################################################################
#                             ITAPS interfaces
################################################################################

MAIN_BUILD_FACES=Base
MAIN_TEST_FACES=

AC_ARG_ENABLE( [imesh],
[AC_HELP_STRING([--disable-imesh],[Do not build SIDL bindings for iMesh API])
 AC_HELP_STRING([--enable-imesh=nofortran],[Build to be linked to an iMesh implementation that does not Fortran-mangle C function names])],
[ENABLE_IMESH=`echo $enableval | tr [:upper:] [:lower:]`],
[ENABLE_IMESH=yes]
)
case "x$ENABLE_IMESH" in
  xnofortran)
    iMesh_FC_FUNC=
    iMesh_FC_FUNC_=
    ENABLE_IMESH=yes
    MAIN_BUILD_FACES="$MAIN_BUILD_FACES Mesh"
    ;;
  xyes)
    iMesh_FC_FUNC=FC_FUNC
    iMesh_FC_FUNC_=FC_FUNC_
    MAIN_BUILD_FACES="$MAIN_BUILD_FACES Mesh"
    ;;
  xno)
    ;;
  *)
    AC_MSG_ERROR( [Invalid value for --enable-imesh flag: $ENABLE_IMESH])
    ;;
esac
AC_SUBST(ENABLE_IMESH)
AC_DEFINE_UNQUOTED([iMesh_FC_FUNC],[$iMesh_FC_FUNC],[Fortran mangling for function names without underscores])
AC_DEFINE_UNQUOTED([iMesh_FC_FUNC_],[$iMesh_FC_FUNC_],[Fortran mangling for function names with underscores])

AC_ARG_ENABLE( [igeom],
[AC_HELP_STRING([--disable-igeom],[Do not build SIDL bindings for iGeom API])
 AC_HELP_STRING([--enable-igeom=nofortran],[Build to be linked to an iGeom implementation that does not Fortran-mangle C function names])],
[ENABLE_IGEOM=`echo $enableval | tr [:upper:] [:lower:]`],
[ENABLE_IGEOM=yes]
)
case "x$ENABLE_IGEOM" in
  xnofortran)
    iGeom_FC_FUNC=
    iGeom_FC_FUNC_=
    ENABLE_IGEOM=yes
    MAIN_BUILD_FACES="$MAIN_BUILD_FACES Geom"
    ;;
  xyes)
    iGeom_FC_FUNC=FC_FUNC
    iGeom_FC_FUNC_=FC_FUNC_
    MAIN_BUILD_FACES="$MAIN_BUILD_FACES Geom"
    ;;
  xno)
    ;;
  *)
    AC_MSG_ERROR( [Invalid value for --enable-igeom flag: $ENABLE_IGEOM])
    ;;
esac
AC_SUBST(ENABLE_IGEOM)
AC_DEFINE_UNQUOTED([iGeom_FC_FUNC],[$iGeom_FC_FUNC],[Fortran mangling for function names without underscores])
AC_DEFINE_UNQUOTED([iGeom_FC_FUNC_],[$iGeom_FC_FUNC_],[Fortran mangling for function names with underscores])


# Extract the value of a variable from a Makefile and assign it to
# a shell variable.
# Arguments: { destination shell var, makefile, makefile var }
AC_DEFUN([GET_MAKEFILE_VAR],[
  echo "echovar:"               > config_tmp.make
  echo "	@echo '\$($3)'" >> config_tmp.make
  echo                         >> config_tmp.make
  echo "include $2"            >> config_tmp.make
  if $1=`make -s -f config_tmp.make echovar`; then 
    rm config_tmp.make
  else
    rm config_tmp.make
    AC_MSG_ERROR("Failed to read value of '$3' from file '$2'")
  fi
])

AC_ARG_WITH([test-imesh],
            [AC_HELP_STRING([--with-test-imesh=PATH/iMesh-Defs.inc],
                            [iMesh implementation to use for testing])],
            [TEST_IMESH="$withval"],
            [TEST_IMESH=no])
case "x$TEST_IMESH" in
  xyes)
  	AC_MSG_ERROR([--with-test-imesh must specify path to iMesh-Defs.inc])
        ;;
  xno)
  	;;
  *)
  	if test -d "$TEST_IMESH"; then TEST_IMESH="$TEST_IMESH/iMesh-Defs.inc"; fi
        test -f "$TEST_IMESH" || AC_MSG_ERROR([$TEST_IMESH : no such file])
        GET_MAKEFILE_VAR([TEST_IMESH_LIBS],[$TEST_IMESH],[IMESH_LIBS])
        MAIN_TEST_FACES="$MAIN_TEST_FACES Mesh"
        ;;
esac
AC_SUBST(TEST_IMESH_LIBS)

AC_ARG_WITH([test-igeom],
            [AC_HELP_STRING([--with-test-igeom=PATH/iGeom-Defs.inc],
                            [iMGeom implementation to use for testing])],
            [TEST_IGEOM="$withval"],
            [TEST_IGEOM=no])
case "x$TEST_IGEOM" in
  xyes)
  	AC_MSG_ERROR([--with-test-igeom must specify path to iGeom-Defs.inc])
        ;;
  xno)
  	;;
  *)
  	if test -d "$TEST_IGEOM"; then TEST_IGEOM="$TEST_IGEOM/iGeom-Defs.inc"; fi
        test -f "$TEST_IGEOM" || AC_MSG_ERROR([$TEST_IGEOM : no such file])
        GET_MAKEFILE_VAR([TEST_IGEOM_LIBS],[$TEST_IGEOM],[IGEOM_LIBS])
        MAIN_TEST_FACES="$MAIN_TEST_FACES Geom"
        ;;
esac
AC_SUBST(TEST_IGEOM_LIBS)

AC_SUBST(MAIN_BUILD_FACES)
AC_SUBST(MAIN_TEST_FACES)

################################################################################
#                     Get paths and libraries from babel
################################################################################

SIDL_LIBDIR=`$BABEL_CONFIG --libdir`
SIDL_INCDIR=`$BABEL_CONFIG --includedir`
BABEL_VERSION=`$BABEL_CONFIG --version`
BABEL_PY_VERSION=`$BABEL_CONFIG --query-var=PYTHON_VERSION`
BABEL_PY_INCLUDE=`$BABEL_CONFIG --includes-py`
BABEL_JNI_INCLUDE=`$BABEL_CONFIG --includes-jni`
AC_SUBST(SIDL_LIBDIR)
AC_SUBST(SIDL_INCDIR)
AC_SUBST(BABEL_VERSION)
AC_SUBST(BABEL_PY_VERSION)
AC_SUBST(BABEL_PY_INCLUDE)
AC_SUBST(BABEL_JNI_INCLUDE)

# Test if a specified integer type (int, long, etc) is the
# type used for arrays of enums in code generated by babel.
# Aguments:
#  1: The SIDL type to check 
#  2: Variable to store type in, if true
AC_DEFUN([IS_BABEL_ENUM_ARR_TYPE],[
AC_MSG_CHECKING([If SIDL enum arrays are $1 arrays])
old_CPPFLAGS="$CPPFLAGS"
old_LIBS="$LIBS"
old_LDFLAGS="$LDFLAGS"
#old_LDLIB_PATH="$LD_LIBRARY_PATH"
#old_SHLIB_PATH="$SHLIB_PATH"
#old_PATH="$PATH"
old_CC="$CC"
old_LD="$LD"
CC="./libtool --tag=CC $old_CC"
LD="./libtool --tag=CC $old_LD"
CPPFLAGS="-I$SIDL_INCDIR"
LIBS="-lsidl"
LDFLAGS="-L$SIDL_LIBDIR -R$SIDL_LIBDIR"
LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${SIDL_LIBDIR}"
SHLIB_PATH="${SHLIB_PATH}:${SIDL_LIBDIR}"
PATH="${PATH}:${SIDL_LIBDIR}"
export LD_LIBRARY_PATH
export SHLIB_PATH
AC_LANG_PUSH([C])
AC_RUN_IFELSE([
  AC_LANG_PROGRAM([
    #include <sidl.h>
    #include <sidl_$1_IOR.h>
    ],[
    struct sidl_Scope__array* enum_arr = sidl_Scope__array_create1d(1);
    struct sidl_$1__array* type_arr = sidl_$1__array_create1d(1);
    struct sidl_$1__array* cast_arr = (struct sidl_$1__array*)enum_arr;
    return !(type_arr->d_metadata.d_vtable == cast_arr->d_metadata.d_vtable);
    ])
  ],[AC_MSG_RESULT([yes]); $2=$1],[AC_MSG_RESULT([no])])
AC_LANG_POP([C])
LD="$old_LD"
CC="$old_CC"
#PATH="$old_PATH"
#SHLIB_PATH="$old_SHLIB_PATH"
#LD_LIBRARY_PATH="$old_LDLIB_PATH"
LDFLAGS="$old_LDFLAGS"
LIBS="$old_LIBS"
CPPFLAGS="$old_CPPFLAGS"
])

for type in int long short; do
  IS_BABEL_ENUM_ARR_TYPE([${type}],[BABEL_ENUM_ARR_TYPE])
  if test "x" != "x$BABEL_ENUM_ARR_TYPE"; then
    break
  fi
done

if test "x" = "x$BABEL_ENUM_ARR_TYPE"; then
  AC_MSG_WARN([Cannot detect real type of SIDL enum arrays.  Will guess based on version number.  A wrong guess may result in runtime memory corruption.])
else
  BABEL_ENUM_ARR_DEFINE="-DBABEL_ENUM_ARR_TYPE=${BABEL_ENUM_ARR_TYPE}"
fi

AC_SUBST([BABEL_ENUM_ARR_DEFINE])


################################################################################
#                               Output Files
################################################################################

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_HEADERS([iGeom/fmangle-iGeom.h])
AC_CONFIG_HEADERS([iMesh/fmangle-iMesh.h])
AC_CONFIG_HEADERS([iBase/fmangle-iBase.h])
AC_CONFIG_FILES([config.make
                 iBase/Makefile
                 iGeom/Makefile
                 iMesh/Makefile])

AC_OUTPUT
test -f Makefile || ${LN_S} ${srcdir}/Makefile Makefile

AC_MSG_RESULT([C:       $CC $CFLAGS])
AC_MSG_RESULT([C++:     $CXX $CXXFLAGS])
AC_MSG_RESULT([Fortran: $FC $FCFLAGS])
AC_MSG_RESULT([F77:     $F77 $F77FLAGS])
