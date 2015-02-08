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

set -e 

if [ $# -lt 2 ]
then
    echo "Usage: $0 presage_install_dir presage_release"
    exit 1
fi

INSTALLDIR=$1
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
	if [ -f $INSTALLDIR/$i ]
	then
		rm $INSTALLDIR/$i
	fi
        cp $i $INSTALLDIR
    done

    # determine presage installation bitness
    BITNESS=""
    if [ -f $INSTALLDIR/bin/text2ngram.exe ]
    then
	if file $INSTALLDIR/bin/text2ngram.exe | grep "PE32+ executable"
	then
	    BITNESS="64"
        elif file $INSTALLDIR/bin/text2ngram.exe | grep "PE32 executable"
	then
	    BITNESS="32"
	fi
    fi

    # figure out Windows-style local installation path
    LOCAL_INSTALL_WIN_DIR=`(cd $INSTALLDIR; pwd -W)`
    LOCAL_INSTALL_WIN_DIR=`echo ${LOCAL_INSTALL_WIN_DIR} | sed 's/\//\\\\/g'`

    # generate NSIS include file
    echo "!define PRESAGE_VERSION \"${RELEASE}\"" > $INSTALLDIR/defines.nsh
    echo "!define PRESAGE_NAME \"presage\"" >> $INSTALLDIR/defines.nsh
    echo "!define BITNESS \"${BITNESS}\"" >> $INSTALLDIR/defines.nsh
    echo "!define LOCAL_INSTALL_WIN_DIR \"${LOCAL_INSTALL_WIN_DIR}\"" >> $INSTALLDIR/defines.nsh

    # license
    LICENSE="$SCRIPTDIR/../../COPYING"
    cp $LICENSE $INSTALLDIR/license.txt

    # py2exe
    SETUP_PY="$SCRIPTDIR/setup.py"
    cp $SETUP_PY $INSTALLDIR/
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
        rm $INSTALLDIR/$i
    done
}

function build_installer()
{
    (cd $INSTALLDIR && $MAKENSIS presage_installer.nsi)
}

function build_py2exe()
{
    (cd $INSTALLDIR \
     && $PYTHON setup.py py2exe \
     && cp dist/* bin/)
}

function install_dependants()
{
    if [ "x$BITNESS" == "x64" ]
    then
        MINGW_EXCEPT_LIB="libgcc_s_seh-1.dll"
    else
        MINGW_EXCEPT_LIB="libgcc_s_dw2-1.dll"
    fi
    MINGW_DEPS="libstdc++-6.dll ${MINGW_EXCEPT_LIB}"
    SQLITE_DEPS="libsqlite3-0.dll"
    if [ "x$BITNESS" == "x64" ]
    then
        GTK_FREETYPE_LIB="libfreetype-6.dll"
	GTK_INTL_LIB="libintl-8.dll"
    else
        GTK_FREETYPE_LIB="freetype6.dll"
	GTK_INTL_LIB="intl.dll"
    fi
    GTK_DEPS="libcairo-2.dll libgdk-win32-2.0-0.dll libgdk_pixbuf-2.0-0.dll libglib-2.0-0.dll libgmodule-2.0-0.dll libgobject-2.0-0.dll libgtk-win32-2.0-0.dll libpango-1.0-0.dll libpangocairo-1.0-0.dll libfontconfig-1.dll libexpat-1.dll libpng14-14.dll libgio-2.0-0.dll libatk-1.0-0.dll libgthread-2.0-0.dll libpangoft2-1.0-0.dll libpangowin32-1.0-0.dll zlib1.dll ${GTK_FREETYPE_LIB} ${GTK_INTL_LIB}"
    QT_DEPS="QtCore4.dll QtGui4.dll libwinpthread-1.dll libpng16-16.dll qscintilla2.dll"

    for i in $MINGW_DEPS $SQLITE_DEPS $GTK_DEPS $QT_DEPS
    do
	SRC="`which $i`";
	DST="$INSTALLDIR/bin";
	echo "cp $SRC $DST";
	if [ -f $DST/$i ]
	then
		rm $DST/$i
	fi
	cp "$SRC" "$DST";
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

