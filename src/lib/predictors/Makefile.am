
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

SUBDIRS = dbconnector

PREDICTORS = \
	libpredictor.la \
	libDummyPredictor.la \
	libDictionaryPredictor.la \
	libAbbreviationExpansionPredictor.la \
	libRecencyPredictor.la \
	libDejavuPredictor.la \
	libARPAPredictor.la

if USE_SQLITE
PREDICTORS += \
	libSmoothedNgramPredictor.la

DBCONNECTOR = \
	dbconnector/libdbconnector.la
endif

noinst_LTLIBRARIES = \
	libpredictors.la \
	$(PREDICTORS)

libpredictors_la_SOURCES = 
libpredictors_la_LIBADD =	$(PREDICTORS) $(DBCONNECTOR)

libpredictor_la_SOURCES =	predictor.cpp predictor.h

libSmoothedNgramPredictor_la_SOURCES =	smoothedNgramPredictor.cpp \
					smoothedNgramPredictor.h   

libAbbreviationExpansionPredictor_la_SOURCES =	abbreviationExpansionPredictor.h   \
						abbreviationExpansionPredictor.cpp 

libDictionaryPredictor_la_SOURCES =	dictionaryPredictor.cpp \
					dictionaryPredictor.h      

libDummyPredictor_la_SOURCES =		dummyPredictor.cpp \
					dummyPredictor.h           

libRecencyPredictor_la_SOURCES =	recencyPredictor.cpp \
					recencyPredictor.h
libRecencyPredictor_la_LDFLAGS =	-lm

libDejavuPredictor_la_SOURCES =		dejavuPredictor.cpp \
					dejavuPredictor.h

libARPAPredictor_la_SOURCES =		ARPAPredictor.cpp \
					ARPAPredictor.h
