## Makefile for the src/core directory of Soothsayer.
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

SUBDIRS =		tokenizer dbconnector

noinst_LTLIBRARIES = 	libcore.la

libcore_la_SOURCES =	prediction.cpp prediction.h         \
			suggestion.cpp suggestion.h         \
			utility.cpp utility.h               \
			iso8859_1.h                         \
			charsets.h                          \
			contextTracker.cpp contextTracker.h \
			predictor.cpp predictor.h           \
			combiner.h combiner.cpp             \
			meritocracyCombiner.h               \
			meritocracyCombiner.cpp             \
			linearCombiner.h                    \
			backoffCombiner.h                   \
			maxEntropyCombiner.h                \
			selector.cpp selector.h             \
			profile.cpp profile.h               \
			profileManager.cpp profileManager.h \
			logger.cpp logger.h                 

# Temporarily disabled
#                       pluginManager.cpp pluginManager.h

libcore_la_LIBADD =	tokenizer/libtokenizer.la     \
			dbconnector/libdbconnector.la 

AM_CPPFLAGS =		-I$(top_srcdir)/src
