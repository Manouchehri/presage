
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

EXTRA_DIST =	soothsayer.xml.template generate_soothsayer_config.sh

BUILT_SOURCES =	soothsayer.xml
CLEANFILES =	soothsayer.xml

# added dependency on Makefile to regenerate configuration file in the
# event it had previously been built and then the package reconfigured
# to install to a different location.
soothsayer.xml:	Makefile soothsayer.xml.template generate_soothsayer_config.sh
	@case "`uname`" in \
	  MINGW*) \
		NATIVE_LOCALSTATEDIR="`cd ${localstatedir} && pwd -W`"; \
		NATIVE_LOCALSTATEDIR="`echo $$NATIVE_LOCALSTATEDIR | sed -e 's|/|\\\\\\\\|g'`"; \
		${srcdir}/generate_soothsayer_config.sh ${srcdir}/soothsayer.xml.template \
			soothsayer.xml "$$NATIVE_LOCALSTATEDIR\\\\lib\\\\soothsayer"; \
	  ;; \
	  *) \
		${srcdir}/generate_soothsayer_config.sh ${srcdir}/soothsayer.xml.template \
			soothsayer.xml ${localstatedir}/lib/soothsayer; \
	  ;; \
	esac;

# ${prefix}/etc directory
sysconf_DATA =	soothsayer.xml


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
