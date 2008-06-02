
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

noinst_LTLIBRARIES =	libtools.la
libtools_la_SOURCES =	progress.cpp progress.h \
			ngram.cpp ngram.h                 

bin_PROGRAMS =		text2ngram \
			soothsayer_demo_text \
			soothsayer_simulator

if HAVE_CURSES
bin_PROGRAMS +=			soothsayer_demo       

soothsayer_demo_SOURCES =	soothsayerDemo.cpp
soothsayer_demo_LDADD =		../lib/libsoothsayer.la
soothsayer_demo_LDFLAGS =	-lcurses
endif

soothsayer_demo_text_SOURCES = 	soothsayerDemoText.cpp
soothsayer_demo_text_LDADD = 	../lib/libsoothsayer.la

soothsayer_simulator_SOURCES =	soothsayerSimulator.cpp
soothsayer_simulator_LDADD =	../lib/libsoothsayer.la  

text2ngram_SOURCES =		text2ngram.cpp
text2ngram_LDADD =		../lib/libsoothsayer.la \
				libtools.la

AM_CPPFLAGS =			-I$(top_srcdir)/src/lib -I$(top_srcdir)/src


if HAVE_HELP2MAN
text2ngram.1:	text2ngram.cpp $(top_srcdir)/configure.ac
	$(MAKE) $(AM_MAKEFLAGS) text2ngram$(EXEEXT)
	help2man --output=$@ --name="generate statistical n-gram data from text" ./text2ngram$(EXEEXT)

soothsayer_demo_text.1:	soothsayerDemoText.cpp $(top_srcdir)/configure.ac
	$(MAKE) $(AM_MAKEFLAGS) soothsayer_demo_text$(EXEEXT)
	help2man --output=$@ --name="soothsayer demo program (text)" ./soothsayer_demo_text$(EXEEXT)

soothsayer_simulator.1:	soothsayerSimulator.cpp $(top_srcdir)/configure.ac
	$(MAKE) $(AM_MAKEFLAGS) soothsayer_simulator$(EXEEXT)
	help2man --output=$@ --name="soothsayer simulator program" ./soothsayer_simulator$(EXEEXT)

dist_man_MANS =	text2ngram.1 \
		soothsayer_demo_text.1 \
		soothsayer_simulator.1

if HAVE_CURSES
soothsayer_demo.1:	soothsayerDemo.cpp $(top_srcdir)/configure.ac
	$(MAKE) $(AM_MAKEFLAGS) soothsayer_demo$(EXEEXT)
	help2man --output=$@ --name="soothsayer demo program (ncurses)" ./soothsayer_demo$(EXEEXT)

dist_man_MANS +=	soothsayer_demo.1
endif

endif
