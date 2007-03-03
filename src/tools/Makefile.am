## Makefile for the src/tools directory of Soothsayer.
## Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>
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

bin_PROGRAMS =	text2unigram   \
		text2bigram    \
		text2trigram   \
		text2unigramdb \
		text2bigramdb  \
		text2trigramdb \
		text2ngram     

text2unigram_SOURCES = 		text2unigram.cpp
text2bigram_SOURCES = 		text2bigram.cpp
text2trigram_SOURCES = 		text2trigram.cpp
text2unigramdb_SOURCES =	text2unigramdb.cpp
text2bigramdb_SOURCES = 	text2bigramdb.cpp
text2trigramdb_SOURCES =	text2trigramdb.cpp
text2ngram_SOURCES =		text2ngram.cpp

LDADD =				libtools.la        \
				../core/libcore.la

text2unigramdb_LDFLAGS =	-lsqlite
text2bigramdb_LDFLAGS =		-lsqlite
text2trigramdb_LDFLAGS =	-lsqlite


noinst_LTLIBRARIES =	libtools.la

libtools_la_SOURCES =	tokenizer.cpp tokenizer.h    \
			../core/iso8859_1.h          \
			database.cpp database.h      \
			progress.cpp progress.h      \
			trigram.cpp trigram.h        \
			bigram.cpp bigram.h          \
			ngram.cpp ngram.h            \
			databaseConnector.cpp        \
			databaseConnector.h          \
			sqliteDatabaseConnector.cpp  \
			sqliteDatabaseConnector.h    


AM_CPPFLAGS =	-I$(top_srcdir)/src/core
