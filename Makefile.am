## Makefile for the toplevel directory of Soothsayer.
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

SUBDIRS =	src test resources bindings doc

BUILT_SOURCES =	dirs.h
CLEANFILES =	dirs.h

dirs.h:	Makefile
	echo '#define prefix "$(prefix)"' > $@
	echo '#define bindir "$(bindir)"' >> $@
	echo '#define libdir "$(libdir)"' >> $@
	echo '#define sysconfdir "$(sysconfdir)"' >> $@
	echo '#define datadir "$(datadir)"' >> $@
	echo '#define pkgdatadir "$(pkgdatadir)"' >> $@
	echo '#define localstatedir "$(localstatedir)"' >> $@


EXTRA_DIST =	INSTALL_cygwin_dev_env.txt FAQ

# Ensure local m4 macros are included during autoreconf and automatic
# aclocal.m4 remaking
ACLOCAL_AMFLAGS = -I m4
