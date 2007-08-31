## Makefile for the bindings directory of Soothsayer.
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

#SUBDIRS =	python

if HAVE_SWIG
if HAVE_PYTHON
if ENABLE_PYTHON_BINDING

SWIG_SOURCES =			soothsayer.i

BUILT_SOURCES =			$(srcdir)/soothsayer_wrap.cpp $(srcdir)/soothsayer.py
CLEANFILES =			$(srcdir)/soothsayer_wrap.cpp $(srcdir)/soothsayer.py

pkgpython_PYTHON =		soothsayer.py
pkgpyexec_LTLIBRARIES =		_soothsayer.la
_soothsayer_la_SOURCES =	$(srcdir)/soothsayer_wrap.cpp $(SWIG_SOURCES)
_soothsayer_la_CPPFLAGS =	$(SWIG_PYTHON_CPPFLAGS) -I$(top_srcdir)/src
_soothsayer_la_LDFLAGS =	-module -no-undefined -avoid-version $(PYTHON_LDFLAGS)
_soothsayer_la_LIBADD =		../src/libsoothsayer.la

$(srcdir)/soothsayer_wrap.cpp:	$(SWIG_SOURCES)
	$(SWIG) $(SWIG_PYTHON_OPT) -I$(top_srcdir)/src -o $@ $<

endif # ENABLE_PYTHON_BINDING
endif # HAVE_PYTHON
endif # HAVE_SWIG
