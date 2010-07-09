
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

EXTRA_DIST =	presage.xml.template generate_presage_config.sh

BUILT_SOURCES =	presage.xml
CLEANFILES =	presage.xml

# added dependency on Makefile to regenerate configuration file in the
# event it had previously been built and then the package reconfigured
# to install to a different location.
presage.xml:	Makefile presage.xml.template generate_presage_config.sh
	${srcdir}/generate_presage_config.sh ${srcdir}/presage.xml.template presage.xml ${pkgdatadir};

# ${prefix}/etc directory
sysconf_DATA =	presage.xml


#
# ${prefix}/share directory
# dist_data_DATA =	presage.xml
#
# ${prefix}/share/${package-name} directory
# dist_pkgdata_DATA =	presage.xml
# 
# ${prefix}/var directory
# dist_localstate_DATA =	presage.xml
# 
# ${prefix}/com directory
# dist_sharedstate_DATA =	presage.xml
# 
