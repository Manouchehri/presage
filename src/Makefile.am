## Makefile for the src directory of Soothsayer.
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

SUBDIRS = 			core plugins tools simulator tinyxml #plump

lib_LTLIBRARIES = 		libsoothsayer.la
libsoothsayer_la_SOURCES = 	soothsayer.cpp soothsayer.h
libsoothsayer_la_LIBADD =	core/libcore.la       \
				tinyxml/libtinyxml.la \
				tools/libtools.la     \
				plugins/libplugin.la  \
				plugins/libSmoothedUniBiTrigramPlugin.la \
				plugins/libDummyPlugin.la                \
				plugins/libDictionaryPlugin.la           \
				plugins/libSmoothedCountPlugin.la        

#				REVISIT remove libSmoothetUniBiTrigram.la 
#				when reintegrating dynamically loaded plugins

#				REVISIT add libplump.la when using plump
#				plump/src/libplump.la 


bin_PROGRAMS =			soothsayerDemo       \
				soothsayerDemoText   \
				soothsayerSimulator  

soothsayerDemo_SOURCES =	soothsayerDemo.cpp
soothsayerDemo_LDADD =		libsoothsayer.la
soothsayerDemo_LDFLAGS =	-lncurses

soothsayerDemoText_SOURCES = 	soothsayerDemoText.cpp
soothsayerDemoText_LDADD = 	libsoothsayer.la

soothsayerSimulator_SOURCES =	soothsayerSimulator.cpp
soothsayerSimulator_LDADD =	simulator/libsimulator.la \
				libsoothsayer.la          
