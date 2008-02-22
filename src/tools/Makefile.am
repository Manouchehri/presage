
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

bin_PROGRAMS =		text2ngram     

text2ngram_SOURCES =	text2ngram.cpp
text2ngram_LDADD =	libtools.la        \
			../core/libcore.la

noinst_LTLIBRARIES =	libtools.la

libtools_la_SOURCES =	progress.cpp progress.h           \
			ngram.cpp ngram.h                 
libtools_la_LIBADD =	$(SQLITE_LIBS)

AM_CPPFLAGS =	-I$(top_srcdir)/src

dist_man_MANS =	text2ngram.1

if HAVE_HELP2MAN
text2ngram.1:	text2ngram.cpp $(top_srcdir)/configure.ac
	$(MAKE) $(AM_MAKEFLAGS) text2ngram$(EXEEXT)
	help2man --output=$@ --name="generate statistical n-gram data from text" ./text2ngram$(EXEEXT)

endif

