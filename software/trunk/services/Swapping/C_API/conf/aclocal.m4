dnl
dnl ITAPS_INIT exists to set up the ITAPS_ROOTS variable, which is a
dnl collection of guesses about where ITAPS implementations may live.
dnl This is an internal function.
dnl
AC_DEFUN(ITAPS_CLIENT_INIT,
[
iBase_IMPLS="GRUMMP MOAB FMDB FronTier NWGrid"
iField_IMPLS=""
iGeom_IMPLS="MOAB FMDB"
iMesh_IMPLS="GRUMMP MOAB FMDB FronTier NWGrid"
iRel_IMPLS="GRUMMP"

CURRENT_DIR=`pwd`
PARENT=`dirname $CURRENT_DIR`
GRANDPARENT=`dirname $PARENT`
GGPARENT=`dirname $GRANDPARENT`
ITAPS_ROOTS="$PARENT/Implementations $GRANDPARENT/Implementations $GGPARENT/Implementations $PARENT $GRANDPARENT $GGPARENT /usr/local /usr/local/ITAPS"
])

dnl
dnl ITAPS_PATH_iMesh is used by applications to detect (often
dnl automatically) the location of a iMesh implementation.  The variable
dnl iMesh_IMPLS will probably need to be extended with time.
dnl
dnl Command line args: --with-iMesh
dnl Substitution variables defined: iMesh_ROOT
dnl
AC_DEFUN(ITAPS_PATH_iMesh,
[
  AC_REQUIRE([ITAPS_CLIENT_INIT])
  AC_ARG_WITH(iMesh,
  AS_HELP_STRING([--with-iMesh=IMPL_PATH],
    [Full path of the iMesh implementation to use.]), , [with_iMesh=_searching])
  case "$with_iMesh" in
    no)  #User input error
      with_iMesh=_searching ;;
    yes) #User input error
      with_iMesh=_searching ;;
    _searching) ;;
    *)   iMesh_ROOT="$with_iMesh" ;;
  esac

  if (test "$with_iMesh" = "_searching") ; then
    AC_MSG_CHECKING([for REQUIRED iMesh implementation])
    for itaps_root in $ITAPS_ROOTS ; do
      for iMesh_impl in $iMesh_IMPLS ; do
        if (test -d $itaps_root/$iMesh_impl) ; then
	  with_iMesh=$itaps_root/$iMesh_impl
	  AC_MSG_RESULT([found $with_iMesh])
	  break
        fi
      done
      if (test "$with_iMesh" != "_searching") ; then
        break
      fi
    done
  fi
  if test "$with_iMesh" = _searching ; then
    AC_MSG_RESULT([Cannot find any iMesh implementation directory.])
    AC_MSG_ERROR([Please install an implementation and/or use --with-iMesh=iMesh_PATH.])
  fi
  iMesh_ROOT="$with_iMesh"
  if test ! -f "$iMesh_ROOT/iMesh-Defs.inc"  ; then
    AC_MSG_RESULT([No iMesh-Defs.inc in $iMesh_ROOT.])
    AC_MSG_ERROR([Please install a correct iMesh implementation.])
  fi
  AC_SUBST(iMesh_ROOT)
])

dnl
dnl ITAPS_PATH_iField is used by applications to detect (often
dnl automatically) the location of a iField implementation.  The variable
dnl iField_IMPLS will probably need to be extended with time.
dnl
dnl Command line args: --with-iField
dnl Substitution variables defined: iField_ROOT
dnl
AC_DEFUN(ITAPS_PATH_iField,
[
AC_REQUIRE([ITAPS_CLIENT_INIT])
AC_ARG_WITH(iField,
AS_HELP_STRING([--with-iField=IMPL_PATH],
[Full path of the iField implementation to use.]), , [with_iField=_searching])
case "$with_iField" in
   no)  #User input error
	with_iField=_searching ;;
   yes) #User input error
	with_iField=_searching ;;
   _searching) ;;
   *)   iField_ROOT="$with_iField" ;;
esac

if (test "$with_iField" = "_searching") ; then
  AC_MSG_CHECKING([for REQUIRED iField implementation])
  for itaps_root in $ITAPS_ROOTS ; do
    for iField_impl in $iField_IMPLS ; do
      if (test -d $itaps_root/$iField_impl) ; then
	with_iField=$itaps_root/$iField_impl
	AC_MSG_RESULT([found $with_iField])
	break
      fi
    done
    if (test "$with_iField" != "_searching") ; then
      break
    fi
  done
fi
if test "$with_iField" = _searching ; then
   AC_MSG_RESULT([Cannot find any iField implementation directory.])
   AC_MSG_ERROR([Please install an implementation and/or use --with-iField=iField_PATH.])
fi
iField_ROOT="$with_iField"
if test ! -f "$iField_ROOT/iField-Defs.inc"  ; then
   AC_MSG_RESULT([No iField-Defs.inc in $iField_ROOT.])
   AC_MSG_ERROR([Please install a correct iField implementation.])
fi
AC_SUBST(iField_ROOT)
])

dnl
dnl ITAPS_PATH_iGeom is used by applications to detect (often
dnl automatically) the location of a iGeom implementation.  The variable
dnl iGeom_IMPLS will probably need to be extended with time.
dnl
dnl Command line args: --with-iGeom
dnl Substitution variables defined: iGeom_ROOT
dnl
AC_DEFUN(ITAPS_PATH_iGeom,
[
AC_REQUIRE([ITAPS_CLIENT_INIT])
AC_ARG_WITH(iGeom,
AS_HELP_STRING([--with-iGeom=IMPL_PATH],
[Full path of the iGeom implementation to use.]), , [with_iGeom=_searching])
case "$with_iGeom" in
   no)  #User input error
	with_iGeom=_searching ;;
   yes) #User input error
	with_iGeom=_searching ;;
   _searching) ;;
   *)   iGeom_ROOT="$with_iGeom" ;;
esac

if (test "$with_iGeom" = "_searching") ; then
  AC_MSG_CHECKING([for REQUIRED iGeom implementation])
  for itaps_root in $ITAPS_ROOTS ; do
    for iGeom_impl in $iGeom_IMPLS ; do
      if (test -d $itaps_root/$iGeom_impl) ; then
	with_iGeom=$itaps_root/$iGeom_impl
	AC_MSG_RESULT([found $with_iGeom])
	break
      fi
    done
    if (test "$with_iGeom" != "_searching") ; then
      break
    fi
  done
fi
if test "$with_iGeom" = _searching ; then
   AC_MSG_RESULT([Cannot find any iGeom implementation directory.])
   AC_MSG_ERROR([Please install an implementation and/or use --with-iGeom=iGeom_PATH.])
fi
iGeom_ROOT="$with_iGeom"
if test ! -f "$iGeom_ROOT/iGeom-Defs.inc"  ; then
   AC_MSG_RESULT([No iGeom-Defs.inc in $iGeom_ROOT.])
   AC_MSG_ERROR([Please install a correct iGeom implementation.])
fi
AC_SUBST(iGeom_ROOT)
])

dnl
dnl ITAPS_PATH_iRel is used by applications to detect (often
dnl automatically) the location of a iRel implementation.  The variable
dnl iRel_IMPLS will probably need to be extended with time.
dnl
dnl Command line args: --with-iRel
dnl Substitution variables defined: iRel_ROOT
dnl
AC_DEFUN(ITAPS_PATH_iRel,
[
AC_REQUIRE([ITAPS_CLIENT_INIT])
AC_ARG_WITH(iRel,
AS_HELP_STRING([--with-iRel=IMPL_PATH],
[Full path of the iRel implementation to use.]), , [with_iRel=_searching])
case "$with_iRel" in
   no)  #User input error
	with_iRel=_searching ;;
   yes) #User input error
	with_iRel=_searching ;;
   _searching) ;;
   *)   iRel_ROOT="$with_iRel" ;;
esac

if (test "$with_iRel" = "_searching") ; then
  AC_MSG_CHECKING([for REQUIRED iRel implementation])
  for itaps_root in $ITAPS_ROOTS ; do
    for iRel_impl in $iRel_IMPLS ; do
      if (test -d $itaps_root/$iRel_impl) ; then
	with_iRel=$itaps_root/$iRel_impl
	AC_MSG_RESULT([found $with_iRel])
	break
      fi
    done
    if (test "$with_iRel" != "_searching") ; then
      break
    fi
  done
fi
if test "$with_iRel" = _searching ; then
   AC_MSG_RESULT([Cannot find any iRel implementation directory.])
   AC_MSG_ERROR([Please install an implementation and/or use --with-iRel=iRel_PATH.])
fi
iRel_ROOT="$with_iRel"
if test ! -f "$iRel_ROOT/iRel-Defs.inc"  ; then
   AC_MSG_RESULT([No iRel-Defs.inc in $iRel_ROOT.])
   AC_MSG_ERROR([Please install a correct iRel implementation.])
fi
AC_SUBST(iRel_ROOT)
])

dnl
dnl ITAPS_PATH_iBase is used by applications to detect (often
dnl automatically) the location of a iBase implementation.  The variable
dnl iBase_IMPLS will probably need to be extended with time.
dnl
dnl Command line args: --with-iBase
dnl Substitution variables defined: iBase_ROOT
dnl
AC_DEFUN(ITAPS_PATH_iBase,
[
AC_REQUIRE([ITAPS_CLIENT_INIT])
AC_ARG_WITH(iBase,
AS_HELP_STRING([--with-iBase=IMPL_PATH],
[Full path of the iBase implementation to use.]), , [with_iBase=_searching])
case "$with_iBase" in
   no)  #User input error
	with_iBase=_searching ;;
   yes) #User input error
	with_iBase=_searching ;;
   _searching) ;;
   *)   iBase_ROOT="$with_iBase" ;;
esac

if (test "$with_iBase" = "_searching") ; then
  AC_MSG_CHECKING([for REQUIRED iBase implementation])
  for itaps_root in $ITAPS_ROOTS ; do
    for iBase_impl in $iBase_IMPLS ; do
      if (test -d $itaps_root/$iBase_impl) ; then
	with_iBase=$itaps_root/$iBase_impl
	AC_MSG_RESULT([found $with_iBase])
	break
      fi
    done
    if (test "$with_iBase" != "_searching") ; then
      break
    fi
  done
fi
if test "$with_iBase" = _searching ; then
   AC_MSG_RESULT([Cannot find any iBase implementation directory.])
   AC_MSG_ERROR([Please install an implementation and/or use --with-iBase=iBase_PATH.])
fi
iBase_ROOT="$with_iBase"
if test ! -f "$iBase_ROOT/iBase-Defs.inc"  ; then
   AC_MSG_RESULT([No iBase-Defs.inc in $iBase_ROOT.])
   AC_MSG_ERROR([Please install a correct iBase implementation.])
fi
AC_SUBST(iBase_ROOT)
])

dnl
dnl One stop shopping for most/all of the programs that one is likely to
dnl need to confirm the incantations for in building an implementation
dnl or application.
dnl
dnl Checks for: awk, egrep, C compiler, Babel, install, ln -s, ranlib
dnl Also checks for a compiler for the server language.
dnl
dnl Including various calls, we have:
dnl
dnl Substitution variables will include: 
dnl   CC, CFLAGS, CPPFLAGS, AWK, EGREP, INSTALL, LN_S, RANLIB
dnl And may include:
dnl   CXX, CXXFLAGS, FC, FCFLAGS, FCLIBS, F77, FFLAGS, FLIBS
dnl
AC_DEFUN(ITAPS_PROG_CHECKS,
[
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

  if (test "$ITAPS_TEST_CXX" = "yes") ; then
    AC_PROG_CXXCPP
    AC_PROG_CXX
  fi

  if (test "$ITAPS_TEST_F90" = "yes") ; then
    AC_PROG_FC
    AC_PROG_FC_C_O
    AC_FC_LIBRARY_LDFLAGS
    # Does the compiler require anything special for files with particular
    # extensions?  Creates variables FCFLAGS_$ext, which may be empty.
    AC_LANG_PUSH(Fortran)
    AC_FC_SRCEXT(f90)
    AC_FC_SRCEXT(F90)
    AC_LANG_POP(Fortran)
  fi

  AC_PROG_F77
  if (test "$F77" = "") ; then
    AC_MSG_WARN([No Fortran compiler found!  Using default name mangling...])
    F77_SUBST=name
  else 
    AC_PROG_F77_C_O
    AC_F77_LIBRARY_LDFLAGS
dnl Not sure why the next line is necessary, but it definitely is!
    _AC_F77_NAME_MANGLING
    AC_F77_WRAPPERS
    case $ac_cv_f77_mangling in
      "lower case, no underscore, no extra underscore")
      	   F77_SUBST="name" ;;
      "lower case, no underscore, extra underscore")
    	   F77_SUBST="name ## _" ;;
      "lower case, underscore, no extra underscore")
    	   F77_SUBST="name ## _" ;;
      "lower case, underscore, extra underscore")
    	   F77_SUBST="name ## __" ;;
      "upper case, no underscore, no extra underscore")
    	   F77_SUBST="NAME" ;;
      "upper case, no underscore, extra underscore")
    	   F77_SUBST="NAME ## _" ;;
      "upper case, underscore, no extra underscore")
    	   F77_SUBST="NAME ## _" ;;
      "upper case, underscore, extra underscore")
    	   F77_SUBST="NAME ## __" ;;
      *)
           AC_MSG_WARN([unknown Fortran 77 name-mangling scheme]);;
    esac
  fi
  AC_SUBST(F77_SUBST)

  AC_PROG_INSTALL
  AC_PROG_LN_S
  AC_PROG_MAKE_SET
  AC_PROG_RANLIB
])

dnl
dnl Mandatory for implementations.  Sets the SERVER_LANG substitution
dnl variable, and confirms that a legal choice was given.  Legal
dnl choices:
dnl   C, C++, CXX, F77, F90, java, python (these are case insensitive)
dnl
AC_DEFUN(ITAPS_SERVER_LANG,
[
ITAPS_TEST_CXX=no
ITAPS_TEST_F77=yes
ITAPS_TEST_F90=no
AC_SUBST(SERVER_LANG)
SERVER_LANG=`echo $1 | sed 'y/cfxX/CF++/'`
case "$SERVER_LANG" in
  "C")   ;;
  "C++") ITAPS_TEST_CXX=yes;;
  "F77") ITAPS_TEST_F77=yes;;
  "F90") ITAPS_TEST_F90=yes;;
  *)    AC_MSG_ERROR("invalid server language $1 specified", 1);;
esac
])

dnl
dnl The following function sets the right incantations for building
dnl static and shared libraries on most machines.  Not likely to be as
dnl good as using libtool, by a long shot, nor is it likely to work well
dnl for C and Fortran shared libs, in general.  But it's a start.
dnl
dnl configure substitution variables:  AR_STATIC, AR_SHARED_CXX
dnl
AC_DEFUN([ITAPS_PROG_BUILD_LIB],
[
AC_REQUIRE([AC_PROG_AWK])
dnl The [] in the following line prevents expansion of $1 as an arg to this
dnl function.  DO NOT REMOVE IT!
host_os_short=`echo $host_os | ${AWK} -F\. '{printf("%s\n", toupper($[]1));}' | sed 's/-/_/'`
AC_SUBST(AR_SHARED_CXX)
AC_SUBST(AR_STATIC)
AC_MSG_CHECKING(how to create static libraries)
case "$host_os_short" in
  IRIX6*)  AR_STATIC="CC -elf -shared -all -woff 131 -prelink -ptused -o" ;;
  *)       AR_STATIC="ar ruv";;
esac
AC_MSG_RESULT($AR_STATIC)
AC_MSG_CHECKING(how to create shared libraries from C++ source)
case "$host_os_short" in
  FREEBSD*) AR_SHARED_CXX="ld -Bshareable -o" ;;
  HPUX*)   AR_SHARED_CXX="$CXX -shared -o" ;; # Assume Compaq compiler
  IRIX5*)  AR_SHARED_CXX="ld -shared -all -o" ;;
  IRIX6*)  AR_SHARED_CXX="$CXX -elf -shared -all -woff 131 -prelink -ptused -o" ;;
  LINUX*)  if (test "$GCC" = "yes"); then
	      # Linux gcc
	      CXX="$CXX -fPIC"
	      CC="$CC -fPIC"
	      AR_SHARED_CXX="$CXX -shared -o"
	   else
	      if (test "$host_cpu" = "alpha"); then
		 # Linux Alpha/Compaq
		 AR_SHARED_CXX="$CXX -shared -o"
	      else
		 # Linux x86/Intel
		 AR_SHARED_CXX="$CXX -shared"
	      fi
	   fi ;;
  OSF*)    AR_SHARED_CXX="ld -shared -all -expect_unresolved \* -o" ;;
  SOLARIS2*) AR_SHARED_CXX="ld -G -o" ;;
  SUNOS4*) AR_SHARED_CXX="ld -assert pure-text -o" ;;
  AIX*)	   AR_SHARED_CXX="$CXX -qmkshrobj -o" ;;
  *)
	AC_MSG_RESULT(failed!)
	AC_MSG_NOTICE([don't know how to make a shared library on this machine...])
	AC_MSG_NOTICE([will use static libraries instead])
	AR_SHARED_CXX="false"
	libsuf=a
	;;
esac
if (test "$AR_SHARED_CXX" != "false") ; then
  AC_MSG_RESULT($AR_SHARED_CXX)
fi
]) dnl End of ITAPS_PROG_BUILD_LIB

AC_DEFUN(ITAPS_BUILD_iBase,
[
AC_ARG_ENABLE([iBase],
AS_HELP_STRING([--enable-iBase],
[build ITAPS base interface server and clients]),,enableval=yes)
if (test "X$enableval" = "Xyes") ; then
  HEADER_FILE=iBase/iBase.h
  AC_MSG_CHECKING(for $HEADER_FILE)
  if (test -f $HEADER_FILE) ; then
    AC_CONFIG_FILES(iBase-Defs.inc iBase/Makefile)
    BUILD_INTERFACES="$BUILD_INTERFACES iBase"
    ITAPS_DEF_FILES="$ITAPS_DEF_FILES iBase-Defs.inc"
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN("disabled iBase against user request")
  fi
fi
])

AC_DEFUN(ITAPS_BUILD_iMesh,
[
AC_ARG_ENABLE([iMesh],
AS_HELP_STRING([--enable-iMesh],
[build ITAPS mesh interface server and clients]))
if (test "X$enable_iMesh" = "Xyes") ; then
  HEADER_FILE=iMesh/iMesh.h
  AC_MSG_CHECKING(for $HEADER_FILE)
  if (test -f $HEADER_FILE) ; then
    AC_MSG_RESULT(yes)
    AC_CONFIG_FILES(iMesh-Defs.inc iMesh/Makefile iMesh/iMesh_protos.h)
    BUILD_INTERFACES="$BUILD_INTERFACES iMesh"
    ITAPS_DEF_FILES="$ITAPS_DEF_FILES iMesh-Defs.inc"
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN("disabled iMesh against user request")
  fi
fi
])

AC_DEFUN(ITAPS_BUILD_iRel,
[
AC_ARG_ENABLE([iRel],
AS_HELP_STRING([--enable-iRel],
[build ITAPS relations interface server and clients]))
if (test "X$enable_iRel" = "Xyes") ; then
  HEADER_FILE=iRel/iRel.h
  AC_MSG_CHECKING(for $HEADER_FILE)
  if (test -f $HEADER_FILE) ; then
    AC_MSG_RESULT(yes)
    AC_CONFIG_FILES(iRel-Defs.inc iRel/Makefile)
    BUILD_INTERFACES="$BUILD_INTERFACES iRel"
    ITAPS_DEF_FILES="$ITAPS_DEF_FILES iRel-Defs.inc"
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN("disabled iRel against user request")
  fi
fi
])

AC_DEFUN(ITAPS_BUILD_iGeom,
[
AC_ARG_ENABLE([iGeom],
AS_HELP_STRING([--enable-iGeom],
[build ITAPS geometry interface server and clients]))
if (test "X$enable_iGeom" = "Xyes") ; then
  HEADER_FILE=iGeom/iGeom.h
  AC_MSG_CHECKING(for $HEADER_FILE)
  if (test -f $HEADER_FILE) ; then
    AC_MSG_RESULT(yes)
    AC_CONFIG_FILES(iGeom-Defs.inc iGeom/Makefile)
    BUILD_INTERFACES="$BUILD_INTERFACES iGeom"
    ITAPS_DEF_FILES="$ITAPS_DEF_FILES iGeom-Defs.inc"
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN("disabled iGeom against user request")
  fi
fi
])

AC_DEFUN(ITAPS_BUILD_iField,
[
AC_ARG_ENABLE([iField],
AS_HELP_STRING([--enable-iField],
[build ITAPS field interface server and clients]))
if (test "X$enable_iField" = "Xyes") ; then
  HEADER_FILE=iField/iField.h
  AC_MSG_CHECKING(for $HEADER_FILE)
  if (test -f $HEADER_FILE) ; then
    AC_MSG_RESULT(yes)
    AC_CONFIG_FILES(iField-Defs.inc iField/Makefile)
    BUILD_INTERFACES="$BUILD_INTERFACES iField"
    ITAPS_DEF_FILES="$ITAPS_DEF_FILES iField-Defs.inc"
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN("disabled iField against user request")
  fi
fi
])

AC_DEFUN(ITAPS_BUILD_PACKAGES,
[AC_SUBST(BUILD_INTERFACES)
AC_SUBST(ITAPS_DEF_FILES)

ITAPS_BUILD_iBase
ITAPS_BUILD_iField
ITAPS_BUILD_iGeom
ITAPS_BUILD_iMesh
ITAPS_BUILD_iRel
])

dnl ITAPS_CONFIGURE_SERVER does all of the standard generic stuff that ITAPS
dnl servers require.  It's basically a driver that contains everything
dnl that was standard in the configure.in file. 
AC_DEFUN(ITAPS_CONFIGURE_SERVER,
[
AC_CONFIG_SRCDIR(iBase/iBase.h)
AC_CANONICAL_HOST

ITAPS_SERVER_LANG($2)
BACKEND_DIRS="$3"
AC_CONFIG_SUBDIRS($1)
AC_SUBST(BACKEND_DIRS)

IMPL_NAME=$1
IMPL_ROOT=`pwd`
AC_SUBST(IMPL_ROOT)
AC_SUBST(IMPL_NAME)

AC_PREFIX_DEFAULT(`pwd`)

ITAPS_PROG_CHECKS
ITAPS_BUILD_PACKAGES
ITAPS_PROG_BUILD_LIB


AC_CONFIG_FILES(Makefile conf/Makefile-include)
])

m4_include([acinclude.m4])
