
###########
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

if BUILD_GTK_PRESAGE_MATE

bin_PROGRAMS =		gpresagemate

gpresagemate_SOURCES = 	gpresagemate.cpp
gpresagemate_CPPFLAGS =	$(GNOME_CFLAGS) $(GTHREAD_CFLAGS) -I$(top_srcdir)/src/lib
gpresagemate_LDADD = 	$(GNOME_LIBS) $(GTHREAD_LIBS) -lXevie ../../../src/lib/libpresage.la

# if HAVE_HELP2MAN
# gpresagemate.1:	gpresagemate$(EXEEXT) gpresagemate.cpp $(top_srcdir)/configure.ac
# 	help2man --output=$@ --no-info --name="GTK presage mate" ./gpresagemate$(EXEEXT)
# 
# dist_man_MANS =	gpresagemate.1
#
# DISTCLEANFILES =	gpresagemate.1
# endif

endif