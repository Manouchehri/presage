
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

if HAVE_SWIG
if HAVE_PYTHON
if ENABLE_PYTHON_BINDING

PRESAGE_INTERFACE =	../../src/lib/presage.h \
			../../src/lib/presageException.h \
			../../src/lib/presageCallback.h 
SWIG_INTERFACE =	../presage.i

BUILT_SOURCES =		presage_wrap.cpp presage_wrap.h presage.py
CLEANFILES =		presage_wrap.cpp presage_wrap.h presage.py

EXTRA_DIST =	presage_python_demo

all-local:	presage_wrap.cpp presage_wrap.h presage.py
	@case "`uname`" in \
	  MINGW*) \
		CPPFLAGS="-DMS_WIN64 $(CPPFLAGS)" $(PYTHON) setup.py build --compiler=mingw32 \
	  ;; \
	  *) \
		CPPFLAGS="$(CPPFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" $(PYTHON) setup.py build \
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
		NATIVE_WINDOWS_PREFIX="`cd $(DESTDIR)$(prefix) && pwd -W`"; \
		NATIVE_WINDOWS_PREFIX="`echo $$NATIVE_WINDOWS_PREFIX | sed -e 's|/|\\\\\\\\|g'`"; \
		$(PYTHON) setup.py install --prefix="$$NATIVE_WINDOWS_PREFIX" \
	  ;; \
	  *) \
		$(PYTHON) setup.py install --prefix=$(DESTDIR)$(prefix) \
	  ;; \
	esac;

# This rule cleans up stuff installed by Python's setup.py
# Unfortunately, Python's distutils do not provide an uninstall
# command, so we have to make up for it here in uninstall-local
# hook. This might break if distutils' behaviour changes as automake
# has no control over what distutils install command does.
#
uninstall-local:
	rm -f $(DESTDIR)$(libdir)/python*/site-packages/_presage.so
	rm -f $(DESTDIR)$(libdir)/python*/site-packages/presage.py
	rm -f $(DESTDIR)$(libdir)/python*/site-packages/presage.pyc
	rm -f $(DESTDIR)$(libdir)/python*/site-packages/python_presage-*.egg-info
	rm -f $(DESTDIR)$(bindir)/presage_python_demo

clean-local:
	rm -rf build

presage_wrap.cpp presage_wrap.h presage.py:	$(SWIG_INTERFACE) $(PRESAGE_INTERFACE)
	$(SWIG) -c++ -python -I$(top_srcdir)/src/lib -o presage_wrap.cpp -outdir . $(srcdir)/$(SWIG_INTERFACE)

if HAVE_HELP2MAN
presage_python_demo.1:	presage_python_demo.in $(top_srcdir)/configure.ac
	chmod u+x ./presage_python_demo
	help2man --output=$@ --no-info --name="presage demo program (Python)" "$(PYTHON) ./presage_python_demo"

dist_man_MANS =		presage_python_demo.1

DISTCLEANFILES =	presage_python_demo.1
endif 

endif # ENABLE_PYTHON_BINDING
endif # HAVE_PYTHON
endif # HAVE_SWIG
