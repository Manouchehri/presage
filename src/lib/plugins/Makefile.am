
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

noinst_LTLIBRARIES =	libplugin.la                      \
			libSmoothedNgramPlugin.la         \
			libDictionaryPlugin.la            \
			libSmoothedCountPlugin.la         \
			libDummyPlugin.la                 \
			libAbbreviationExpansionPlugin.la \
			libRecencyPlugin.la               \
			libDejavuPlugin.la                \
			libARPAPlugin.la


libplugin_la_SOURCES =	plugin.cpp plugin.h

libSmoothedNgramPlugin_la_SOURCES =		smoothedNgramPlugin.cpp \
						smoothedNgramPlugin.h   

libAbbreviationExpansionPlugin_la_SOURCES =	abbreviationExpansionPlugin.h   \
						abbreviationExpansionPlugin.cpp 

libDictionaryPlugin_la_SOURCES =	dictionaryPlugin.cpp    \
					dictionaryPlugin.h      

libSmoothedCountPlugin_la_SOURCES =	smoothedCountPlugin.cpp \
					smoothedCountPlugin.h

libDummyPlugin_la_SOURCES =		dummyPlugin.cpp         \
					dummyPlugin.h           

libRecencyPlugin_la_SOURCES =		recencyPlugin.cpp	\
					recencyPlugin.h
libRecencyPlugin_la_LDFLAGS =		-lm

libDejavuPlugin_la_SOURCES =		dejavuPlugin.cpp        \
					dejavuPlugin.h

libARPAPlugin_la_SOURCES =		ARPAPlugin.cpp		\
					ARPAPlugin.h

AM_CPPFLAGS = -I$(top_srcdir)/src/lib
