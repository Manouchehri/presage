
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

if HAVE_SWIG
if HAVE_PYTHON
if ENABLE_PYTHON_BINDING


# pypresagemate uses autotools
dist_bin_SCRIPTS = pypresagemate.py

CLEANFILES = $(dist_bin_SCRIPTS)

# presage_prompter delegates to setup.py
EXTRA_DIST =	presage_prompter \
		prompter/__init__.py \
		prompter/prompter.py

all-local:	$(EXTRA_DIST)
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
	rm -rf $(DESTDIR)$(libdir)/python*/site-packages/prompter
	rm -f $(DESTDIR)$(bindir)/presage_prompter

clean-local:
	$(PYTHON) setup.py clean --all


if HAVE_HELP2MAN

presage_prompter.1:	presage_prompter.in $(top_srcdir)/configure.ac
	chmod u+x ./presage_prompter
	help2man --output=$@ --name="Prompter, the intelligent mind reader" ./presage_prompter

dist_man_MANS =	presage_prompter.1

endif

endif
endif
endif
