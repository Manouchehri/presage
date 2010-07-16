
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

SUBDIRS = dbconnector

check_PROGRAMS = $(TESTS)

if HAVE_CPPUNIT

TESTS =	predictorsTestRunner newPredictorsTestRunner

predictorsTestRunner_SOURCES = 	predictorsTestRunner.cpp \
				predictorsTestMockObjects.h \
				predictorsTestMockObjects.cpp \
				predictorsTestFixture.h \
				predictorsTestFixture.cpp \
				abbreviationExpansionPredictorTest.h \
				abbreviationExpansionPredictorTest.cpp
predictorsTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
predictorsTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
predictorsTestRunner_LDADD =	libpredictorstest.la

if USE_SQLITE
predictorsTestRunner_SOURCES +=	smoothedNgramPredictorTest.h \
				smoothedNgramPredictorTest.cpp
endif


newPredictorsTestRunner_SOURCES =	predictorsTestRunner.cpp \
					recencyPredictorTest.h \
					recencyPredictorTest.cpp \
					dejavuPredictorTest.h \
					dejavuPredictorTest.cpp 
newPredictorsTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
newPredictorsTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
newPredictorsTestRunner_LDADD =		libpredictorstest.la

check_LTLIBRARIES =			libpredictorstest.la

libpredictorstest_la_SOURCES = 
libpredictorstest_la_LIBADD =		$(top_builddir)/src/lib/predictors/libpredictors.la \
					$(top_builddir)/src/lib/core/libcore.la

if USE_SQLITE
newPredictorsTestRunner_SOURCES +=	newSmoothedNgramPredictorTest.h \
					newSmoothedNgramPredictorTest.cpp
libpredictorstest_la_LIBADD +=		$(top_builddir)/src/lib/predictors/dbconnector/libdbconnector.la 
endif

endif # HAVE_CPPUNIT

AM_CPPFLAGS =	-I$(top_srcdir)/src/lib
