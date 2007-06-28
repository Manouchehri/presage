## Makefile for the resources/profiles directory of Soothsayer.
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

EXTRA_DIST =	soothsayer.xml.template generate_soothsayer_config.sh

BUILT_SOURCES =	soothsayer.xml
CLEANFILES =	soothsayer.xml

# added dependency on Makefile to regenerate configuration file in the
# event it had previously been built and then the package reconfigured
# to install to a different location.
soothsayer.xml:	Makefile soothsayer.xml.template generate_soothsayer_config.sh
	${srcdir}/generate_soothsayer_config.sh ${srcdir} ${localstatedir}

# ${prefix}/etc directory
nodist_sysconf_DATA =	soothsayer.xml


#
# ${prefix}/share directory
# dist_data_DATA =	soothsayer.xml
#
# ${prefix}/share/${package-name} directory
# dist_pkgdata_DATA =	soothsayer.xml
# 
# ${prefix}/var directory
# dist_localstate_DATA =	soothsayer.xml
# 
# ${prefix}/com directory
# dist_sharedstate_DATA =	soothsayer.xml
# 
