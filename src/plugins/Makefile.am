## Makefile for the src/plugins directory of Soothsayer.
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

noinst_LTLIBRARIES =	libplugin.la                     \
			libSmoothedUniBiTrigramPlugin.la \
			libDictionaryPlugin.la           \
			libSmoothedCountPlugin.la        \
			libDummyPlugin.la                   


libplugin_la_SOURCES =	plugin.cpp plugin.h

libSmoothedUniBiTrigramPlugin_la_SOURCES =	smoothedUniBiTrigramPlugin.cpp \
						smoothedUniBiTrigramPlugin.h   
libSmoothedUniBiTrigramPlugin_la_LIBADD =	libplugin.la

###
# Dynamically loadable plugin module
# lib_LTLIBRARIES =			smoothedUniBiTrigramPlugin.la
# 
# smoothedUniBiTrigramPlugin_la_SOURCES = smoothedUniBiTrigramPlugin.cpp \
# 					smoothedUniBiTrigramPlugin.h
# smoothedUniBiTrigramPlugin_la_LIBADD =	libplugin.la
# smoothedUniBiTrigramPlugin_la_LDFLAGS =	-module -no-undefined 


libDictionaryPlugin_la_SOURCES =	dictionaryPlugin.cpp    \
					dictionaryPlugin.h      
libDictionaryPlugin_la_LIBADD =		libplugin.la

libSmoothedCountPlugin_la_SOURCES =	smoothedCountPlugin.cpp \
					smoothedCountPlugin.h
libSmoothedCountPlugin_la_LIBADD =	libplugin.la

libDummyPlugin_la_SOURCES =		dummyPlugin.cpp         \
					dummyPlugin.h           
libDummyPlugin_la_LIBADD =		libplugin.la


AM_CPPFLAGS = -I$(top_srcdir)/src
