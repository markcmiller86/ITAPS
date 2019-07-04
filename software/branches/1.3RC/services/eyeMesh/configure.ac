#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
# Modifications
#   Mark C. Miller, Tue Jul 27 21:25:14 PDT 2010
#   Added AC_DEFINE for HAVE_VTK
#
#   Mark C. Miller, Tue Sep  7 10:16:46 PDT 2010
#   Removed inadvertent override of QTDIR=xxx. Probably due to
#   some debugging garbage left in.


AC_PREREQ([2.59])
AC_INIT([eyeMesh], [0.2], [itaps-dev@lists.mcs.anl.gov])
AC_CONFIG_SRCDIR([Explorer.cpp])
AC_CONFIG_HEADERS([config.h])

#
# Setup IMESH_DEFS variable
#
AC_ARG_VAR(IMESH_DEFS, [Full path-quailified name of iMesh-Defs.inc] )
if test -z "$IMESH_DEFS"; then
    AC_MSG_ERROR(IMESH_DEFS variable not specified)
else
    if ! test -r $IMESH_DEFS; then
        AC_MSG_ERROR([The file specified for IMESH_DEFS, "$IMESH_DEFS", either does not exist or is not readable])
    fi
fi

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([limits.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([isascii])

#
# The following Qt configure logic was obtained from...
# http://qtnode.net/wiki?title=Qt_with_autotools
#
AC_MSG_CHECKING(QT4.1 directory)
AC_ARG_WITH([qt-dir],
             AC_HELP_STRING([--with-qt-dir=/path/to/Qt-4.1.0],
                            [to specify the path to the Qt-4.1.0 directory.]),
             [QTPATHS="$withval"],
             [QTPATHS="/usr/local/Trolltech/Qt-4.1.0 /c/Qt/4.1.0"])

for x in $QTPATHS; do
    if test -d $x ; then
       QTDIR="$x"
    fi
done
if test $QTDIR = xxx ; then
   AC_MSG_ERROR(Could not locate QT 4.1)
fi
AC_MSG_RESULT($QTDIR)
host=`uname -a` # AC_CANONICAL_HOST is broken at the time of this writing.
case $host in
  MINGW32*)
    AC_MSG_NOTICE(MinGW detected.)
    QTLIBS="-L$QTDIR/lib -lopengl32 -lglu32 -lgdi32 -luser32 -lmingw32 -lqtmain -lQtOpenGL4 -lQtGui4 -lQtCore4 -mthreads -Wl,-enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -Wl,-s -Wl,-s -Wl,-subsystem,windows"
    if test -d $QTDIR/include/qt4; then
        QTINC="-I$QTDIR/include/qt4 -I$QTDIR/include/qt4/QtCore -I$QTDIR/include/qt4/QtGui -I$QTDIR/include/qt4/QtOpenGL -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -DQT_NEEDS_QMAIN -frtti -fexceptions"
    else
        QTINC="-I$QTDIR/include -I$QTDIR/include/QtCore -I$QTDIR/include/QtGui -I$QTDIR/include/QtOpenGL -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -DQT_NEEDS_QMAIN -frtti -fexceptions"
    fi
    ;;
  *)
    AC_MSG_NOTICE(I'm assuming this is Linux)
    AC_PATH_XTRA
    QTLIBS="-Wl,-rpath,$QTDIR/lib -L$QTDIR/lib -lQtGui -lQtOpenGL -lQtCore $X_LIBS -lX11 -lXext -lXmu -lXt $X_EXTRA_LIBS -lGLU -lGL -lpthread"
    if test -d $QTDIR/include/qt4; then
        QTINC="-I$QTDIR/include/qt4 -I$QTDIR/include/qt4/QtGui -I$QTDIR/include/qt4/QtCore -I$QTDIR/include/qt4/QtOpenGL $X_CFLAGS -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED"
    else
        QTINC="-I$QTDIR/include -I$QTDIR/include/QtGui -I$QTDIR/include/QtCore -I$QTDIR/include/QtOpenGL $X_CFLAGS -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED"
    fi
    ;;
esac
QTBIN="$QTDIR/bin"

old_LIBS="$LIBS"
old_INCLUDES="$INCLUDES"
old_PATH="$PATH"
old_CXXFLAGS="$CXXFLAGS"

LIBS="$LIBS $QTLIBS"
INCLUDES="$INCLUDES $QTINC"
PATH="$PATH:$QTBIN"
CXXFLAGS="$CXXFLAGS $QTINC"

# Now we check whether we can actually build a Qt app.
cat > myqt.h << EOF
#include <QObject>
class Test : public QObject
{
Q_OBJECT
public:
  Test() {}
  ~Test() {}
public slots:
  void receive() {}
signals:
  void send();
};
EOF

cat > myqt.cpp << EOF
#include "myqt.h"
#include <QApplication>
int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  Test t;
  QObject::connect( &t, SIGNAL(send()), &t, SLOT(receive()) );
}
EOF

AC_MSG_CHECKING(does moc work?)
bnv_try_1="$QTBIN/moc myqt.h -o moc_myqt.cpp"
AC_TRY_EVAL(bnv_try_1)
if test x"$ac_status" != x0; then
   AC_MSG_ERROR(moc doesn't work)
fi
AC_MSG_RESULT(yes)
AC_MSG_CHECKING(can I compile moc_myqt.cpp?)
bnv_try_2="$CXX -c $CXXFLAGS -o moc_myqt.o moc_myqt.cpp"
AC_TRY_EVAL(bnv_try_2)
if test x"$ac_status" != x0; then
   AC_MSG_ERROR(couldn't compile moc_myqt.cpp)
fi
AC_MSG_RESULT(yes)
AC_MSG_CHECKING(can I compile myqt.cpp?)
bnv_try_3="$CXX $QTINC -c $CXXFLAGS -o myqt.o myqt.cpp"
AC_TRY_EVAL(bnv_try_3)
if test x"$ac_status" != x0; then
   AC_MSG_ERROR(couldn't compile myqt.cpp)
fi
AC_MSG_RESULT(yes)
AC_MSG_CHECKING(can I link against QT?)
nv_try_4="$CXX $LIBS -o myqt myqt.o moc_myqt.o"
AC_TRY_EVAL(bnv_try_4)
if test x"$ac_status" != x0; then
   AC_MSG_ERROR(couldn't link)
fi
AC_MSG_RESULT(yes)

AC_MSG_CHECKING(for mkoctfile)
AC_TRY_EVAL(mkoctfile)
if test x"$ac_status" != x0; then
   AC_MSG_ERROR(mkoctfile is not in the path)
fi
AC_MSG_RESULT(yes)
rm -f moc_myqt.cpp myqt.h myqt.cpp myqt.o myqt moc_myqt.o

LIBS="$old_LIBS"
INCLUDES="$old_INCLUDES"
PATH="$old_PATH"
CXXFLAGS="$old_CXXFLAGS"

#
# End of Qt configure logic obtained from TrollTech
#
AC_SUBST(QTLIBS)
AC_SUBST(QTINC)
AC_SUBST(QTBIN)

AM_OPTIONS_VTK

AM_PATH_VTK(4.2.0,echo "vtk passed")
AC_SUBST(VTK_LIBS)
AC_SUBST(VTK_CFLAGS)
AC_SUBST(VTK_CXXFLAGS)
AC_SUBST(VTK_LDFLAGS)
if test -n "$VTK_LDFLAGS"; then
    AC_DEFINE(HAVE_VTK,1,[Support for VTK])
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
