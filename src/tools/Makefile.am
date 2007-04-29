## Makefile for the src/tools directory of Soothsayer.
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

bin_PROGRAMS =			text2ngram     

text2ngram_SOURCES =		text2ngram.cpp

LDADD =				libtools.la        \
				../core/libcore.la

noinst_LTLIBRARIES =	libtools.la

libtools_la_SOURCES =	progress.cpp progress.h           \
			ngram.cpp ngram.h                 \
			databaseConnector.cpp             \
			databaseConnector.h               \
			sqliteDatabaseConnector.cpp       \
			sqliteDatabaseConnector.h         


AM_CPPFLAGS =	-I$(top_srcdir)/src/core
