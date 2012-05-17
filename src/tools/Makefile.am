
###########
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

SUBDIRS =		simulator 

noinst_LTLIBRARIES =	libtools.la
libtools_la_SOURCES =	ngram.cpp ngram.h

bin_PROGRAMS =		presage_demo_text \
			presage_simulator

if USE_SQLITE
bin_PROGRAMS +=		text2ngram

text2ngram_SOURCES =	text2ngram.cpp
text2ngram_LDADD =	libtools.la \
			../lib/predictors/dbconnector/libdbconnector.la \
			../lib/core/libcore.la \
			../lib/libpresage.la
endif

if HAVE_CURSES
bin_PROGRAMS +=		presage_demo

presage_demo_SOURCES =	presageDemo.cpp
presage_demo_LDADD =	../lib/libpresage.la
presage_demo_LDFLAGS =	-lcurses
endif

presage_demo_text_SOURCES = 	presageDemoText.cpp
presage_demo_text_LDADD = 	../lib/libpresage.la

presage_simulator_SOURCES =	presageSimulator.cpp
presage_simulator_LDADD =	simulator/libsimulator.la \
				../lib/core/libcore.la \
				../lib/libpresage.la

AM_CPPFLAGS =	-I$(top_srcdir)/src/lib -I$(top_srcdir)/src


if HAVE_HELP2MAN
presage_demo_text.1:	presage_demo_text$(EXEEXT) $(top_srcdir)/configure.ac
	help2man --output=$@ --no-info --name="presage demo program (text)" ./presage_demo_text$(EXEEXT)

presage_simulator.1:	presage_simulator$(EXEEXT) presageSimulator.cpp $(top_srcdir)/configure.ac
	help2man --output=$@ --no-info --name="presage simulator program" ./presage_simulator$(EXEEXT)

dist_man_MANS =		presage_demo_text.1 \
			presage_simulator.1

DISTCLEANFILES =	presage_demo_text.1 \
			presage_simulator.1

if USE_SQLITE
text2ngram.1:		text2ngram$(EXEEXT) $(top_srcdir)/configure.ac
	help2man --output=$@ --no-info --name="generate statistical n-gram data from text" ./text2ngram$(EXEEXT)

dist_man_MANS +=	text2ngram.1

DISTCLEANFILES +=	text2ngram.1
endif

if HAVE_CURSES
presage_demo.1:		presage_demo$(EXEEXT) $(top_srcdir)/configure.ac
	help2man --output=$@ --no-info --name="presage demo program (ncurses)" ./presage_demo$(EXEEXT)

dist_man_MANS +=	presage_demo.1

DISTCLEANFILES +=	presage_demo.1
endif

endif
