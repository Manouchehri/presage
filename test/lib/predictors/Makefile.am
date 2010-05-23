
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

predictorsTestRunner_SOURCES =	predictorsTestRunner.cpp \
				predictorsTestMockObjects.h \
				predictorsTestMockObjects.cpp \
				predictorsTestFixture.h \
				predictorsTestFixture.cpp \
				$(top_srcdir)/src/lib/core/suggestion.h \
				$(top_srcdir)/src/lib/core/suggestion.cpp \
				$(top_srcdir)/src/lib/core/prediction.h \
				$(top_srcdir)/src/lib/core/prediction.cpp \
				$(top_srcdir)/src/lib/core/utility.h \
				$(top_srcdir)/src/lib/core/utility.cpp \
				$(top_srcdir)/src/lib/core/variable.h \
				$(top_srcdir)/src/lib/core/variable.cpp \
				$(top_srcdir)/src/lib/core/configuration.h \
				$(top_srcdir)/src/lib/core/configuration.cpp \
				$(top_srcdir)/src/lib/presageException.h \
				$(top_srcdir)/src/lib/core/presageException.cpp	\
				$(top_srcdir)/src/lib/core/observable.h \
				$(top_srcdir)/src/lib/core/observable.cpp \
				$(top_srcdir)/src/lib/core/observer.h \
				$(top_srcdir)/src/lib/core/observer.cpp \
				abbreviationExpansionPredictorTest.h \
				abbreviationExpansionPredictorTest.cpp \
				$(top_srcdir)/src/lib/predictors/abbreviationExpansionPredictor.h \
				$(top_srcdir)/src/lib/predictors/abbreviationExpansionPredictor.cpp
predictorsTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
predictorsTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
predictorsTestRunner_LDADD =	$(top_builddir)/src/lib/predictors/libpredictor.la

if USE_SQLITE
predictorsTestRunner_SOURCES +=	smoothedNgramPredictorTest.h \
				smoothedNgramPredictorTest.cpp \
				$(top_srcdir)/src/lib/predictors/smoothedNgramPredictor.cpp \
				$(top_srcdir)/src/lib/predictors/smoothedNgramPredictor.h 
predictorsTestRunner_LDADD +=	$(top_builddir)/src/lib/predictors/dbconnector/libdbconnector.la
endif

# the following test runner links with libpresage, instead of using
# mock objects in place of the core libsootsayer library facilities
newPredictorsTestRunner_SOURCES = 	predictorsTestRunner.cpp \
				recencyPredictorTest.h \
				recencyPredictorTest.cpp \
				dejavuPredictorTest.h \
				dejavuPredictorTest.cpp 
newPredictorsTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
newPredictorsTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
newPredictorsTestRunner_LDADD =	$(top_builddir)/src/lib/predictors/libpredictor.la	\
				$(top_builddir)/src/lib/libpresage.la

if USE_SQLITE
newPredictorsTestRunner_SOURCES += \
				newSmoothedNgramPredictorTest.h \
				newSmoothedNgramPredictorTest.cpp
predictorsTestRunner_LDADD +=	$(top_builddir)/src/lib/predictors/dbconnector/libdbconnector.la 
endif

endif # HAVE_CPPUNIT

AM_CPPFLAGS =	-I$(top_srcdir)/src/lib
