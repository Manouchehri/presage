#!/bin/sh

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


# This script takes the configuration $TEMPLATE file, the output file
# name and the pkgdatadir as arguments and writes to $OUTPUT file,
# replacing all tokens (in the form of ::TOKEN::) in $TEMPLATE.

# exit on error
set -e

if [ $# -lt 3 ];
then
    echo "Usage: $0 template output pkgdatadir"
    exit 1
fi

TEMPLATE=$1
OUTPUT=$2
PACKAGEDATADIR=$3

# Determining whether PACKAGEDATADIR is in a format which will be
# understood by SQLite on Cygwin, whereby SQLite will not be
# able to open a database file when specified as an absolute path
# rooted at / (e.g. /var/database_en.db)
#
# Sqlite has no problem using a relative path (which is of no use to
# us) or opening a database file rooted at cygwin's root
# (e.g. c:/cygwin/var/database_en.db)
#
case `uname` in
CYGWIN*)
    PACKAGEDATADIR=`cygpath -m $PACKAGEDATADIR`;
    ;;
MINGW*)
    PACKAGEDATADIR=`cd $PACKAGEDATADIR && pwd -W`;
    PACKAGEDATADIR=`echo $PACKAGEDATADIR | sed -e 's|/|\\\\\\\\|g'`;
    ;;
*)
    ;;
esac

# Replace the token in $TEMPLATE and write result to $OUTPUT
sed -e "
s|::PACKAGEDATADIR::|${PACKAGEDATADIR}|
" ${TEMPLATE} > ${OUTPUT}
