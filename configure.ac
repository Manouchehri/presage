dnl configure.ac for Presage

dnl Presage, an extensible predictive text entry system
dnl ---------------------------------------------------
dnl
dnl Copyright (C) 2008  Matteo Vescovi <matteo.vescovi\@yahoo.co.uk>
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License along
dnl with this program; if not, write to the Free Software Foundation, Inc.,
dnl 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

dnl Process this file with autoconf to produce a configure script.


AC_INIT([presage],
	[0.8b],
	[matteo.vescovi@yahoo.co.uk])
AM_INIT_AUTOMAKE([-Wall -Werror])

AC_CONFIG_SRCDIR([src/lib/presage.cpp])
AC_CONFIG_HEADERS([config.h:config.hin])

dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_CC
AC_PROG_CPP
AM_PROG_CC_C_O

dnl ==================
dnl Checks for libtool
dnl ==================
############################
# Temporarily disabling this
#AC_LIB_LTDL
#AC_LIBLTDL_CONVENIENCE
AC_SUBST(LTDLINCL)
AC_SUBST(LIBLTDL)
AC_LIBTOOL_WIN32_DLL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL


dnl Cygwin windows specific flag
LDFLAGS="-L/usr/local/lib $LDFLAGS"

dnl ====================
dnl Checks for libraries
dnl ====================

dnl =======================
dnl Checks for header files
dnl =======================
AC_STDC_HEADERS
AC_HEADER_DIRENT
AC_CHECK_HEADERS([pwd.h])

dnl ==================
dnl Checks for ncurses
dnl ==================
AC_CHECK_LIB([curses],
             [initscr],
             [have_curses_library=true],
	     [AC_MSG_WARN([curses library not found. curses demo programs will not be built.])])
AC_CHECK_HEADERS([curses.h],
                 [have_curses_header=true],
                 [AC_MSG_WARN([curses header file not found. curses demo programs will not be built.])],
                 [])
AM_CONDITIONAL(HAVE_CURSES, test "x$have_curses_library" = "xtrue" -a "x$have_curses_header" = "xtrue")

dnl =================
dnl Checks for SQLite
dnl =================
dnl 
dnl check for libsqlite3
dnl if (libsqlite3 found) {
dnl     set SQLITE_LIBS to -lsqlite3
dnl     check for sqlite3.h
dnl     if (sqlite3.h found) {
dnl         using sqlite3
dnl     } else {
dnl         error
dnl     }
dnl } else {
dnl     check for libsqlite2
dnl     if (libsqlite2 found) {
dnl         set SQLITE_LIBS to -lsqlite
dnl         check for sqlite2.h
dnl         if (sqlite2.h found) {
dnl             using sqlite2
dnl         } else
dnl             error
dnl         }
dnl     } else {
dnl         error
dnl     }
dnl }
AC_CHECK_LIB([sqlite3],
             [sqlite3_open],
             [SQLITE_LIBS="-lsqlite3" &&
              AC_CHECK_HEADERS([sqlite3.h],
                               [AC_MSG_NOTICE([using sqlite 3])],
                               [AC_MSG_ERROR([Found sqlite3 library, but cannot find matching sqlite3.h header.])],
                               [])],
	     [AC_CHECK_LIB([sqlite],
                           [sqlite_open],
                           [SQLITE_LIBS="-lsqlite" && \
                            AC_CHECK_HEADERS([sqlite.h],
                                             [AC_MSG_NOTICE([using sqlite 2])],
                                             [AC_MSG_ERROR([Found sqlite 2 library, but cannot find matching sqlite.h header.])],
                                             [])],
                           [AC_MSG_ERROR([SQLite library not found. SQLite can be obtained from http://sqlite.org.])])])
AC_SUBST([SQLITE_LIBS], ["$SQLITE_LIBS"])


dnl ==================
dnl Checks for CppUnit
dnl ==================
AM_PATH_CPPUNIT([1.9.6],
                [],
                [AC_MSG_WARN([CppUnit not found. Unit tests will not be built. CppUnit can be obtained from http://cppunit.sourceforge.net.])])
AM_CONDITIONAL(HAVE_CPPUNIT, test "$CPPUNIT_LIBS")


dnl ============================
dnl Bindings for other languages
dnl ==

dnl ===============
dnl Checks for SWIG
dnl ===============
AC_PATH_PROG([SWIG], [swig])
AM_CONDITIONAL(HAVE_SWIG, test "$SWIG")


dnl =================
dnl Checks for Python
dnl =================
AM_PATH_PYTHON([2.0],
               [],
               [AC_MSG_WARN([Python not found. Python is required to build presage python binding. Python can be obtained from http://www.python.org])])
if test "$PYTHON"
then
    python_include_path=`$PYTHON -c "import distutils.sysconfig; print distutils.sysconfig.get_python_inc();"`
    AC_CHECK_HEADERS([${python_include_path}/Python.h],
                     [have_python_header=true],
                     [AC_MSG_WARN([Python.h header file not found. Python development files are required to build presage python binding. Python can be obtained from http://www.python.org])],
                     [])
fi
AM_CONDITIONAL(HAVE_PYTHON, test "$PYTHON" -a "x$have_python_header" = "xtrue")


AC_ARG_ENABLE([python-binding],
              AS_HELP_STRING([--disable-python-binding],
                             [build python binding (default=yes)]),
              [ac_enable_python_binding=$enableval],
              [ac_enable_python_binding=yes])
if test "x$ac_enable_python_binding" = "xyes"
then
    if test ! "$SWIG" -o ! "$PYTHON" -o ! "x$have_python_header" = "xtrue"
    then
        AC_MSG_WARN([Python binding for presage cannot be built. Ensure that SWIG and Python packages are available.])
    fi
else
    AC_MSG_NOTICE([Python binding for presage will not be built.])
    AC_MSG_NOTICE([Enable Python binding module building with --enable-python-binding])
fi
AM_CONDITIONAL(ENABLE_PYTHON_BINDING, test "x$ac_enable_python_binding" = "xyes")


dnl ========================
dnl Checks for documentation
dnl ========================
AC_ARG_ENABLE([documentation],
        AS_HELP_STRING([--disable-documentation],[disable documentation generation  (default: enabled)]),
        [enable_documentation=$enableval],
        [enable_documentation=yes])
AM_CONDITIONAL([BUILD_DOCUMENTATION], [test "x$enable_documentation" = "xyes"])


dnl ==================
dnl Checks for Doxygen
dnl ==================
AC_CHECK_PROG([HAVE_DOXYGEN], [doxygen], [yes], [no])
if test "x$HAVE_DOXYGEN" = "xno"
then
    AC_MSG_NOTICE([Doxygen not found. API documentation will not be built.])
fi
AM_CONDITIONAL([HAVE_DOXYGEN], [test "x$HAVE_DOXYGEN" = "xyes"])
if test "x$enable_documentation" = "xyes" -a "x$HAVE_DOXYGEN" = "xyes"
then
    build_documentation=yes
else
    build_documentation=no
fi

dnl ==============
dnl Checks for dot
dnl ==============
AC_CHECK_PROG([HAVE_DOT], [dot], [yes], [no])
if test "x$HAVE_DOT" = "xno"
then
    AC_MSG_NOTICE([dot not found. Documentation graphs and diagrams will not be generated.])
fi
AC_SUBST([HAVE_DOT], ["$HAVE_DOT"])
if test "x$enable_documentation" = "xyes" -a "x$HAVE_DOXYGEN" = "xyes" -a "x$HAVE_DOT" = "xyes"
then
    build_documentation_diagrams=yes
else
    build_documentation_diagrams=no
fi


dnl ===================
dnl Checks for help2man
dnl ===================
AC_CHECK_PROG([HAVE_HELP2MAN], [help2man], [yes], [no])
if test "x$HAVE_HELP2MAN" = "xno"
then
    AC_MSG_NOTICE([help2man not found. Man documentation pages will not be built.])
fi
AM_CONDITIONAL([HAVE_HELP2MAN], [test "x$HAVE_HELP2MAN" = "xyes"])


dnl ====================
dnl Checks for GTK tools
dnl ====================
AC_ARG_ENABLE([gpresagemate],
        AS_HELP_STRING([--disable-gpresagemate],[build gpresagemate (default: enabled)]),
        [enable_gpresagemate=$enableval],
        [enable_gpresagemate=yes])

AC_ARG_ENABLE([gprompter],
        AS_HELP_STRING([--disable-gprompter],[build gprompter (default: enabled)]),
        [enable_gprompter=$enableval],
        [enable_gprompter=yes])

if test "x$enable_gpresagemate" = "xyes"
then
	AC_CHECK_HEADERS([X11/Xlib.h X11/Xproto.h X11/X.h X11/Xutil.h X11/keysymdef.h pthread.h],
	                 [have_x11_headers=yes],
	                 [have_x11_headers=no],
	                 [])
	AC_CHECK_HEADERS([X11/extensions/Xevie.h],
                         [have_x11_extension_xevie=yes],
                         [have_x11_extension_xevie=no],
	                 [[#if HAVE_X11_XLIB_H
	                   # include <X11/Xlib.h>
	                   #endif]])
fi

if test "x$enable_gpresagemate" = "xyes" -o "x$enable_gprompter" = "xyes"
then
	PKG_CHECK_MODULES([GNOME],
	                  [gtk+-2.0],
	                  [have_gtk=yes],
	                  [have_gtk=no])
	AC_SUBST([GNOME_CFLAGS])
	AC_SUBST([GNOME_LIBS])
	PKG_CHECK_MODULES([GTHREAD],
			  [gthread-2.0],
			  [have_gthread=yes],
			  [have_gthread=no])
	AC_SUBST([GTHREAD_CFLAGS])
	AC_SUBST([GTHREAD_LIBS])
fi

if test "x$enable_gpresagemate" = "xyes" -a \
   	"x$have_x11_headers" = "xyes" -a \
   	"x$have_x11_extension_xevie" = "xyes" -a \
	"x$have_gtk" = "xyes" -a \
	"x$have_gthread" = "xyes"
then
	build_gtk_presage_mate=yes
else
	build_gtk_presage_mate=no
fi
AM_CONDITIONAL([BUILD_GTK_PRESAGE_MATE], [test "x$build_gtk_presage_mate" = "xyes"])

if test "x$enable_gprompter" = "xyes" -a \
   	"x$have_gtk" = "xyes" -a \
	"x$have_gthread" = "xyes"
then
	build_gtk_prompter=yes
else
	build_gtk_prompter=no
fi
AM_CONDITIONAL(BUILD_GTK_PROMPTER, test "x$build_gtk_prompter" = "xyes")

dnl ===================================================
dnl Checks for CMU-Statistical Language Modelling tools
dnl ===================================================
AC_PATH_PROG([TEXT2WFREQ], [text2wfreq])
AC_PATH_PROG([TEXT2IDNGRAM], [text2idngram])
AC_PATH_PROG([WFREQ2VOCAB], [wfreq2vocab])
AC_PATH_PROG([IDNGRAM2LM], [idngram2lm])
if test "$TEXT2WFREQ" -a \
        "$WFREQ2VOCAB" -a \
        "$IDNGRAM2LM" -a \
	"$TEXT2IDNGRAM" 
then
    build_arpa_ngram_resources=yes
else
    build_arpa_ngram_resources=no
    AC_MSG_NOTICE([CMU-Cambridge SLM tools not found. ARPA ngram language model will not be built.])
fi
AM_CONDITIONAL([BUILD_ARPA_NGRAM], [test "x$build_arpa_ngram_resources" = "xyes"])


############################
# Temporarily disabling this
#AC_CONFIG_SUBDIRS(src/plump)


AC_OUTPUT([
	Makefile
	src/Makefile
	src/lib/Makefile
	src/lib/core/Makefile
	src/lib/core/tokenizer/Makefile
	src/lib/core/context_tracker/Makefile
	src/lib/core/dbconnector/Makefile
	src/lib/simulator/Makefile
	src/lib/tinyxml/Makefile
	src/lib/plugins/Makefile
	src/tools/Makefile
	test/Makefile
	test/lib/Makefile
	test/lib/common/Makefile
	test/lib/core/Makefile
	test/lib/core/tokenizer/Makefile
	test/lib/core/context_tracker/Makefile
	test/lib/core/dbconnector/Makefile
	test/lib/plugins/Makefile
	test/lib/simulator/Makefile
	test/tools/Makefile
	test/integration/Makefile
	resources/Makefile
	resources/arpa/Makefile
	resources/profiles/Makefile
	bindings/Makefile
	bindings/python/setup.py
	bindings/python/Makefile
	apps/Makefile
	apps/gtk/Makefile
	apps/gtk/gpresagemate/Makefile
	apps/gtk/gprompter/Makefile
	apps/python/Makefile
	apps/python/pyprompter
	apps/python/setup.py
	doc/Doxyfile
	doc/Makefile
	setup.py
	bindings/python/presage_python_demo
])

if test "x$have_curses_library" = "xtrue" -a "x$have_curses_header" = "xtrue"
then
    build_demo_application="yes"
else
    build_demo_application="no"
fi
if test "$CPPUNIT_LIBS"
then
    build_unit_tests="yes"
else
    build_unit_tests="no"
fi
if test "$SWIG" -a "$PYTHON" -a "x$have_python_header" = "xtrue" -a "x$ac_enable_python_binding" = "xyes"
then
    build_python_binding="yes"
else
    build_python_binding="no"
fi
AC_MSG_RESULT([
  $PACKAGE $VERSION configured successfully.

  General configuration:

    Compiler: ................... ${CXX}
    Curses demo application: .... ${build_demo_application}
    Unit tests: ................. ${build_unit_tests}
    Python binding: ............. ${build_python_binding}
    Man pages: .................. ${HAVE_HELP2MAN}
    API documentation: .......... ${build_documentation}
    API documentation diagrams: . ${build_documentation_diagrams}


  Predictive plugins:

    Smoothed n-gram: ............ yes
    Recency promotion: .......... yes
    Abbreviation expansion: ..... yes
    Smoothed count: ............. yes
    Dictionary: ................. yes
    Deja-vu: .................... yes
    ARPA: ....................... yes


  Additional applications:

    Presage prompter: ........... ${build_python_binding}
    GTK+ presage mate: .......... ${build_gtk_presage_mate}
    GTK+ prompter: .............. ${build_gtk_prompter}


  Additional resources:
    ARPA format ngram: .......... ${build_arpa_ngram_resources}


  Building:

    Type `make' to build $PACKAGE.
    Type `make check' to test $PACKAGE.
    Type `make install' to install $PACKAGE.
])
