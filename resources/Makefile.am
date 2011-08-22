
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

SUBDIRS =	arpa profiles

EXTRA_DIST = \
	the_picture_of_dorian_gray.txt \
	l_innocente.txt \
	el_quijote.txt \
	abbreviations_en.txt \
	abbreviations_it.txt \
	presage.svg

dist_pkgdata_DATA = \
	presage.png \
	presage.xpm \
	presage.svg \
	abbreviations_en.txt \
	abbreviations_it.txt 

if USE_SQLITE

BUILT_SOURCES =	database_en.db \
		database_it.db \
		database_es.db

database_it.db:	l_innocente.txt
	@rm -f $@
	@$(top_builddir)/src/tools/text2ngram -n 1 -l -f sqlite -o $@ $^
	@$(top_builddir)/src/tools/text2ngram -n 2 -l -f sqlite -o $@ $^
	@$(top_builddir)/src/tools/text2ngram -n 3 -l -f sqlite -o $@ $^

database_en.db:	the_picture_of_dorian_gray.txt
	@rm -f $@
	@$(top_builddir)/src/tools/text2ngram -n 1 -l -f sqlite -o $@ $^
	@$(top_builddir)/src/tools/text2ngram -n 2 -l -f sqlite -o $@ $^
	@$(top_builddir)/src/tools/text2ngram -n 3 -l -f sqlite -o $@ $^

database_es.db:	el_quijote.txt
	@rm -f $@
	@$(top_builddir)/src/tools/text2ngram -n 1 -l -f sqlite -o $@ $^
	@$(top_builddir)/src/tools/text2ngram -n 2 -l -f sqlite -o $@ $^
	@$(top_builddir)/src/tools/text2ngram -n 3 -l -f sqlite -o $@ $^

# Clean out files created during tests.
# Required to make distcheck happy.
DISTCLEANFILES =	database_en.db \
			database_it.db \
			database_es.db

pkgdata_DATA =	database_en.db \
		database_it.db \
		database_es.db 

endif
