
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

check_PROGRAMS = $(TESTS)              \
		dictionaryPluginDriver \
		dummyPluginDriver      \
		smoothedCountDriver    

if HAVE_CPPUNIT

TESTS =	pluginsTestRunner newPluginsTestRunner

pluginsTestRunner_SOURCES =	pluginsTestRunner.cpp \
				pluginsTestMockObjects.h \
				pluginsTestMockObjects.cpp \
				pluginsTestFixture.h \
				pluginsTestFixture.cpp \
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
				smoothedNgramPluginTest.h \
				smoothedNgramPluginTest.cpp \
				$(top_srcdir)/src/lib/plugins/smoothedNgramPlugin.cpp \
				$(top_srcdir)/src/lib/plugins/smoothedNgramPlugin.h \
				abbreviationExpansionPluginTest.h \
				abbreviationExpansionPluginTest.cpp \
				$(top_srcdir)/src/lib/plugins/abbreviationExpansionPlugin.h \
				$(top_srcdir)/src/lib/plugins/abbreviationExpansionPlugin.cpp
pluginsTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
pluginsTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
pluginsTestRunner_LDADD =	$(top_builddir)/src/lib/plugins/libplugin.la               \
				$(top_builddir)/src/lib/core/dbconnector/libdbconnector.la 

# the following test runner links with libpresage, instead of using
# mock objects in place of the core libsootsayer library facilities
newPluginsTestRunner_SOURCES = 	pluginsTestRunner.cpp \
				recencyPluginTest.h \
				recencyPluginTest.cpp \
				dejavuPluginTest.h \
				dejavuPluginTest.cpp \
				newSmoothedNgramPluginTest.h \
				newSmoothedNgramPluginTest.cpp
newPluginsTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
newPluginsTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
newPluginsTestRunner_LDADD =	$(top_builddir)/src/lib/plugins/libplugin.la	\
				$(top_builddir)/src/lib/libpresage.la

endif # HAVE_CPPUNIT

dictionaryPluginDriver_SOURCES =	dictionaryPluginDriver.cpp
dictionaryPluginDriver_LDADD =		$(top_builddir)/src/lib/plugins/libDictionaryPlugin.la \
					$(top_builddir)/src/lib/libpresage.la 

dummyPluginDriver_SOURCES = 	dummyPluginDriver.cpp
dummyPluginDriver_LDADD =	$(top_builddir)/src/lib/plugins/libDummyPlugin.la \
				$(top_builddir)/src/lib/libpresage.la 

smoothedCountDriver_SOURCES = 	smoothedCountDriver.cpp
smoothedCountDriver_LDADD =	$(top_builddir)/src/lib/plugins/libSmoothedCountPlugin.la \
				$(top_builddir)/src/lib/libpresage.la

AM_CPPFLAGS =	-I$(top_srcdir)/src/lib
