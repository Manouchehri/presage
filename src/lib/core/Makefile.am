
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

SUBDIRS =	\
	tokenizer \
	context_tracker

noinst_LTLIBRARIES = 	\
	libcore.la

libcore_la_SOURCES =	\
	prediction.cpp \
	prediction.h \
	suggestion.cpp \
	suggestion.h \
	utility.cpp \
	utility.h \
	iso8859_1.h \
	charsets.h \
	observable.cpp \
	observable.h \
	observer.cpp \
	observer.h \
	dispatcher.h \
	dispatcher.cpp \
	predictorActivator.cpp \
	predictorActivator.h \
	combiner.h \
	combiner.cpp \
	meritocracyCombiner.h \
	meritocracyCombiner.cpp \
	linearCombiner.h \
	backoffCombiner.h \
	maxEntropyCombiner.h \
	selector.cpp \
	selector.h \
	variable.cpp \
	variable.h \
	configuration.cpp \
	configuration.h \
	profile.cpp \
	profile.h \
	defaultProfile.cpp \
	defaultProfile.h \
	profileManager.cpp \
	profileManager.h \
	logger.cpp \
	logger.h \
	presageException.cpp \
	../presageException.h \
	predictorRegistry.cpp \
	predictorRegistry.h \
	progress.cpp \
	progress.h 

libcore_la_LIBADD =	\
	tokenizer/libtokenizer.la \
	context_tracker/libcontexttracker.la

libcore_la_CPPFLAGS =	-I$(top_srcdir)/src/lib

if BUILD_TINYXML
libcore_la_CPPFLAGS +=	-I$(top_srcdir)/src/lib/tinyxml
endif
