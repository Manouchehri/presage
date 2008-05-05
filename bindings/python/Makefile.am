
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

SOOTHSAYER_INTERFACE =	../../src/lib/soothsayer.h ../../src/lib/soothsayerException.h
SWIG_INTERFACE =	../soothsayer.i

BUILT_SOURCES =		soothsayer_wrap.cpp soothsayer.py
CLEANFILES =		soothsayer_wrap.cpp soothsayer.py

EXTRA_DIST =	runme.py fix_wrapper.sed

all-local:	soothsayer_wrap.cpp soothsayer.py
	@case "`uname`" in \
	  MINGW*) \
		$(PYTHON) setup.py build --compiler=mingw32 \
	  ;; \
	  *) \
		$(PYTHON) setup.py build \
	  ;; \
	esac;

# python on MinGW/MSYS requires pure Windows style paths
# Using following (cd $dir && pwd -W) hack to get a nearly
# native Windows path, then translating the forward slash
# to a backward slash to make python distutils jolly.
# P.S. the backslash has to be escaped thrice, hence a single
# backslash turns into a sequence of eight (neat, huh?)
#
install-exec-local:
	@case "`uname`" in \
	  MINGW*) \
		NATIVE_WINDOWS_PREFIX="`cd $(prefix) && pwd -W`"; \
		NATIVE_WINDOWS_PREFIX="`echo $$NATIVE_WINDOWS_PREFIX | sed -e 's|/|\\\\\\\\|g'`"; \
		$(PYTHON) setup.py install --prefix="$$NATIVE_WINDOWS_PREFIX" \
	  ;; \
	  *) \
		$(PYTHON) setup.py install --prefix=$(prefix) \
	  ;; \
	esac;

clean-local:
	rm -rf build

soothsayer_wrap.cpp soothsayer.py:	$(SWIG_INTERFACE) $(SOOTHSAYER_INTERFACE)
	$(SWIG) -c++ -python -I$(top_srcdir)/src/lib -o soothsayer_wrap.cpp -outdir . $(srcdir)/$(SWIG_INTERFACE)
	@case "`uname`" in \
	  CYGWIN*) \
		echo "WARNING: working around bug in SWIG on Windows/Cygwin platforms ONLY"; \
		sed -i.bak -f $(srcdir)/fix_wrapper.sed soothsayer_wrap.cpp; \
	  ;; \
	esac;

endif # ENABLE_PYTHON_BINDING
endif # HAVE_PYTHON
endif # HAVE_SWIG
