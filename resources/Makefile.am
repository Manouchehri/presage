## Makefile for the resources directory of Soothsayer.
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

SUBDIRS =	profiles

EXTRA_DIST =	i_malavoglia.txt                \
		la_coscienza_di_zeno.txt        \
		the_picture_of_dorian_gray.txt

BUILT_SOURCES =	database_en.db \
		database_it.db

database_it.db:	i_malavoglia.txt
	$(top_builddir)/src/tools/text2ngram -n 1 -l -f sqlite -o $@ $^
	$(top_builddir)/src/tools/text2ngram -n 2 -l -f sqlite -o $@ $^
	$(top_builddir)/src/tools/text2ngram -n 3 -l -f sqlite -o $@ $^

database_en.db:	the_picture_of_dorian_gray.txt
	$(top_builddir)/src/tools/text2ngram -n 1 -l -f sqlite -o $@ $^
	$(top_builddir)/src/tools/text2ngram -n 2 -l -f sqlite -o $@ $^
	$(top_builddir)/src/tools/text2ngram -n 3 -l -f sqlite -o $@ $^

nodist_localstate_DATA =	database_en.db \
				database_it.db

# Clean out files created during tests.
# Required to make distcheck happy.
DISTCLEANFILES =	database_en.db \
			database_it.db
