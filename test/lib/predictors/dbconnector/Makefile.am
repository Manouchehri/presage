
##########
#  Presage, an extensible predictive text entry system
#  ------------------------------------------------------
#
#  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with this program; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

## Process this file with automake to produce Makefile.in

if HAVE_CPPUNIT
if USE_SQLITE

TESTS =	dbconnectorTestRunner

check_PROGRAMS = $(TESTS)

dbconnectorTestRunner_SOURCES =	dbconnectorTestRunner.cpp       \
				databaseConnectorTest.cpp       \
				databaseConnectorTest.h         \
				sqliteDatabaseConnectorTest.cpp \
				sqliteDatabaseConnectorTest.h

# presageException files are included in sources since sqlite
# database connector defines an exception that inherits from
# presage base exception

dbconnectorTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
dbconnectorTestRunner_LDFLAGS =		$(CPPUNIT_LIBS)
dbconnectorTestRunner_LDADD =		$(top_builddir)/src/lib/predictors/dbconnector/libdbconnector.la \
					$(top_builddir)/src/lib/core/libcore.la \
					$(top_builddir)/src/lib/libpresage.la
dbconnectorTestRunner_CPPFLAGS =	-I$(top_srcdir)/src/lib

# Clean out files created during tests.
# Required to make distcheck happy.
DISTCLEANFILES =	test.db

endif
endif
