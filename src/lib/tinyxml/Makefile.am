
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

EXTRA_DIST = 	changes.txt \
		readme.txt \
		utf8test.gif \
		utf8test.xml \
		utf8testverify.xml


TESTS = xmltest

check_PROGRAMS =	xmltest
xmltest_SOURCES =	xmltest.cpp
xmltest_LDADD = 	libtinyxml.la
#xmltest_CXXFLAGS =	-DTIXML_USE_STL

noinst_LTLIBRARIES =		libtinyxml.la
libtinyxml_la_SOURCES = 	tinyxml.cpp tinyxml.h \
				tinyxmlparser.cpp \
				tinyxmlerror.cpp \
				tinystr.cpp tinystr.h
#libtinyxml_la_CXXFLAGS =	-DTIXML_USE_STL

# Clean out files created during tests.
# Required to make distcheck happy.
DISTCLEANFILES =	utf8testout.xml \
			demotest.xml    \
			textfile.txt    \
			test5.xml       \
			test6.xml       \
			test7.xml
