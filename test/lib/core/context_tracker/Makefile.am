
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

AUTOMAKE_OPTIONS = subdir-objects

if HAVE_CPPUNIT

TESTS =		contextTrackerTestRunner

check_PROGRAMS = $(TESTS)

contextTrackerTestRunner_SOURCES =	contextTrackerTestRunner.cpp \
					contextChangeDetectorTest.h \
					contextChangeDetectorTest.cpp \
					contextTrackerTest.h \
					contextTrackerTest.cpp 
contextTrackerTestRunner_CPPFLAGS =	-I$(top_srcdir)/src/lib
contextTrackerTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
contextTrackerTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
contextTrackerTestRunner_LDADD =	libcontexttrackertest.la

check_LTLIBRARIES = 	\
	libcontexttrackertest.la

libcontexttrackertest_la_SOURCES =	
libcontexttrackertest_la_LIBADD = \
	../../common/libcommon.la \
	$(top_builddir)/src/lib/core/libcore.la \
	$(top_builddir)/src/lib/predictors/libpredictors.la


if BUILD_TINYXML
contextTrackerTestRunner_CPPFLAGS +=	-I$(top_srcdir)/src/lib/tinyxml
libcontexttrackertest_la_LIBADD +=	$(top_builddir)/src/lib/tinyxml/libtinyxml.la
else
libcontexttrackertest_la_LIBADD +=	-ltinyxml
endif

endif
