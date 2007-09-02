dnl configure.ac for Soothsayer
dnl Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2, or (at your option)
dnl any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

dnl  Process this file with autoconf to produce a configure script.


AC_INIT([soothsayer],
	[0.5],
	[matteo.vescovi@yahoo.co.uk])
AM_INIT_AUTOMAKE([-Wall -Werror])

AC_CONFIG_SRCDIR([src/soothsayer.cpp])
AC_CONFIG_HEADERS([config.h:config.hin])

dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_CXX
AC_PROG_CXXCPP

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
AC_PROG_SWIG(1.3.17)
AM_CONDITIONAL(HAVE_SWIG, test "$SWIG")


dnl =================
dnl Checks for Python
dnl =================
AM_PATH_PYTHON([1.5],
               [],
               [AC_MSG_WARN([Python not found. Python is required to build soothsayer python binding. Python can be obtained from http://www.python.org])])
AM_CONDITIONAL(HAVE_PYTHON, test "$PYTHON")


AC_ARG_ENABLE([python-binding],
              AS_HELP_STRING([--enable-python-binding],
                             [build python binding (default=no]),
              [ac_enable_python_binding=yes],
              [ac_enable_python_binding=no])
if test "x$ac_enable_python_binding" = "xyes"
then
    SWIG_ENABLE_CXX
    # SWIG_MULTI_MODULE_SUPPORT
    SWIG_PYTHON
else
    AC_MSG_NOTICE([Python binding for soothsayer will not be built])
    AC_MSG_NOTICE([You can enable python binding module with --enable-python-binding])
fi
AM_CONDITIONAL(ENABLE_PYTHON_BINDING, test "x$ac_enable_python_binding" = "xyes")


############################
# Temporarily disabling this
#AC_CONFIG_SUBDIRS(src/plump)


AC_OUTPUT([
	Makefile
	src/Makefile
	src/core/Makefile
	src/core/tokenizer/Makefile
	src/plugins/Makefile
	src/tools/Makefile
	src/simulator/Makefile
	src/tinyxml/Makefile
	test/Makefile
	test/core/Makefile
	test/core/tokenizer/Makefile
	test/plugins/Makefile
	test/tools/Makefile
	test/simulator/Makefile
	resources/Makefile
	resources/profiles/Makefile
	bindings/Makefile
	bindings/python/setup.py
	bindings/python/Makefile])
