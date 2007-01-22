## Makefile for the test/plugins directory of Soothsayer.
## Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>
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

TESTS =	

check_PROGRAMS = $(TESTS) \
		dictionaryPluginDriver \
		dummyPluginDriver \
		smoothedCountDriver \
		smoothedUniBiTrigramDriver

dictionaryPluginDriver_SOURCES =	dictionaryPluginDriver.cpp \
					$(top_srcdir)/src/plugins/dictionaryPlugin.cpp \
					$(top_srcdir)/src/plugins/dictionaryPlugin.h
dictionaryPluginDriver_LDFLAGS = 	-lsqlite

dummyPluginDriver_SOURCES = 	dummyPluginDriver.cpp \
				$(top_srcdir)/src/plugins/dummyPlugin.cpp \
				$(top_srcdir)/src/plugins/dummyPlugin.h
dummyPluginDriver_LDFLAGS = 	-lsqlite

smoothedCountDriver_SOURCES = 	smoothedCountDriver.cpp \
				$(top_srcdir)/src/plugins/smoothedCountPlugin.cpp \
				$(top_srcdir)/src/plugins/smoothedCountPlugin.h
smoothedCountDriver_LDFLAGS = 	-lsqlite

smoothedUniBiTrigramDriver_SOURCES = 	smoothedUniBiTrigramDriver.cpp \
					$(top_srcdir)/src/plugins/smoothedUniBiTrigramPlugin.cpp \
					$(top_srcdir)/src/plugins/smoothedUniBiTrigramPlugin.h
smoothedUniBiTrigramDriver_LDFLAGS =	-lsqlite

LDADD =	$(top_builddir)/src/libsoothsayer.la 

AM_CPPFLAGS =	-I$(top_srcdir)/src

DEPENDENCIES =	libsoothsayer.la
