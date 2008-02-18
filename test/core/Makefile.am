## Makefile for the test/core directory of Soothsayer.
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

SUBDIRS =	tokenizer dbconnector

EXTRA_DIST =	profileTest.xml

if HAVE_CPPUNIT

TESTS =		coreTestRunner

check_PROGRAMS = $(TESTS)

coreTestRunner_SOURCES =	coreTestRunner.cpp                          \
				suggestionTest.h suggestionTest.cpp         \
				predictionTest.h predictionTest.cpp         \
				tokenizer/testStringSuite.h                 \
				tokenizer/testStringSuite.cpp               \
				contextTrackerTest.h contextTrackerTest.cpp \
				selectorTest.h selectorTest.cpp             \
				profileTest.h profileTest.cpp               \
				profileManagerTest.h profileManagerTest.cpp \
				meritocracyCombinerTest.h meritocracyCombinerTest.cpp \
				loggerTest.h loggerTest.cpp                 \
				variableTest.h variableTest.cpp             

coreTestRunner_CPPFLAGS =	-I$(top_srcdir)/src                 \
				-I$(top_srcdir)/test/core/tokenizer
coreTestRunner_CXXFLAGS =	$(CPPUNIT_CFLAGS)
coreTestRunner_LDFLAGS =	$(CPPUNIT_LIBS)
coreTestRunner_LIBS =		-ldl

# Temporarily disabled
#pluginManagerDriver_SOURCES =	pluginManagerDriver.cpp
##pluginManagerDriver_LDFLAGS =	-ldl


LDADD =	$(top_builddir)/src/libsoothsayer.la

DEPENDENCIES =	libsoothsayer.la

endif
