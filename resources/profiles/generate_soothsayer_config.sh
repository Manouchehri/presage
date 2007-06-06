#!/bin/bash

## Soothsayer, an extensible predictive text entry system
## ------------------------------------------------------
##
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


# This script takes the directory that contains $TEMPLATE and the
# localstatedir as arguments and writes to $OUTPUT file, replacing all
# tokens (in the form of ::TOKEN::) in $TEMPLATE.

TEMPLATE=soothsayer.xml.template
OUTPUT=soothsayer.xml

if [ $# -lt 2 ];
then
    echo "Usage: $0 srcdir localstatedir"
    exit 1
fi

srcdir=$1
localstatedir=$2

# Determining CYGWIN_ROOT is required to work around a problem with
# Sqlite on Cygwin, whereby Sqlite will not be able to open a database
# file when specified as an absolute path rooted at /
# (e.g. /var/database_en.db)
#
# Sqlite has no problem using a relative path (which is of no use to
# us) or opening a database file rooted at cygwin's root
# (e.g. c:/cygwin/var/database_en.db)

if [ `uname | grep CYGWIN` ];
then
    CYGWIN_ROOT=`mount | grep ' on / type ' | awk '{print $1}' | tr \\\\ /`
else
    CYGWIN_ROOT=
fi

# Replace the token in $TEMPLATE and write result to $OUTPUT
sed -e "s|::LOCALSTATEDIR::|${CYGWIN_ROOT}${localstatedir}|" ${srcdir}/${TEMPLATE} > ${OUTPUT}
