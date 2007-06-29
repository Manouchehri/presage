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
	[0.1.1],
	[matteo.vescovi@yahoo.co.uk])
AM_INIT_AUTOMAKE([-Wall -Werror])

AC_CONFIG_SRCDIR([src/soothsayer.cpp])
AC_CONFIG_HEADERS([config.h:config.hin])

dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_CC
AC_PROG_CPP
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
AC_CHECK_LIB([ncurses],
             [initscr],
             [],
	     [AC_MSG_ERROR([ncurses library not found. Please install ncurses package.])])

AC_CHECK_HEADERS([curses.h],
                 [],
                 [AC_MSG_ERROR([ncurses header file not found. Please install ncurses development package.])],
                 [])

dnl =================
dnl Checks for SQLite
dnl =================
dnl 
dnl Sqlite 3
AC_CHECK_LIB([sqlite3],
             [sqlite3_open],
             [],
	     [AC_CHECK_LIB([sqlite],
                           [sqlite_open],
                           [],
	                   [AC_MSG_ERROR([SQLite library not found. SQLite can be obtained from http://sqlite.org.])])])


AC_CHECK_HEADERS([sqlite3.h],
                 [],
                 [AC_CHECK_HEADERS([sqlite.h],
                                   [],
                                   [AC_MSG_ERROR([SQLite header file not found. SQLite can be obtained from http://sqlite.org.])],
                                   [])],

                 [])


dnl ==================
dnl Checks for CppUnit
dnl ==================
AM_PATH_CPPUNIT([1.9.6],
                [],
                [AC_MSG_WARN([CppUnit not found. Unit tests will not be built. CppUnit can be obtained from http://cppunit.sourceforge.net.])])
AM_CONDITIONAL(HAVE_CPPUNIT, test "$CPPUNIT_LIBS")


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
	resources/profiles/Makefile])
