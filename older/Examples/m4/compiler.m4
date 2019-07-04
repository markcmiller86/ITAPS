#######################################################################################
# Get libtool configuration variable
# Arguments:
#  libtool config tag (e.g CXX)
#  libtool variable name
#  variable in which to store result
#######################################################################################
AC_DEFUN([ITAPS_LIBTOOL_VAR], [
  $3=`./libtool --tag=$1 --config | sed -e 's/^$2=//p' -e 'd' | tr -d '"\n'`
])

#######################################################################################
# Implement checks for C and C++ compilers, with all corresponding options
#
# Sets the following variables:
#  CPP      - The C preprocessor
#  CC       - The C compiler
#  CXX      - The C++ compiler
#  CFLAGS   - C compiler flags
#  CXXFLAGS - C++ compiler flags
#  WITH_MPI - 'yes' if parallel support, 'no' otherwise
#
#######################################################################################
AC_DEFUN([ITAPS_CHECK_COMPILERS], [

  # Save these before calling AC_PROG_CC or AC_PROG_CXX
  # because those macros will modify them, and we want
  # the original user values, not the autoconf defaults.
USER_CXXFLAGS="$CXXFLAGS"
USER_CFLAGS="$CFLAGS"

  # Check for Parallel
  # Need to check this early so we can look for the correct compiler
AC_ARG_WITH( [mpi], AC_HELP_STRING([[--with-mpi(=DIR)]], [Enable parallel support]),
             [WITH_MPI=$withval],[WITH_MPI=no] )
case "x$WITH_MPI" in
  xno)
    CC_LIST="cc gcc cl egcs pgcc"
    CXX_LIST="CC aCC cxx xlC_r xlC c++ g++ pgCC gpp cc++ cl FCC KCC RCC"
    FC_LIST="gfortran ifort pgf90"
    F77_LIST="gfortran ifort pgf77"
    ;;
  xyes)
    CC_LIST="mpicc mpcc"
    CXX_LIST="mpiCC mpCC mpicxx"
    FC_LIST="mpif90"
    F77_LIST="mpif77"
    ;;
  x*)
    if test -z "$CC";then
      for prog in mpicc mpcc; do
        if test -x ${WITH_MPI}/bin/$prog; then
          CC="${WITH_MPI}/bin/$prog"
          CC_LIST="$prog"
        fi
      done
    else
      CC_LIST="$CC"
    fi
    if test -z "$CXX";then
      for prog in mpicxx mpiCC mpCC mpicxx; do
        if test -x ${WITH_MPI}/bin/$prog; then
          CXX="${WITH_MPI}/bin/$prog"
          CXX_LIST="$prog"
        fi
      done
    else
      CXX_LIST="$CXX"
    fi
    if test -z "$FC";then
      for prog in mpif90; do
        if test -x ${WITH_MPI}/bin/$prog; then
          FC="${WITH_MPI}/bin/$prog"
          FC_LIST="$prog"
        fi
      done
    else
      FC_LIST="$FC"
    fi
    if test -z "$F77";then
      for prog in mpif77; do
        if test -x ${WITH_MPI}/bin/$prog; then
          F77="${WITH_MPI}/bin/$prog"
          F77_LIST="$prog"
        fi
      done
    else
      F77_LIST="$F77"
    fi
    WITH_MPI=yes
    ;;
esac
AC_PROG_CC( [$CC_LIST] )
AC_PROG_CPP
AC_PROG_CXX( [$CXX_LIST] )
AC_PROG_CXXCPP
AC_PROG_FC( [$FC_LIST] )
AC_PROG_F77( [$F77_LIST] )

# Try to determine compiler-specific flags.  This must be done
# before setting up libtool so that it can override libtool settings.
ITAPS_CC_FLAGS
ITAPS_CXX_FLAGS
CFLAGS="$USER_CFLAGS $ITAPS_CC_SPECIAL"
CXXFLAGS="$USER_CXXFLAGS $ITAPS_CXX_SPECIAL"

# On IBM/AIX, the check for OBJEXT fails for the mpcc compiler.
# (Comment out this hack, it should be fixed correctly now)
#if test "x$OBJEXT" = "x"; then
#  OBJEXT=o
#fi

  # Check for debug flags
AC_ARG_ENABLE( debug, AC_HELP_STRING([--enable-debug],[Debug symbols (-g)]),
               [enable_debug=$enableval], [enable_debug=] )  
AC_ARG_ENABLE( optimize, AC_HELP_STRING([--enable-optimize],[Compile optimized (-O2)]),
               [enable_cxx_optimize=$enableval
                enable_cc_optimize=$enableval
		enable_fc_optimize=$enableval
		enable_f77_optimize=$enableval
		], 
               [enable_cxx_optimize=
                enable_cc_optimize=
		enable_fc_optimize=
		enable_f77_optimize=
		] )

# Do enable_optimize by default, unless user has specified
# custom CXXFLAGS or CFLAGS
if test "x$enable_debug" = "x"; then
  if test "x$enable_cxx_optimize" = "x"; then
    if test "x$USER_CXXFLAGS" = "x"; then
      enable_cxx_optimize=yes
    fi
  fi
  if test "x$enable_cc_optimize" = "x"; then
    if test "x$USER_CFLAGS" = "x"; then
      enable_cc_optimize=yes
    fi
  fi
  if test "x$enable_fc_optimize" = "x"; then
    if test "x$USER_FCFLAGS" = "x"; then
      enable_fc_optimize=yes
    fi
  fi
  if test "x$enable_f77_optimize" = "x"; then
    if test "x$USER_FFLAGS" = "x"; then
      enable_f77_optimize=yes
    fi
  fi
fi

# Choose compiler flags from CLI args
if test "xyes" = "x$enable_debug"; then
  CXXFLAGS="$CXXFLAGS -g"
  CFLAGS="$CFLAGS -g"
  FCFLAGS="$FCFLAGS -g"
  FFLAGS="$FFLAGS -g"
fi
if test "xyes" = "x$enable_cxx_optimize"; then
  CXXFLAGS="$CXXFLAGS -O2 -DNDEBUG"
fi
if test "xyes" = "x$enable_cc_optimize"; then
  CFLAGS="$CFLAGS -O2 -DNDEBUG"
fi
if test "xyes" = "x$enable_fc_optimize"; then
  FCFLAGS="$FCFLAGS -O2 -DNDEBUG"
fi
if test "xyes" = "x$enable_f77_optimize"; then
  FFLAGS="$FFLAGS -O2 -DNDEBUG"
fi

  # Check for 32/64 bit.
  # This requires ITAPS_CXX_FLAGS and ITAPS_CC_FLAGS to have been called first
AC_ARG_ENABLE( 32bit, AC_HELP_STRING([--enable-32bit],[Force 32-bit objects]),
[
  if test "xyes" != "x$enableval"; then
    AC_MSG_ERROR([Unknown argument --enable-32bit=$enableval])
  elif test "x" = "x$ITAPS_CXX_32BIT"; then
    AC_MSG_ERROR([Don't know how to force 32-bit C++ on this platform.  Try setting CXXFLAGS manually])
  elif test "x" = "x$ITAPS_CC_32BIT"; then
    AC_MSG_ERROR([Don't know how to force 32-bit C on this platform.  Try setting CFLAGS manually])
  fi
  CXXFLAGS="$CXXFLAGS $ITAPS_CXX_32BIT"
  CFLAGS="$CFLAGS $ITAPS_CC_32BIT"
  enable_32bit=yes
])
# This requires ITAPS_CXX_FLAGS and ITAPS_CC_FLAGS to have been called first
AC_ARG_ENABLE( 64bit, AC_HELP_STRING([--enable-64bit],[Force 64-bit objects]),
[
  if test "xyes" != "x$enableval"; then
    AC_MSG_ERROR([Unknown argument --enable-64bit=$enableval])
  elif test "x" = "x$ITAPS_CXX_64BIT"; then
    AC_MSG_ERROR([Don't know how to force 64-bit C++ on this platform.  Try setting CXXFLAGS manually])
  elif test "x" = "x$ITAPS_CC_64BIT"; then
    AC_MSG_ERROR([Don't know how to force 64-bit C on this platform.  Try setting CFLAGS manually])
  elif test "xyes" = "x$enable_32bit"; then
    AC_MSG_ERROR([Cannot do both --enable-32bit and --enable-64bit])
  fi
  CXXFLAGS="$CXXFLAGS $ITAPS_CXX_64BIT"
  CFLAGS="$CFLAGS $ITAPS_CC_64BIT"
])

]) # ITAPS_CHECK_COMPILERS

#######################################################################################
# *******************************************************************************
# **************************** INTERNAL STUFF ***********************************
# *******************************************************************************
#######################################################################################

#################################################################################
# Check if the compiler defines a specific preprocessor macro
# Arguments:
#  - preprocessor define to check for
#  - action upon success
#  - action upon failure
#################################################################################
AC_DEFUN([ITAPS_TRY_COMPILER_DEFINE], [
AC_COMPILE_IFELSE([
AC_LANG_PROGRAM( [[#ifndef $1
  choke me
#endif]], []) ],
[$2],[$3])
])


#######################################################################################
# Check for compiler-specific flags.
# Sets the following environmental variables:
#   ITAPS_CXX_SPECIAL : Any compiler-specific flags which must be specified
#   ITAPS_CXX_32BIT   : Flag to force compilation of 32-bit code
#   ITAPS_CXX_64BIT   : Flag to force compilation of 64-bit code
#######################################################################################
AC_DEFUN([ITAPS_CXX_FLAGS], [
AC_REQUIRE([AC_PROG_CXX])

# Detect compiler 
AC_MSG_CHECKING([for known c++ compilers])
# Autoconf does G++ for us
if test x$GXX = xyes; then
  cxx_compiler=GNU
# Search for other compiler types
# For efficiency, limit checks to relevant OSs
else
  cxx_compiler=unknown
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  case "$target_os" in
    aix*)
      ITAPS_TRY_COMPILER_DEFINE([__IBMCPP__],[cxx_compiler=VisualAge])
      ;;
    solaris*|sunos*)
      ITAPS_TRY_COMPILER_DEFINE([__SUNPRO_CC],[cxx_compiler=SunWorkshop])
      ;;
    irix*)
      ITAPS_TRY_COMPILER_DEFINE([__sgi],[cxx_compiler=MIPSpro])
      ;;
    linux*)
      ITAPS_TRY_COMPILER_DEFINE([__INTEL_COMPILER],[cxx_compiler=Intel])
      ITAPS_TRY_COMPILER_DEFINE([__IBMCPP__],[cxx_compiler=VisualAge])
      ITAPS_TRY_COMPILER_DEFINE([__DECCXX_VER],[cxx_compiler=Compaq])
      ITAPS_TRY_COMPILER_DEFINE([__SUNPRO_CC],[cxx_compiler=SunWorkshop])
      ITAPS_TRY_COMPILER_DEFINE([__PGI],[cxx_cmopiler=PortlandGroup])
      ;;
    hpux*)
      ITAPS_TRY_COMPILER_DEFINE([__HP_aCC],[cxx_compiler=HP])
      ;;
    windows*)
      ITAPS_TRY_COMPILER_DEFINE([__MSC_VER],[cxx_compiler=VisualStudio])
      ITAPS_TRY_COMPILER_DEFINE([__INTEL_COMPILER],[cxx_compiler=Intel])
      ITAPS_TRY_COMPILER_DEFINE([__DECCXX_VER],[cxx_compiler=Compaq])
      ITAPS_TRY_COMPILER_DEFINE([__BORLANDC__],[cxx_compiler=Borland])
      ITAPS_TRY_COMPILER_DEFINE([__CYGWIN__],[cxx_compiler=Cygwin])
      ITAPS_TRY_COMPILER_DEFINE([__MINGW32__],[cxx_compiler=MinGW])
      ;;
    *)
      ITAPS_TRY_COMPILER_DEFINE([__PGI],[cc_cmopiler=PortlandGroup])
      ;;
  esac
  AC_LANG_RESTORE
fi
AC_MSG_RESULT([$cxx_compiler])
if test "x$cxx_compiler" = "xunknown"; then
  AC_MSG_WARN([Unrecognized C++ compiler: $CXX])
fi

# Now decide special compiler flags using compiler/cpu combination
AC_MSG_CHECKING([for known compiler/OS combinations])
case "$cxx_compiler:$host_cpu" in
  GNU:sparc*)
    ITAPS_CXX_32BIT=-m32
    ITAPS_CXX_64BIT=-m64
    ITAPS_CXX_SPECIAL="-Wall -pipe"
    ;;
  GNU:powerpc*)
    ITAPS_CXX_32BIT=-m32
    ITAPS_CXX_64BIT=-m64
    ITAPS_CXX_SPECIAL="-Wall -pipe"
    ;;
  GNU:i?86|GNU:x86_64)
    ITAPS_CXX_32BIT=-m32
    ITAPS_CXX_64BIT=-m64
    ITAPS_CXX_SPECIAL="-Wall -pipe"
    ;;
  GNU:mips*)
    ITAPS_CXX_32BIT="-mips32 -mabi=32"
    ITAPS_CXX_64BIT="-mips64 -mabi=64"
    ITAPS_CXX_SPECIAL="-Wall -pipe"
    ;;
  VisualAge:*)
    ITAPS_CXX_32BIT=-q32
    ITAPS_CXX_64BIT=-q64
    # Do V5.0 namemangling for compatibility with ACIS, and enable RTTI
    case "$target_vendor" in
      bgp)
        ITAPS_CXX_SPECIAL=""
        AR="ar"
        NM="nm -B"
        ;;
      *)
        ITAPS_CXX_SPECIAL="-qrtti=all -qnamemangling=v5"
        AR="ar"
        NM="nm -B -X 32_64"
        ;;
    esac
    ;;
  MIPSpro:mips)
    ITAPS_CXX_32BIT=-n32
    ITAPS_CXX_64BIT=-64
    ITAPS_CXX_SPECIAL=-LANG:std
    ;;
  MIPSpro:*)
    ITAPS_CXX_SPECIAL=-LANG:std
    ;;
  SunWorkshop:sparc*)
    ITAPS_CXX_32BIT=-xarch=generic
    ITAPS_CXX_64BIT=-xarch=generic64
    ;;
  SunWorkshop:i?86|SunWorkshop:x86_64)
    ITAPS_CXX_32BIT=-m32
    ITAPS_CXX_64BIT=-m64
    ;;
  *)
    ;;
esac
AC_MSG_RESULT([$cxx_compiler:$host_cpu])
]) # end ITAPS_CXX_FLAGS

#######################################################################################
# Check for compiler-specific flags.
# Sets the following environmental variables:
#   ITAPS_CC_SPECIAL : Any compiler-specific flags which must be specified
#   ITAPS_CC_32BIT   : Flag to force compilation of 32-bit code
#   ITAPS_CC_64BIT   : Flag to force compilation of 64-bit code
#######################################################################################
AC_DEFUN([ITAPS_CC_FLAGS], [
AC_REQUIRE([AC_PROG_CC])

# Detect compiler 

AC_MSG_CHECKING([for known C compilers])
# Autoconf does gcc for us
if test x$GCC = xyes; then
  cc_compiler=GNU
# Search for other compiler types
# For efficiency, limit checks to relevant OSs
else
  cc_compiler=unknown
  case "$target_os" in
    aix*)
      ITAPS_TRY_COMPILER_DEFINE([__IBMC__],[cc_compiler=VisualAge])
      ;;
    solaris*|sunos*)
      ITAPS_TRY_COMPILER_DEFINE([__SUNPRO_C],[cc_compiler=SunWorkshop])
      ;;
    irix*)
      ITAPS_TRY_COMPILER_DEFINE([__sgi],[cc_compiler=MIPSpro])
      ;;
    linux*)
      ITAPS_TRY_COMPILER_DEFINE([__INTEL_COMPILER],[cc_compiler=Intel])
      ITAPS_TRY_COMPILER_DEFINE([__IBMC__],[cc_compiler=VisualAge])
      ITAPS_TRY_COMPILER_DEFINE([__DECC_VER],[cc_compiler=Compaq])
      ITAPS_TRY_COMPILER_DEFINE([__SUNPRO_C],[cc_compiler=SunWorkshop])
      ITAPS_TRY_COMPILER_DEFINE([__PGI],[cc_cmopiler=PortlandGroup])
      ;;
    hpux*)
      ITAPS_TRY_COMPILER_DEFINE([__HP_cc],[cc_compiler=HP])
      ;;
    windows*)
      ITAPS_TRY_COMPILER_DEFINE([__MSC_VER],[cc_compiler=VisualStudio])
      ITAPS_TRY_COMPILER_DEFINE([__INTEL_COMPILER],[cc_compiler=Intel])
      ITAPS_TRY_COMPILER_DEFINE([__DECC_VER],[cc_compiler=Compaq])
      ITAPS_TRY_COMPILER_DEFINE([__BORLANDC__],[cc_compiler=Borland])
      ITAPS_TRY_COMPILER_DEFINE([__TURBOC__],[cc_compiler=TurboC])
      ITAPS_TRY_COMPILER_DEFINE([__CYGWIN__],[cc_compiler=Cygwin])
      ITAPS_TRY_COMPILER_DEFINE([__MINGW32__],[cc_compiler=MinGW])
      ;;
    *)
      ITAPS_TRY_COMPILER_DEFINE([__PGI],[cc_cmopiler=PortlandGroup])
      ;;
  esac
fi
AC_MSG_RESULT([$cc_compiler])
if test "x$cc_compiler" = "xunknown"; then
  AC_MSG_WARN([Unrecognized C compiler: $CXX])
fi

# Now decide special compiler flags using compiler/cpu combination
AC_MSG_CHECKING([for known compiler/OS combinations])
case "$cc_compiler:$host_cpu" in
  GNU:sparc*)
    ITAPS_CC_32BIT=-m32
    ITAPS_CC_64BIT=-m64
    ITAPS_CC_SPECIAL="-Wall -pipe"
    ;;
  GNU:powerpc*)
    ITAPS_CC_32BIT=-m32
    ITAPS_CC_64BIT=-m64
    ITAPS_CC_SPECIAL="-Wall -pipe"
    ;;
  GNU:i?86|GNU:x86_64)
    ITAPS_CC_32BIT=-m32
    ITAPS_CC_64BIT=-m64
    ITAPS_CC_SPECIAL="-Wall -pipe"
    ;;
  GNU:mips*)
    ITAPS_CC_32BIT="-mips32 -mabi=32"
    ITAPS_CC_64BIT="-mips64 -mabi=64"
    ITAPS_CC_SPECIAL="-Wall -pipe"
    ;;
  VisualAge:*)
    case "$target_vendor" in
      bgp)
        ITAPS_CC_32BIT=-q32
        ITAPS_CC_64BIT=-q64
        AR="ar"
        NM="nm -B"
        ;;
      *)
        ITAPS_CC_32BIT=-q32
        ITAPS_CC_64BIT=-q64
        AR="ar -X 32_64"
        NM="nm -B -X 32_64"
        ;;
    esac
    ;;
  MIPSpro:mips)
    ITAPS_CC_32BIT=-n32
    ITAPS_CC_64BIT=-64
    ITAPS_CC_SPECIAL=-LANG:std
    ;;
  MIPSpro:*)
    ITAPS_CC_SPECIAL=-LANG:std
    ;;
  SunWorkshop:sparc*)
    ITAPS_CC_32BIT=-xarch=generic
    ITAPS_CC_64BIT=-xarch=generic64
    ;;
  SunWorkshop:i?86|SunWorkshop:x86_64)
    ITAPS_CC_32BIT=-m32
    ITAPS_CC_64BIT=-m64
    ;;
  *)
    ;;
esac
AC_MSG_RESULT([$cc_compiler:$host_cpu])
]) # end ITAPS_CC_FLAGS
