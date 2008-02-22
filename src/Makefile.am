
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

SUBDIRS = 			core plugins tools simulator tinyxml #plump

include_HEADERS =		soothsayer.h

lib_LTLIBRARIES = 		libsoothsayer.la
libsoothsayer_la_SOURCES = 	soothsayer.cpp soothsayer.h
libsoothsayer_la_LIBADD =	core/libcore.la       \
				tinyxml/libtinyxml.la \
				tools/libtools.la     \
				plugins/libplugin.la  \
				plugins/libSmoothedNgramPlugin.la         \
				plugins/libSmoothedUniBiTrigramPlugin.la  \
				plugins/libAbbreviationExpansionPlugin.la \
				plugins/libDummyPlugin.la                 \
				plugins/libDictionaryPlugin.la            \
				plugins/libSmoothedCountPlugin.la         

#				REVISIT remove plugins/lib*.la entries
#				when reintegrating dynamically loaded plugins

#				REVISIT add libplump.la when using plump
#				plump/src/libplump.la 
libsoothsayer_la_LDFLAGS =	-no-undefined

bin_PROGRAMS =			soothsayerDemoText   \
				soothsayerSimulator  

if HAVE_CURSES
bin_PROGRAMS +=			soothsayerDemo       

soothsayerDemo_SOURCES =	soothsayerDemo.cpp
soothsayerDemo_LDADD =		libsoothsayer.la
soothsayerDemo_LDFLAGS =	-lcurses
endif

soothsayerDemoText_SOURCES = 	soothsayerDemoText.cpp
soothsayerDemoText_LDADD = 	libsoothsayer.la

soothsayerSimulator_SOURCES =	soothsayerSimulator.cpp
soothsayerSimulator_LDADD =	simulator/libsimulator.la \
				libsoothsayer.la  

dist_man_MANS =	soothsayerDemo.1      \
		soothsayerDemoText.1  \
		soothsayerSimulator.1 

if HAVE_HELP2MAN
soothsayerDemo.1:	soothsayerDemo.cpp $(top_srcdir)/configure.ac
	$(MAKE) $(AM_MAKEFLAGS) soothsayerDemo$(EXEEXT)
	help2man --output=$@ --name="soothsayer demo program (ncurses)" ./soothsayerDemo$(EXEEXT)

soothsayerDemoText.1:	soothsayerDemoText.cpp $(top_srcdir)/configure.ac
	$(MAKE) $(AM_MAKEFLAGS) soothsayerDemoText$(EXEEXT)
	help2man --output=$@ --name="soothsayer demo program (text)" ./soothsayerDemoText$(EXEEXT)

soothsayerSimulator.1:	soothsayerSimulator.cpp $(top_srcdir)/configure.ac
	$(MAKE) $(AM_MAKEFLAGS) soothsayerSimulator$(EXEEXT)
	help2man --output=$@ --name="soothsayer simulator program" ./soothsayerSimulator$(EXEEXT)

endif

