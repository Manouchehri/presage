dnl configure.ac for Soothsayer
dnl Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>
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
	[0.1],
	[matteo.vescovi@tiscali.it])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/soothsayer.cpp])
AC_CONFIG_HEADERS([config.h:config.hin])

dnl Checks for CppUnit
AM_PATH_CPPUNIT(1.9.6)


dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP


dnl Checks for libtool
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

dnl Check for SQLite library
AC_CHECK_LIB([sqlite],
             [sqlite_open],
             [],
	     [AC_MSG_ERROR([SQLite library not found. SQLite can be obtained from http://sqlite.org.])])
     
dnl Check for CPPUnit library
AC_CHECK_LIB([cppunit],
             [TestRunner],
             [],
	     [AC_MSG_WARN([CPPUnit library not found. CPPUnit can be obtained from http://cppunit.sourceforge.net.])])

dnl =======================
dnl Checks for header files
dnl =======================
AC_STDC_HEADERS

dnl Check for SQLite header
AC_CHECK_HEADERS([sqlite.h],
                 [],
                 [AC_MSG_ERROR([SQLite header file not found. SQLite can be obtained from http://sqlite.org.])],
                 [])

AC_CHECK_HEADERS([cppunit/TestRunner.h],
                 [],
                 [AC_MSG_WARN([CPPUnit header file not found. CPPUnit can be obtained from http://cppunit.sourceforge.net.])],
                 [])

AC_CHECK_HEADERS([dirent.h],
                 [],
                 [],
                 [])

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
