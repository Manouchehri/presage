#!/bin/bash
#
# Copyright (C) Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#

####
# Integration tests
#

TEXT2NGRAM=../../src/tools/text2ngram
SIMULATOR=../../src/soothsayerSimulator

TRAINING_CORPUS=${srcdir}/foo_corpus.txt
CONTROL_CORPUS=${srcdir}/foo_control.txt

CONFIG_TEMPLATE=${srcdir}/../../resources/profiles/soothsayer.xml.template
CONFIG=soothsayer.xml

NGRAM_CARDINALITY=5

DATABASE=database.db

ERROR_CODE=1

####
# Generate resources for statistical predictive plugins
#
# arg1 n-gram order of statistical resources
# arg2 database name
function generate_resources()
{
    if [ -z "$1" -o -z "$2" ]
    then
        echo "Error: generate_resources() requires two arguments"
	exit $ERROR_CODE
    fi

    i=1
    while [ "$i" -le "$1" ]
    do
      execute "$TEXT2NGRAM -o $DATABASE -n $i -f sqlite -l $2"
      i=`expr $i + 1`
    done
}

####
# Tweak configuration file
# 
function generate_config()
{
#    # use generate_soothsayer_config script from src/resources/profiles
#    ${srcdir}/../../resources/profiles/generate_soothsayer_config.sh ${srcdir}/../../resources/profiles/soothsayer.xml.template soothsayer.xml .
#

    cp -f ${CONFIG_TEMPLATE} ${CONFIG}
    replace_config_entry PLUGINS SmoothedNgramPlugin
    replace_config_entry DBFILENAME ${DATABASE}
    NGRAM_DELTAS=""
    COUNTER=1
    DELTA=10
    while [ "$COUNTER" -le "$NGRAM_CARDINALITY" ]
    do
      NGRAM_DELTAS="${NGRAM_DELTAS} ${DELTA}"
      DELTA=`expr ${DELTA} \* 10`
      COUNTER=`expr $COUNTER + 1`
    done
    replace_config_entry DELTAS "${NGRAM_DELTAS}"
}

function replace_config_entry()
{
    sed -i.bak -e "s|<$1>.*</$1>|<$1>$2</$1>|" ${CONFIG}
}

####
# Restore configuration file
#
function restore_config()
{
#    rm -f ${CONFIG} ${CONFIG}.bak
    echo
}

####
# simulate
#
# arg1 control corpus file
#
function simulate()
{
    if [ -z "$1" ]
    then
	echo "Error: simulate requires an argument"
	exit $ERROR_CODE
    else
	execute "$SIMULATOR -i -q $CONTROL_CORPUS"
    fi
}

####
# execute()
# 
# arg1 command line argument to execute
#
function execute()
{
    if [ -z "$1" ]
    then
	echo "Error: invoked execute() with no argument."
	exit $ERROR_CODE
    else
	$1
	if [ $? -ne 0 ]
	    then
	    echo "Error executing command: $1"
	    exit $ERROR_CODE
	fi
    fi
}

####
# clean_up()
# 
# cleans up after itself
#
function clean_up()
{
    if [ -f $DATABASE ]
    then
	rm $DATABASE
    fi
}


####
# Mainline
#
clean_up
generate_config
generate_resources $NGRAM_CARDINALITY $TRAINING_CORPUS
simulate $CONTROL_CORPUS
restore_config
clean_up
