
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

SUBDIRS = 			tinyxml core simulator plugins #plump

include_HEADERS =		presage.h \
				presageException.h \
				presageCallback.h 

lib_LTLIBRARIES = 		libpresage.la
libpresage_la_SOURCES = 	presage.cpp \
				presage.h \
				presageException.h \
				presageCallback.h 
libpresage_la_LIBADD =		core/libcore.la \
				tinyxml/libtinyxml.la \
				simulator/libsimulator.la \
				plugins/libplugin.la  \
				plugins/libSmoothedNgramPlugin.la \
				plugins/libAbbreviationExpansionPlugin.la \
				plugins/libDummyPlugin.la \
				plugins/libDictionaryPlugin.la \
				plugins/libSmoothedCountPlugin.la \
				plugins/libRecencyPlugin.la \
				plugins/libDejavuPlugin.la \
				plugins/libARPAPlugin.la 

#				REVISIT remove plugins/lib*.la entries
#				when reintegrating dynamically loaded plugins

#				REVISIT add libplump.la when using plump
#				plump/src/libplump.la 

libpresage_la_LDFLAGS =		-no-undefined \
				-version-info 1:0:0 
