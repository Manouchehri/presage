
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

EXTRA_DIST =	\
	getting_started.txt \
	python_binding.txt \
	INSTALL_Cygwin_dev_env.txt \
	INSTALL_MinGW_MSYS_dev_env.txt	

if BUILD_DOCUMENTATION
if HAVE_DOXYGEN
#pkgdata_DATA =	html latex
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/html
	for i in `find html -type f`; \
	do \
		$(INSTALL_DATA) $$i $(DESTDIR)$(pkgdatadir)/$$i; \
	done
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/latex
	for i in `find latex -type f`; \
	do \
		$(INSTALL_DATA) $$i $(DESTDIR)$(pkgdatadir)/$$i; \
	done

all-local:	doxygen.timestamp

doxygen.timestamp:	Doxyfile
	doxygen
	touch $@

DISTCLEANFILES =	doxygen.timestamp

distclean-local:
	rm -rf html latex

uninstall-local:
	rm -rf $(DESTDIR)$(pkgdatadir)/html $(DESTDIR)$(pkgdatadir)/latex
endif
endif