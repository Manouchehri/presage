## Makefile for the test/core/dbconnector directory of Soothsayer.
## Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

## Process this file with automake to produce Makefile.in

if HAVE_CPPUNIT

TESTS =	dbconnectorTestRunner

check_PROGRAMS = $(TESTS)

dbconnectorTestRunner_SOURCES =	dbconnectorTestRunner.cpp       \
				databaseConnectorTest.cpp       \
				databaseConnectorTest.h         \
				sqliteDatabaseConnectorTest.cpp \
				sqliteDatabaseConnectorTest.h   
dbconnectorTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
dbconnectorTestRunner_LDFLAGS =		$(CPPUNIT_LIBS)
dbconnectorTestRunner_LIBS =		-ldl
dbconnectorTestRunner_LDADD =		$(top_builddir)/src/core/dbconnector/libdbconnector.la
dbconnectorTestRunner_CPPFLAGS =	-I$(top_srcdir)/src

# Clean out files created during tests.
# Required to make distcheck happy.
DISTCLEANFILES =	test.db

endif
