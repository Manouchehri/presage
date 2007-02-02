## Makefile for the toplevel directory of Soothsayer.
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

SUBDIRS =	src test resources

BUILT_SOURCES =	sysconfdir.h datadir.h pkgdatadir.h localstatedir.h
CLEANFILES =	sysconfdir.h datadir.h pkgdatadir.h localstatedir.h

sysconfdir.h:	Makefile
	echo '#define sysconfdir "$(sysconfdir)"' > $@

datadir.h:	Makefile
	echo '#define datadir "$(datadir)"' > $@

pkgdatadir.h:	Makefile
	echo '#define pkgdatadir "$(pkgdatadir)"' > $@

localstatedir.h:	Makefile
	echo '#define localstatedir "$(localstatedir)"' > $@

EXTRA_DIST =	INSTALL_cygwin_dev_env.txt
