
#############
#  Soothsayer, an extensible predictive text entry system
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

TESTS =	pluginsTestRunner

pluginsTestRunner_SOURCES =	pluginsTestRunner.cpp                                     \
				pluginsTestMockObjects.h                                  \
				pluginsTestMockObjects.cpp                                \
				pluginsTestFixture.h                                      \
				pluginsTestFixture.cpp                                    \
				$(top_srcdir)/src/core/suggestion.h                       \
				$(top_srcdir)/src/core/suggestion.cpp                     \
				$(top_srcdir)/src/core/prediction.h                       \
				$(top_srcdir)/src/core/prediction.cpp                     \
				$(top_srcdir)/src/core/utility.h                          \
				$(top_srcdir)/src/core/utility.cpp                        \
				$(top_srcdir)/src/core/variable.h                         \
				$(top_srcdir)/src/core/variable.cpp                       \
				smoothedNgramPluginTest.h                          \
				smoothedNgramPluginTest.cpp                        \
				$(top_srcdir)/src/plugins/smoothedNgramPlugin.cpp  \
				$(top_srcdir)/src/plugins/smoothedNgramPlugin.h    \
				smoothedUniBiTrigramPluginTest.h                          \
				smoothedUniBiTrigramPluginTest.cpp                        \
				$(top_srcdir)/src/plugins/smoothedUniBiTrigramPlugin.cpp  \
				$(top_srcdir)/src/plugins/smoothedUniBiTrigramPlugin.h    \
				abbreviationExpansionPluginTest.h                         \
				abbreviationExpansionPluginTest.cpp                       \
				$(top_srcdir)/src/plugins/abbreviationExpansionPlugin.h   \
				$(top_srcdir)/src/plugins/abbreviationExpansionPlugin.cpp 


pluginsTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
pluginsTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
pluginsTestRunner_LDADD =	$(top_builddir)/src/plugins/libplugin.la               \
				$(top_builddir)/src/core/dbconnector/libdbconnector.la 

endif # HAVE_CPPUNIT

dictionaryPluginDriver_SOURCES =	dictionaryPluginDriver.cpp \
					$(top_srcdir)/src/plugins/dictionaryPlugin.cpp \
					$(top_srcdir)/src/plugins/dictionaryPlugin.h
dictionaryPluginDriver_LDADD =		$(top_builddir)/src/libsoothsayer.la 

dummyPluginDriver_SOURCES = 	dummyPluginDriver.cpp \
				$(top_srcdir)/src/plugins/dummyPlugin.cpp \
				$(top_srcdir)/src/plugins/dummyPlugin.h
dummyPluginDriver_LDADD =	$(top_builddir)/src/libsoothsayer.la 

smoothedCountDriver_SOURCES = 	smoothedCountDriver.cpp \
				$(top_srcdir)/src/plugins/smoothedCountPlugin.cpp \
				$(top_srcdir)/src/plugins/smoothedCountPlugin.h
smoothedCountDriver_LDADD =	$(top_builddir)/src/libsoothsayer.la 

AM_CPPFLAGS =	-I$(top_srcdir)/src
