
#############
#  Soothsayer, an extensible predictive text entry system
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

if HAVE_SWIG
if HAVE_PYTHON
if ENABLE_PYTHON_BINDING

SWIG_INTERFACE =	../soothsayer.i

BUILT_SOURCES =		soothsayer_wrap.cpp soothsayer.py
CLEANFILES =		soothsayer_wrap.cpp soothsayer.py

EXTRA_DIST =	runme.py

all-local:	soothsayer_wrap.cpp soothsayer.py
	$(PYTHON) setup.py build

install-exec-local:
	$(PYTHON) setup.py install --prefix=$(prefix)

clean-local:
	rm -rf build

soothsayer_wrap.cpp soothsayer.py:	$(SWIG_INTERFACE)
	$(SWIG) -c++ -python -I$(top_srcdir)/src -o soothsayer_wrap.cpp -outdir . $(srcdir)/$(SWIG_INTERFACE)
	@if test "x`uname -o`" = "xCygwin"; then \
		echo "WARNING: working around bug in SWIG on Windows/Cygwin platforms ONLY"; \
		sed -i.bak -f $(srcdir)/fix_wrapper.sed soothsayer_wrap.cpp; \
	fi

endif # ENABLE_PYTHON_BINDING
endif # HAVE_PYTHON
endif # HAVE_SWIG
