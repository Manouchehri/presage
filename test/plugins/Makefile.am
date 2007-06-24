## Makefile for the test/plugins directory of Soothsayer.
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

TESTS =	pluginsTestRunner

check_PROGRAMS = $(TESTS) \
		dictionaryPluginDriver \
		dummyPluginDriver \
		smoothedCountDriver \
		smoothedUniBiTrigramDriver

pluginsTestRunner_SOURCES =	pluginsTestRunner.cpp                                    \
				smoothedUniBiTrigramPluginTest.h                         \
				smoothedUniBiTrigramPluginTest.cpp                       \
				$(top_srcdir)/src/plugins/smoothedUniBiTrigramPlugin.cpp \
				$(top_srcdir)/src/plugins/smoothedUniBiTrigramPlugin.h   \
				$(top_srcdir)/src/core/prediction.h                      \
				$(top_srcdir)/src/core/prediction.cpp                    \
				$(top_srcdir)/src/core/suggestion.h                      \
				$(top_srcdir)/src/core/suggestion.cpp                    \
				$(top_srcdir)/src/core/utility.h                         \
				$(top_srcdir)/src/core/utility.cpp                       

pluginsTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
pluginsTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
pluginsTestRunner_LDADD =	$(top_builddir)/src/plugins/libplugin.la \
				$(top_builddir)/src/tools/libtools.la    

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

smoothedUniBiTrigramDriver_SOURCES = 	smoothedUniBiTrigramDriver.cpp \
					$(top_srcdir)/src/plugins/smoothedUniBiTrigramPlugin.cpp \
					$(top_srcdir)/src/plugins/smoothedUniBiTrigramPlugin.h
smoothedUniBiTrigramDriver_LDFLAGS =	-lsqlite
smoothedUniBiTrigramDriver_LDADD =	$(top_builddir)/src/libsoothsayer.la 

AM_CPPFLAGS =	-I$(top_srcdir)/src
