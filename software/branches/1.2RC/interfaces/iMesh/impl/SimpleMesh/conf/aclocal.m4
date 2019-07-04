dnl
dnl TSTT_INIT exists to set up the TSTT_ROOTS variable, which is a
dnl collection of guesses about where TSTT implementations may live.
dnl This is an internal function.
dnl
AC_DEFUN(TSTT_CLIENT_INIT,
[
TSTTB_IMPLS="GRUMMP MOAB FMDB FronTier NWGrid"
TSTTF_IMPLS=""
TSTTG_IMPLS="MOAB FMDB"
TSTTM_IMPLS="GRUMMP MOAB FMDB FronTier NWGrid"
TSTTR_IMPLS="GRUMMP"

CURRENT_DIR=`pwd`
PARENT=`dirname $CURRENT_DIR`
GRANDPARENT=`dirname $PARENT`
GGPARENT=`dirname $GRANDPARENT`
TSTT_ROOTS="$PARENT/Implementations $GRANDPARENT/Implementations
$GGPARENT/Implementations $PARENT $GRANDPARENT $GGPARENT /usr/local
/usr/local/TSTT"
])

dnl
dnl TSTT_PATH_TSTTM is used by applications to detect (often
dnl automatically) the location of a TSTTM implementation.  The variable
dnl TSTTM_IMPLS will probably need to be extended with time.
dnl
dnl Command line args: --with-tsttm
dnl Substitution variables defined: TSTTM_ROOT
dnl
AC_DEFUN(TSTT_PATH_TSTTM,
[
AC_REQUIRE([TSTT_CLIENT_INIT])
AC_ARG_WITH(tsttm,
AS_HELP_STRING([--with-tsttm=IMPL_PATH],
[Full path of the TSTTM implementation to use.]), , [with_tsttm=_searching])
case "$with_tsttm" in
   no)  #User input error
	with_tsttm=_searching ;;
   yes) #User input error
	with_tsttm=_searching ;;
   _searching) ;;
   *)   TSTTM_ROOT="$with_tsttm" ;;
esac

if (test "$with_tsttm" = "_searching") ; then
  AC_MSG_CHECKING([for REQUIRED TSTTM implementation])
  for tstt_root in $TSTT_ROOTS ; do
    for tsttm_impl in $TSTTM_IMPLS ; do
      if (test -d $tstt_root/$tsttm_impl) ; then
	with_tsttm=$tstt_root/$tsttm_impl
	AC_MSG_RESULT([found $with_tsttm])
	break
      fi
    done
    if (test "$with_tsttm" != "_searching") ; then
      break
    fi
  done
fi
if test "$with_tsttm" = _searching ; then
   AC_MSG_RESULT([Cannot find any TSTTM implementation directory.])
   AC_MSG_ERROR([Please install an implementation and/or use --with-tsttm=TSTTM_PATH.])
fi
TSTTM_ROOT="$with_tsttm"
if test ! -f "$TSTTM_ROOT/TSTTM-Defs.inc"  ; then
   AC_MSG_RESULT([No TSTTM-Defs.inc in $TSTTM_ROOT.])
   AC_MSG_ERROR([Please install a correct TSTTM implementation.])
fi
AC_SUBST(TSTTM_ROOT)
])

dnl
dnl TSTT_PATH_TSTTF is used by applications to detect (often
dnl automatically) the location of a TSTTF implementation.  The variable
dnl TSTTF_IMPLS will probably need to be extended with time.
dnl
dnl Command line args: --with-tsttf
dnl Substitution variables defined: TSTTF_ROOT
dnl
AC_DEFUN(TSTT_PATH_TSTTF,
[
AC_REQUIRE([TSTT_CLIENT_INIT])
AC_ARG_WITH(tsttf,
AS_HELP_STRING([--with-tsttf=IMPL_PATH],
[Full path of the TSTTF implementation to use.]), , [with_tsttf=_searching])
case "$with_tsttf" in
   no)  #User input error
	with_tsttf=_searching ;;
   yes) #User input error
	with_tsttf=_searching ;;
   _searching) ;;
   *)   TSTTF_ROOT="$with_tsttf" ;;
esac

if (test "$with_tsttf" = "_searching") ; then
  AC_MSG_CHECKING([for REQUIRED TSTTF implementation])
  for tstt_root in $TSTT_ROOTS ; do
    for tsttf_impl in $TSTTF_IMPLS ; do
      if (test -d $tstt_root/$tsttf_impl) ; then
	with_tsttf=$tstt_root/$tsttf_impl
	AC_MSG_RESULT([found $with_tsttf])
	break
      fi
    done
    if (test "$with_tsttf" != "_searching") ; then
      break
    fi
  done
fi
if test "$with_tsttf" = _searching ; then
   AC_MSG_RESULT([Cannot find any TSTTF implementation directory.])
   AC_MSG_ERROR([Please install an implementation and/or use --with-tsttf=TSTTF_PATH.])
fi
TSTTF_ROOT="$with_tsttf"
if test ! -f "$TSTTF_ROOT/TSTTF-Defs.inc"  ; then
   AC_MSG_RESULT([No TSTTF-Defs.inc in $TSTTF_ROOT.])
   AC_MSG_ERROR([Please install a correct TSTTF implementation.])
fi
AC_SUBST(TSTTF_ROOT)
])

dnl
dnl TSTT_PATH_TSTTG is used by applications to detect (often
dnl automatically) the location of a TSTTG implementation.  The variable
dnl TSTTG_IMPLS will probably need to be extended with time.
dnl
dnl Command line args: --with-tsttg
dnl Substitution variables defined: TSTTG_ROOT
dnl
AC_DEFUN(TSTT_PATH_TSTTG,
[
AC_REQUIRE([TSTT_CLIENT_INIT])
AC_ARG_WITH(tsttg,
AS_HELP_STRING([--with-tsttg=IMPL_PATH],
[Full path of the TSTTG implementation to use.]), , [with_tsttg=_searching])
case "$with_tsttg" in
   no)  #User input error
	with_tsttg=_searching ;;
   yes) #User input error
	with_tsttg=_searching ;;
   _searching) ;;
   *)   TSTTG_ROOT="$with_tsttg" ;;
esac

if (test "$with_tsttg" = "_searching") ; then
  AC_MSG_CHECKING([for REQUIRED TSTTG implementation])
  for tstt_root in $TSTT_ROOTS ; do
    for tsttg_impl in $TSTTG_IMPLS ; do
      if (test -d $tstt_root/$tsttg_impl) ; then
	with_tsttg=$tstt_root/$tsttg_impl
	AC_MSG_RESULT([found $with_tsttg])
	break
      fi
    done
    if (test "$with_tsttg" != "_searching") ; then
      break
    fi
  done
fi
if test "$with_tsttg" = _searching ; then
   AC_MSG_RESULT([Cannot find any TSTTG implementation directory.])
   AC_MSG_ERROR([Please install an implementation and/or use --with-tsttg=TSTTG_PATH.])
fi
TSTTG_ROOT="$with_tsttg"
if test ! -f "$TSTTG_ROOT/TSTTG-Defs.inc"  ; then
   AC_MSG_RESULT([No TSTTG-Defs.inc in $TSTTG_ROOT.])
   AC_MSG_ERROR([Please install a correct TSTTG implementation.])
fi
AC_SUBST(TSTTG_ROOT)
])

dnl
dnl TSTT_PATH_TSTTR is used by applications to detect (often
dnl automatically) the location of a TSTTR implementation.  The variable
dnl TSTTR_IMPLS will probably need to be extended with time.
dnl
dnl Command line args: --with-tsttr
dnl Substitution variables defined: TSTTR_ROOT
dnl
AC_DEFUN(TSTT_PATH_TSTTR,
[
AC_REQUIRE([TSTT_CLIENT_INIT])
AC_ARG_WITH(tsttr,
AS_HELP_STRING([--with-tsttr=IMPL_PATH],
[Full path of the TSTTR implementation to use.]), , [with_tsttr=_searching])
case "$with_tsttr" in
   no)  #User input error
	with_tsttr=_searching ;;
   yes) #User input error
	with_tsttr=_searching ;;
   _searching) ;;
   *)   TSTTR_ROOT="$with_tsttr" ;;
esac

if (test "$with_tsttr" = "_searching") ; then
  AC_MSG_CHECKING([for REQUIRED TSTTR implementation])
  for tstt_root in $TSTT_ROOTS ; do
    for tsttr_impl in $TSTTR_IMPLS ; do
      if (test -d $tstt_root/$tsttr_impl) ; then
	with_tsttr=$tstt_root/$tsttr_impl
	AC_MSG_RESULT([found $with_tsttr])
	break
      fi
    done
    if (test "$with_tsttr" != "_searching") ; then
      break
    fi
  done
fi
if test "$with_tsttr" = _searching ; then
   AC_MSG_RESULT([Cannot find any TSTTR implementation directory.])
   AC_MSG_ERROR([Please install an implementation and/or use --with-tsttr=TSTTR_PATH.])
fi
TSTTR_ROOT="$with_tsttr"
if test ! -f "$TSTTR_ROOT/TSTTR-Defs.inc"  ; then
   AC_MSG_RESULT([No TSTTR-Defs.inc in $TSTTR_ROOT.])
   AC_MSG_ERROR([Please install a correct TSTTR implementation.])
fi
AC_SUBST(TSTTR_ROOT)
])

dnl
dnl TSTT_PATH_TSTTB is used by applications to detect (often
dnl automatically) the location of a TSTTB implementation.  The variable
dnl TSTTB_IMPLS will probably need to be extended with time.
dnl
dnl Command line args: --with-tsttb
dnl Substitution variables defined: TSTTB_ROOT
dnl
AC_DEFUN(TSTT_PATH_TSTTB,
[
AC_REQUIRE([TSTT_CLIENT_INIT])
AC_ARG_WITH(tsttb,
AS_HELP_STRING([--with-tsttb=IMPL_PATH],
[Full path of the TSTTB implementation to use.]), , [with_tsttb=_searching])
case "$with_tsttb" in
   no)  #User input error
	with_tsttb=_searching ;;
   yes) #User input error
	with_tsttb=_searching ;;
   _searching) ;;
   *)   TSTTB_ROOT="$with_tsttb" ;;
esac

if (test "$with_tsttb" = "_searching") ; then
  AC_MSG_CHECKING([for REQUIRED TSTTB implementation])
  for tstt_root in $TSTT_ROOTS ; do
    for tsttb_impl in $TSTTB_IMPLS ; do
      if (test -d $tstt_root/$tsttb_impl) ; then
	with_tsttb=$tstt_root/$tsttb_impl
	AC_MSG_RESULT([found $with_tsttb])
	break
      fi
    done
    if (test "$with_tsttb" != "_searching") ; then
      break
    fi
  done
fi
if test "$with_tsttb" = _searching ; then
   AC_MSG_RESULT([Cannot find any TSTTB implementation directory.])
   AC_MSG_ERROR([Please install an implementation and/or use --with-tsttb=TSTTB_PATH.])
fi
TSTTB_ROOT="$with_tsttb"
if test ! -f "$TSTTB_ROOT/TSTTB-Defs.inc"  ; then
   AC_MSG_RESULT([No TSTTB-Defs.inc in $TSTTB_ROOT.])
   AC_MSG_ERROR([Please install a correct TSTTB implementation.])
fi
AC_SUBST(TSTTB_ROOT)
])

dnl
dnl One stop shopping for most/all of the programs that one is likely to
dnl need to confirm the incantations for in building an implementation
dnl or application, including checking Babel stuff.
dnl
dnl Checks for: awk, egrep, C compiler, Babel, install, ln -s, ranlib
dnl Determines which client languages are requested, then checks as
dnl needed for C++, F77, F90, java, and python.
dnl
dnl Including various calls, we have:
dnl
dnl Command line args: --enable-all-clients --enable-client-LANG
dnl Substitution variables include: CC, CFLAGS, CXX, CXXFLAGS, FC,
dnl FCFLAGS, FCLIBS, F77, FFLAGS, FLIBS, JAVAC, JAVA_HOME, PYTHON,
dnl AWK, EGREP, INSTALL, LN_S, RANLIB, CLIENT_LANGS
dnl
dnl For Babel stuff, see TSTT_PROG_BABEL
dnl
dnl The only non-standard/non-obvious one here is CLIENT_LANGS, which
dnl contains a list of all languages for which clients are requested.
dnl This can be used in a Makefile for loop to iterate through each
dnl language.
dnl
AC_DEFUN(TSTT_PROG_CHECKS,
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

# Check Babel next, so that we can check for language validity (need the
# Babel include path for that).
TSTT_PROG_BABEL

# Now queue up the client languages
TSTT_CLIENT_LANGS

if (test "$TSTT_CLIENT_C" = "yes") ; then
  CLIENT_LANGS="C"
fi

if (test "$TSTT_TEST_CXX" = "yes") ; then
  AC_PROG_CXXCPP
  AC_PROG_CXX
  if (test "$TSTT_CLIENT_CXX" = "yes") ; then
    CLIENT_LANGS="$CLIENT_LANGS C++"
  fi
fi

if (test "$TSTT_TEST_F90" = "yes") ; then
  AC_PROG_FC
  AC_PROG_FC_C_O
  AC_FC_LIBRARY_LDFLAGS
  # Does the compiler require anything special for files with particular
  # extensions?  Creates variables FCFLAGS_$ext, which may be empty.
  AC_LANG_PUSH(Fortran)
  AC_FC_SRCEXT(f90)
  AC_FC_SRCEXT(F90)
  if (test "$TSTT_CLIENT_F90" = "yes") ; then
    CLIENT_LANGS="$CLIENT_LANGS F90"
  fi
  AC_LANG_POP(Fortran)
fi

if (test "$TSTT_TEST_F77" = "yes") ; then
  AC_PROG_F77
  AC_PROG_F77_C_O
  AC_F77_LIBRARY_LDFLAGS
  if (test "$TSTT_CLIENT_F77" = "yes") ; then
    CLIENT_LANGS="$CLIENT_LANGS F77"
  fi
fi

if (test "$TSTT_TEST_JAVA" = "yes") ; then
  TSTT_PROG_JAVA
  if (test "X$JAVAC" != "X" -a  "$TSTT_CLIENT_JAVA" = "yes") ; then
    CLIENT_LANGS="$CLIENT_LANGS java"
  fi
fi

if (test "$TSTT_TEST_PYTHON" = "yes") ; then
  echo "checking for python (simplistic test)..."
  AC_PATH_PROG(PYTHON, python, false)
  if (test "X$PYTHON" != "X") ; then
    if (test "$TSTT_CLIENT_PYTHON" = "yes") ; then
      CLIENT_LANGS="$CLIENT_LANGS python"
    fi
  fi
  AC_SUBST(PYTHON)
fi
echo successfully configured for clients in $CLIENT_LANGS
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
])

dnl
dnl Locates a Babel implementation (using a user path if specified).
dnl If a user path is given, the default path is -not- checked at
dnl all.  The SIDL library is added to the LIBS substitution variable.
dnl
dnl Also, determines the Babel version in use.  This is only very
dnl occasionally useful to know.
dnl
dnl Defines a configure option (--with-babel).
dnl
dnl Provides output variables:
dnl    BABEL          the full path for the executable
dnl    BABEL_ROOT     partial path (BABEL = BABEL_ROOT/bin/babel
dnl    BABEL_VERSION  0.10.2, for instance
dnl    BABEL_SERIAL   1001002, for the same case; this is potentially useful
dnl                     for situations where different babel versions
dnl                     have different behavior: you can easily check
dnl                     for versions >= 0.10.0, for instance
dnl
dnl Also autoinserts BABEL_VERSION and BABEL_SERIAL into a configuration
dnl header, if you have one.
dnl
AC_DEFUN(TSTT_PROG_BABEL,
[
AC_REQUIRE([AC_PROG_AWK])
AC_ARG_WITH(babel,
AS_HELP_STRING([--with-babel=PATH],
[specify partial path for Babel (PATH/bin/babel)]))
case "$with_babel" in
   ""|no|yes)  #No user path specified; check default path
	    AC_PATH_PROG(BABEL, babel --version, false, $PATH) ;;
   *)       #User specified a path; check only there
	    BABEL_ROOT=$with_babel
	    AC_PATH_PROG(BABEL, babel --version, false, $BABEL_ROOT/bin) ;;
esac

if (test "$BABEL" = "false") ; then
   AC_MSG_ERROR(Babel not found.  use --with-babel=/path/of/babel to enable it)
fi

dnl Definitely found babel, or we would not be here.

dnl Make sure to set BABEL_ROOT even if Babel is in the user path!
if (test "$BABEL_ROOT" = "") ; then
  BABEL_BIN_DIR=`dirname $BABEL`
  BABEL_ROOT=`dirname $BABEL_BIN_DIR`
fi

dnl Get the Babel version number
BABEL_VERSION=`$BABEL --version | $AWK '/Babel version .*/ {print \$[3];}'`
echo "using babel version number $BABEL_VERSION" 1>&6


dnl This serial number makes it easy to check whether you've got a
dnl version higher than some level; the 1 is added at the beginning so
dnl that these are interpreted as decimal numbers, not octal.
BABEL_SERIAL=`echo $BABEL_VERSION | $AWK -F . '{printf("1%02d%02d%02d", \$[1], \$[2], \$[3]);}'`
echo "using babel version serial number $BABEL_SERIAL" 1>&6

AC_DEFINE_UNQUOTED(BABEL_VERSION, "$BABEL_VERSION")
AC_DEFINE_UNQUOTED(BABEL_SERIAL, $BABEL_SERIAL)

AC_SUBST(BABEL)
AC_SUBST(BABEL_ROOT)
AC_SUBST(BABEL_VERSION)
AC_SUBST(BABEL_SERIAL)

dnl Checks for Babel/SIDL header files
CFLAGS="$CFLAGS -I$BABEL_ROOT/include"
CPPFLAGS="$CPPFLAGS -I$BABEL_ROOT/include"
AC_CHECK_HEADERS(babel_config.h sidl.h,,
	     [echo ;
	      echo "ERROR:  Can not find necessary headers in $BABEL_ROOT/include!" ;
	      echo ; exit 1])

dnl Checks for SIDL libraries.
LIBS="$LIBS -L$BABEL_ROOT/lib"
AC_CHECK_LIB(sidl, sidl_BaseClass__externals,found_sidl=yes,found_sidl=no,-ldl)
if (test "$found_sidl" = "yes") ; then
  AC_CHECK_LIB(sidl, sidl_BaseClass__externals,found_sidl=yes,found_sidl=no,-ldl -lxml2)
fi
if (test "$found_sidl" = "no") ; then
  echo
  echo "ERROR:  Must have SIDL libraries in $BABEL_ROOT/lib!"
  echo
fi

STATIC_WILL_FAIL=no
AC_CHECK_LIB(z, gzopen,, STATIC_WILL_FAIL=yes)
AC_CHECK_LIB(xml2, xmlInitParser,, STATIC_WILL_FAIL=yes,-lz)
AC_CHECK_LIB(dl, dlclose,, STATIC_WILL_FAIL=yes,-lxml2 -lz)
if (test "$STATIC_WILL_FAIL" = "yes") ; then
  AC_MSG_WARN(Static linkage with sidl will likely require adding libraries in the makefile)]
fi
)
SIDL_STATIC_LIBS="-ldl -lxml2 -lz"
AC_SUBST(SIDL_STATIC_LIBS)
] dnl End of body of TSTT_PROG_BABEL
)

dnl Downloads a separate package (like a mesh database back-end or some
dnl uncommon external tool).
dnl
dnl You probably also want a rule that causes the tarball to be
dnl converted into a compiled distribution; the GRUMMP implementation
dnl has a working example.
dnl
dnl The function takes four arguments: 
dnl  - the path name where the package should be put (full or relative)
dnl  - the machine where the distribution is found
dnl  - the pathname on that machine's anon ftp service
dnl  - whether the tar file has the customary top-level directory on unpacking
dnl
EXTERNAL_PACKAGES=""
AC_SUBST(EXTERNAL_PACKAGES)
AC_DEFUN(TSTT_FETCH,
[
echo Fetching $1 from ftp://$2/$3
DISTRO_MACHINE=$2
FTP_REL_PATH=$3
MOVE_UP=$4
if (test -z "$MOVE_UP") ; then
  MOVE_UP=yes
fi
TGZ_NAME=`basename $FTP_REL_PATH`

if (test ! -d $1) ; then
  AC_PATH_PROG(WGET, wget)
  if (test "X$WGET" != "X") ; then
    echo "wget-ing `basename $FTP_REL_PATH` from $DISTRO_MACHINE..."
    $WGET --passive ftp://$DISTRO_MACHINE/$FTP_REL_PATH
  fi
  if (test ! -f $SHORT_NAME) ; then
    AC_PATH_PROG(FTP, ftp)
    if (test "X$FTP" != "X") ; then
      echo "starting anon ftp to get $FTP_REL_PATH"
      ftp $DISTRO_MACHINE
    fi
  fi
  if (test -f $TGZ_NAME) ; then
    gunzip $TGZ_NAME
    TAR_NAME=`echo $TGZ_NAME | sed 's/.gz//'`
    mkdir -p $1_tmp $1
    PWD=`pwd`
    cd $1_tmp
    tar -xf ../$TAR_NAME
    cd ..
    if (test "X$MOVE_UP" = "Xyes") ; then
      mv $1_tmp/*/* $1
    else
      mv $1_tmp/* $1
    fi
    rm -rf $1_tmp $TAR_NAME
  else
    AC_MSG_ERROR(["Get $TGZ_NAME from ftp://$DISTRO_MACHINE/$FTP_REL_PATH
Then re-run configure."])
  fi
fi
# If the thing now exists, then prepare to configure and build it.
if (test -d $1) ; then
  EXTERNAL_PACKAGES="$EXTERNAL_PACKAGES $1"
fi
]
)

dnl
dnl Checks the Babel config file to determine whether the language
dnl specified in arg 1 is enabled (sensible values: CXX, FORTRAN77,
dnl FORTRAN90, JAVA, PYTHON).  Returns yes or no in arg 2.
dnl
AC_DEFUN(TSTT_LANG_ENABLED,
[
  AC_MSG_CHECKING(for $1 Babel support)
  $2=yes
  tmp=`$EGREP 'define $1_DISABLED' $BABEL_ROOT/include/babel_config.h`
  if (test "$tmp" = "#define $1_DISABLED 1") ; then
    $2=no
  fi
  AC_MSG_RESULT($$2)
])

dnl
dnl Checks all possible languages for Babel support.  Also, pukes if the
dnl server is written in a disabled language.
dnl
AC_DEFUN(TSTT_LANG_ENABLED_ALL,
[
dnl Must call TSTT_SERVER_LANG first!
  TSTT_LANG_ENABLED(CXX, TSTT_CXX_ENABLED)
  TSTT_LANG_ENABLED(FORTRAN77, TSTT_F77_ENABLED)
  TSTT_LANG_ENABLED(FORTRAN90, TSTT_F90_ENABLED)
  TSTT_LANG_ENABLED(JAVA, TSTT_JAVA_ENABLED)
  TSTT_LANG_ENABLED(PYTHON, TSTT_PYTHON_ENABLED)

  #If the server is in a disabled language, panic!
  if (test "$TSTT_CXX_ENABLED" = "no" -a "$SERVER_LANG" = "C++") ; then
    AC_MSG_ERROR(C++ server but no Babel support for C++, 1)
  fi
  if (test "$TSTT_F77_ENABLED" = "no" -a "$SERVER_LANG" = "F77") ; then
    AC_MSG_ERROR(F77 server but no Babel support for F77, 1)
  fi
  if (test "$TSTT_F90_ENABLED" = "no" -a "$SERVER_LANG" = "F90") ; then
    AC_MSG_ERROR(F90 server but no Babel support for F90, 1)
  fi
  if (test "$TSTT_JAVA_ENABLED" = "no" -a "$SERVER_LANG" = "java") ; then
    AC_MSG_ERROR(java server but no Babel support for java, 1)
  fi
  if (test "$TSTT_PYTHON_ENABLED" = "no" -a "$SERVER_LANG" = "python") ; then
    AC_MSG_ERROR(python server but no Babel support for python, 1)
  fi
])

dnl
dnl Mandatory for implementations.  Sets the SERVER_LANG substitution
dnl variable, and confirms that a legal choice was given.  Legal
dnl choices:
dnl   C, C++, CXX, F77, F90, java, python (these are case insensitive)
dnl
AC_DEFUN(TSTT_SERVER_LANG,
[
AC_SUBST(SERVER_LANG)
SERVER_LANG=`echo $1 | sed 'y/cfxXJAVPYTHON/CF++javpython/'`
case "$SERVER_LANG" in
  "C") ;;
  "C++") ;;
  "F77") ;;
  "F90") ;;
  "java") ;;
  "python") ;;
  *)    AC_MSG_ERROR("invalid server language $1 specified", 1);;
esac
echo server written in $SERVER_LANG
])

dnl
dnl Through command line options, identifies all requested client
dnl languages.  Clients may not be built for all of these if Babel
dnl support is missing for them.  Announces which languages will be
dnl tested for (including the server language, so TSTT_SERVER_LANG must
dnl be called first).
dnl
dnl Command line args: --enable-all-clients, --enable-client-LANG and
dnl their disable counterparts.
dnl
dnl Substitution variables:  CLIENT_LANGS is defined here, but set in
dnl TSTT_PROG_CHECK, after determining Babel support and compiler
dnl specifics.
dnl
AC_DEFUN(TSTT_CLIENT_LANGS,
[
TSTT_LANG_ENABLED_ALL

TSTT_TEST_C=no
TSTT_TEST_CXX=no
TSTT_TEST_F77=no
TSTT_TEST_F90=no
TSTT_TEST_JAVA=no
TSTT_TEST_PYTHON=no
AC_SUBST(CLIENT_LANGS)
AC_ARG_ENABLE(all-clients,
AS_HELP_STRING([--enable-all-clients],
[creates client libraries for C, C++, F77, F90, java, and python]),
[case "$enableval" in
  "yes")  TSTT_CLIENT_C=yes
	  TSTT_CLIENT_CXX=yes
	  TSTT_CLIENT_F77=yes
	  TSTT_CLIENT_F90=yes
	  TSTT_CLIENT_JAVA=yes
	  TSTT_CLIENT_PYTHON=yes ;;
  "no")   TSTT_CLIENT_C=no
	  TSTT_CLIENT_CXX=no
	  TSTT_CLIENT_F77=no
	  TSTT_CLIENT_F90=no
	  TSTT_CLIENT_JAVA=no
	  TSTT_CLIENT_PYTHON=no ;;
esac
])
AC_ARG_ENABLE(C-client,
AS_HELP_STRING([--enable-C-client],[creates client libraries for C]),
[if (test "$enableval" = "yes") ; then
  TSTT_CLIENT_C=yes
else
  TSTT_CLIENT_C=no
fi]
)
AC_ARG_ENABLE(CXX-client,
AS_HELP_STRING([--enable-CXX-client],[creates client libraries for C++]),
[if (test "$enableval" = "yes") ; then
  TSTT_CLIENT_CXX=yes
else
  TSTT_CLIENT_CXX=no
fi]
)
AC_ARG_ENABLE(F77-client,
AS_HELP_STRING([--enable-F77-client],[creates client libraries for F77]),
[if (test "$enableval" = "yes") ; then
  TSTT_CLIENT_F77=yes
else
  TSTT_CLIENT_F77=no
fi]
)
AC_ARG_ENABLE(F90-client,
AS_HELP_STRING([--enable-F90-client],[creates client libraries for F90]),
[if (test "$enableval" = "yes") ; then
  TSTT_CLIENT_F90=yes
else
  TSTT_CLIENT_F90=no
fi]
)
AC_ARG_ENABLE(java-client,
AS_HELP_STRING([--enable-java-client],[creates client libraries for java]),
[if (test "$enableval" = "yes") ; then
  TSTT_CLIENT_JAVA=yes
else
  TSTT_CLIENT_JAVA=no
fi]
)
AC_ARG_ENABLE(python-client,
AS_HELP_STRING([--enable-python-client],[creates client libraries for python]),
[if (test "$enableval" = "yes") ; then
  TSTT_CLIENT_PYTHON=yes
else
  TSTT_CLIENT_PYTHON=no
fi]
)

if (test "$TSTT_CXX_ENABLED" = "no" -a "$TSTT_CLIENT_CXX" = "yes") ; then
  AC_MSG_WARN(C++ client disabled due to lack of Babel support)
  TSTT_CLIENT_CXX=no
fi
if (test "$TSTT_F77_ENABLED" = "no" -a "$TSTT_CLIENT_F77" = "yes") ; then
  AC_MSG_WARN(F77 client disabled due to lack of Babel support)
  TSTT_CLIENT_F77=no
fi
if (test "$TSTT_F90_ENABLED" = "no" -a "$TSTT_CLIENT_F90" = "yes") ; then
  AC_MSG_WARN(F90 client disabled due to lack of Babel support)
  TSTT_CLIENT_F90=no
fi
if (test "$TSTT_JAVA_ENABLED" = "no" -a "$TSTT_CLIENT_JAVA" = "yes") ; then
  AC_MSG_WARN(java client disabled due to lack of Babel support)
  TSTT_CLIENT_JAVA=no
fi
if (test "$TSTT_PYTHON_ENABLED" = "no" -a "$TSTT_CLIENT_PYTHON" = "yes") ; then
  AC_MSG_WARN(python client disabled due to lack of Babel support)
  TSTT_CLIENT_PYTHON=no
fi

# Need to test for a language if it's the server lang or a valid client lang
if (test "$TSTT_CLIENT_CXX" = "yes" -o "$SERVER_LANG" = "C++") ; then
  TSTT_TEST_CXX=yes
fi
if (test "$TSTT_CLIENT_F77" = "yes" -o "$SERVER_LANG" = "F77") ; then
  TSTT_TEST_F77=yes
fi
if (test "$TSTT_CLIENT_F90" = "yes" -o "$SERVER_LANG" = "F90") ; then
  TSTT_TEST_F90=yes
fi
if (test "$TSTT_CLIENT_JAVA" = "yes" -o "$SERVER_LANG" = "java") ; then
  TSTT_TEST_JAVA=yes
fi
if (test "$TSTT_CLIENT_PYTHON" = "yes" -o "$SERVER_LANG" = "python") ; then
  TSTT_TEST_PYTHON=yes
fi

# Announce what we're testing for.
echo $ECHO_N "will check for languages: C "
if (test "$TSTT_TEST_CXX" = "yes") ; then
  echo $ECHO_N "C++ "
fi
if (test "$TSTT_TEST_F77" = "yes") ; then
  echo $ECHO_N "F77 "
fi
if (test "$TSTT_TEST_F90" = "yes") ; then
  echo $ECHO_N "F90 "
fi
if (test "$TSTT_TEST_JAVA" = "yes") ; then
  echo $ECHO_N "java "
fi
if (test "$TSTT_TEST_PYTHON" = "yes") ; then
  echo $ECHO_N "python "
fi
echo " "
]
)

dnl
dnl Find an OPTIONAL jdk1.2.  A tweak on a CCA configure macro.
dnl
dnl Command line arg: --with-java
dnl Configure substitution variables:  JAVAC, JAVA_HOME
dnl
AC_DEFUN(TSTT_PROG_JAVA,
[
# jdk for java2/swing
# This list of guesses is pretty likely to be terrible; better hope
# java's in the user's path.
TSTT_JAVA_DIRS="$JAVA_HOME /usr/local/jdk $HOME/jdk /usr/jdk /opt/local/jdk
   /usr/java/jdk /usr/local/java/jdk /opt/java/jdk /opt/local/java/jdk
   $prefix/jdk"

if test "$target_os" = "macosx" ; then
   # MacOSX comes with complete Java package installed
   AC_PATH_PROG(JAVAC, javac, , /usr/bin)
   JAVA_HOME="/usr"
else
   AC_ARG_WITH(javac,
     AS_HELP_STRING([--with-javac=JAVA_HOME],
		    [Partial path for javac (JAVA_HOME/bin/java)]),
     , [with_java=_searching])
   case "$with_java" in
       no)
	   AC_MSG_WARN([Option '--without-java' may result in reduced functionality.])
	   ;;
       yes)
	   with_java=_searching
	   ;;
       *)
	   ;;
   esac

   if test "$with_java" = _searching ; then
       AC_PATH_PROG(JAVAC, javac)
       if (test "X$JAVAC" = "X") ; then
	 for jhome in $TSTT_JAVA_DIRS ; do
	   AC_PATH_PROG(JAVAC, javac, , $jhome/bin)
	   if test ! -z "$JAVAC" ; then
	       AC_MSG_RESULT([found $JAVAC.])
	       break
	   fi
	 done
	 if test -z "$JAVAC" ; then
	   AC_MSG_RESULT([not found])
	 fi
       fi
   else
       AC_PATH_PROG(JAVAC, javac, , $with_java/bin)
   fi
   JAVA_BIN_DIR=`dirname $JAVAC`
   JAVA_HOME=`dirname $JAVA_BIN_DIR`
   # find the jni header to get version
   if (test "X$JAVAC" != "X") ; then
     java_header_file=$JAVA_HOME/include/jni.h
     AC_MSG_CHECKING([for java jni.h header])
     if (test -f $java_header_file) ; then
       AC_MSG_RESULT([found])
     else
       AC_MSG_RESULT([not found])
     fi
     AC_MSG_CHECKING([for java jni.h version 1.2 compatibility])
     JAVA_V1_2=`grep "define JDK1_2" $java_header_file | sed 's/#define //'`
     if (test "X$JAVA_V1_2" = "XJDK1_2"); then
       AC_MSG_RESULT(yes)
     else
       AC_MSG_RESULT(no)
       AC_MSG_WARN([JDK {$JAVA_HOME} is the wrong version, apparently.
		     Use --with-java=JAVA_HOME for a jdk1.2 or compatible version,])
     fi
   fi
 fi
 AC_SUBST(JAVAC)
 AC_SUBST(JAVA_HOME)
]) dnl End of TSTT_PROG_JAVA

dnl
dnl The following function sets the right incantations for building
dnl static and shared libraries on most machines.  Not likely to be as
dnl good as using libtool, by a long shot, nor is it likely to work well
dnl for C and Fortran shared libs, in general.  But it's a start.
dnl
dnl configure substitution variables:  AR_STATIC, AR_SHARED_CXX
dnl
AC_DEFUN(TSTT_PROG_BUILD_LIB,
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
	      AR_SHARED_CXX="$CXX -fPIC -shared -o"
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
	echo "don't know how to make a shared library on this machine..."
	echo "will use static libraries instead"
	AR_SHARED_CXX="false"
	libsuf=a
	;;
esac
if (test "$AR_SHARED_CXX" != "false") ; then
  AC_MSG_RESULT($AR_SHARED_CXX)
fi
]) dnl End of TSTT_PROG_BUILD_LIB

AC_DEFUN(TSTT_BUILD_TSTTB,
[
AC_ARG_ENABLE([tsttb],
AS_HELP_STRING([--enable-tsttb],
[build TSTT base interface server and clients]),,enableval=yes)
if (test "X$enableval" = "Xyes") ; then
  SIDL_FILE=TSTTB.sidl
  AC_MSG_CHECKING(for $SIDL_FILE)
  if (test -f $SIDL_FILE) ; then
    AC_CONFIG_FILES(TSTTB-Defs.inc TSTTB/server/Makefile)
    BUILD_INTERFACES="$BUILD_INTERFACES TSTTB"
    TSTT_DEF_FILES="$TSTT_DEF_FILES TSTTB-Defs.inc"
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN("disabled TSTTB against user request")
  fi
fi
])

AC_DEFUN(TSTT_BUILD_TSTTM,
[
AC_ARG_ENABLE([tsttm],
AS_HELP_STRING([--enable-tsttm],
[build TSTT mesh interface server and clients]))
if (test "X$enable_tsttm" = "Xyes") ; then
  SIDL_FILE=TSTTM_${IMPL_NAME}/TSTTM_${IMPL_NAME}.sidl
  AC_MSG_CHECKING(for $SIDL_FILE)
  if (test -f $SIDL_FILE) ; then
    AC_MSG_RESULT(yes)
    AC_CONFIG_FILES(TSTTM-Defs.inc)
    AC_CONFIG_FILES(TSTTM/server/Makefile TSTTM_${IMPL_NAME}/server/Makefile)
    BUILD_INTERFACES="$BUILD_INTERFACES TSTTM"
    TSTT_DEF_FILES="$TSTT_DEF_FILES TSTTM-Defs.inc"
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN("disabled TSTTM against user request")
  fi
fi
])

AC_DEFUN(TSTT_BUILD_TSTTR,
[
AC_ARG_ENABLE([tsttr],
AS_HELP_STRING([--enable-tsttr],
[build TSTT relations interface server and clients]))
if (test "X$enable_tsttr" = "Xyes") ; then
  SIDL_FILE=TSTTR_${IMPL_NAME}/TSTTR_${IMPL_NAME}.sidl
  AC_MSG_CHECKING(for $SIDL_FILE)
  if (test -f $SIDL_FILE) ; then
    AC_MSG_RESULT(yes)
    AC_CONFIG_FILES(TSTTR-Defs.inc)
    AC_CONFIG_FILES(TSTTR/server/Makefile TSTTR_${IMPL_NAME}/server/Makefile)
    BUILD_INTERFACES="$BUILD_INTERFACES TSTTR"
    TSTT_DEF_FILES="$TSTT_DEF_FILES TSTTR-Defs.inc"
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN("disabled TSTTR against user request")
  fi
fi
])

AC_DEFUN(TSTT_BUILD_TSTTG,
[
AC_ARG_ENABLE([tsttg],
AS_HELP_STRING([--enable-tsttg],
[build TSTT geometry interface server and clients]))
if (test "X$enable_tsttg" = "Xyes") ; then
  SIDL_FILE=TSTTG_${IMPL_NAME}/TSTTG_${IMPL_NAME}.sidl
  AC_MSG_CHECKING(for $SIDL_FILE)
  if (test -f $SIDL_FILE) ; then
    AC_MSG_RESULT(yes)
    AC_CONFIG_FILES(TSTTG-Defs.inc)
    AC_CONFIG_FILES(TSTTG/server/Makefile TSTTG_${IMPL_NAME}/server/Makefile)
    BUILD_INTERFACES="$BUILD_INTERFACES TSTTG"
    TSTT_DEF_FILES="$TSTT_DEF_FILES TSTTG-Defs.inc"
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN("disabled TSTTG against user request")
  fi
fi
])

AC_DEFUN(TSTT_BUILD_TSTTF,
[
AC_ARG_ENABLE([tsttf],
AS_HELP_STRING([--enable-tsttf],
[build TSTT field interface server and clients]))
if (test "X$enable_tsttf" = "Xyes") ; then
  SIDL_FILE=TSTTF_${IMPL_NAME}/TSTTF_${IMPL_NAME}.sidl
  AC_MSG_CHECKING(for $SIDL_FILE)
  if (test -f $SIDL_FILE) ; then
    AC_MSG_RESULT(yes)
    AC_CONFIG_FILES(TSTTF-Defs.inc)
    AC_CONFIG_FILES(TSTTF/server/Makefile TSTTF_${IMPL_NAME}/server/Makefile)
    BUILD_INTERFACES="$BUILD_INTERFACES TSTTF"
    TSTT_DEF_FILES="$TSTT_DEF_FILES TSTTF-Defs.inc"
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN("disabled TSTTF against user request")
  fi
fi
])

AC_DEFUN(TSTT_BUILD_PACKAGES,
[AC_SUBST(BUILD_INTERFACES)
AC_SUBST(TSTT_DEF_FILES)

TSTT_BUILD_TSTTB
TSTT_BUILD_TSTTF
TSTT_BUILD_TSTTG
TSTT_BUILD_TSTTM
TSTT_BUILD_TSTTR
])

dnl TSTT_CONFIGURE_SERVER does all of the standard generic stuff that TSTT
dnl servers require.  It's basically a driver that contains everything
dnl that was standard in the configure.in file. 
AC_DEFUN(TSTT_CONFIGURE_SERVER,
[
AC_CONFIG_SRCDIR(TSTTB.sidl)
AC_CANONICAL_HOST

# The following list indicates which client languages are supported by
# default.  If someone ever asks about Java or python, we're likely to
# work out how to build the client-side libraries.  Otherwise, it's
# going to be a very low priority.

TSTT_CLIENT_C=yes
TSTT_CLIENT_CXX=yes
TSTT_CLIENT_F77=yes
TSTT_CLIENT_F90=yes
TSTT_CLIENT_JAVA=no
TSTT_CLIENT_PYTHON=no

TSTT_SERVER_LANG($2)
BACKEND_DIRS="$3"
AC_CONFIG_SUBDIRS($1)
AC_SUBST(BACKEND_DIRS)

IMPL_NAME=$1
IMPL_ROOT=`pwd`
AC_SUBST(IMPL_ROOT)
AC_SUBST(IMPL_NAME)

AC_PREFIX_DEFAULT(`pwd`)

TSTT_PROG_CHECKS
TSTT_BUILD_PACKAGES
TSTT_PROG_BUILD_LIB


AC_CONFIG_FILES(Makefile conf/Makefile-client conf/Makefile-server-inc conf/Makefile-package)
])

m4_include([acinclude.m4])
