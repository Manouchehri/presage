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

if [ $# -lt 2 ]
then
    echo "Usage: $0 presage_instdir presage_release"
    exit 1
fi

INSTDIR=$1
RELEASE=$2

MAKENSIS=makensis.exe
PYTHON=python

function set_up()
{
    SCRIPTDIR=`dirname $0`
    for i in $SCRIPTDIR/presage_installer.nsi \
             $SCRIPTDIR/StrRep.nsh \
             $SCRIPTDIR/ReplaceInFile.nsh
    do
        cp $i $INSTDIR
    done

    # generate NSIS include file
    echo "!define PRESAGE_VERSION \"${RELEASE}\"" > $INSTDIR/defines.nsh
    echo "!define PRESAGE_NAME \"presage\"" >> $INSTDIR/defines.nsh

    # license
    LICENSE="$SCRIPTDIR/../../COPYING"
    cp $LICENSE $INSTDIR/license.txt

    # py2exe
    SETUP_PY="$SCRIPTDIR/setup.py"
    cp $SETUP_PY $INSTDIR/
}

function clean_up()
{
    for i in presage_installer.nsi \
             StrRep.nsh \
             ReplaceInFile.nsh \
             defines.nsh \
             license.txt \
             setup.py
    do
        rm $INSTDIR/$i
    done
}

function build_installer()
{
    (cd $INSTDIR && $MAKENSIS presage_installer.nsi)
}

function build_py2exe()
{
    (cd $INSTDIR \
     && $PYTHON setup.py py2exe \
     && cp dist/* bin/)
}

function install_dependants()
{
    DEPENDANTS="libstdc++-6.dll libgcc_s_dw2-1.dll sqlite3.dll libcairo-2.dll libgdk-win32-2.0-0.dll libgdk_pixbuf-2.0-0.dll libglib-2.0-0.dll libgmodule-2.0-0.dll libgobject-2.0-0.dll libgtk-win32-2.0-0.dll libpango-1.0-0.dll libpangocairo-1.0-0.dll libfontconfig-1.dll libexpat-1.dll freetype6.dll libpng14-14.dll intl.dll libgio-2.0-0.dll libatk-1.0-0.dll libgthread-2.0-0.dll libpangoft2-1.0-0.dll libpangowin32-1.0-0.dll zlib1.dll"

    for i in $DEPENDANTS
    do
	SRC="`which $i`";
	DST="$INSTDIR/bin";
	echo "cp -f $SRC $DST";
	cp -f "$SRC" "$DST";
    done
}

####
# Mainline
#
set_up
install_dependants
#build_py2exe
build_installer
clean_up

