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


####
# Integration tests
#

TEXT2NGRAM=../../src/tools/text2ngram
SIMULATOR=../../src/tools/presage_simulator

TRAINING_CORPUS=${srcdir}/../../COPYING
CONTROL_CORPUS=`basename ${TRAINING_CORPUS}_excerpt.txt`

CONFIG_TEMPLATE=${srcdir}/../../resources/profiles/presage.xml.template
CONFIG=presage.xml

NGRAM_CARDINALITY=3


DATABASE=database.db

ERROR_CODE=1

####
# Generate resources for statistical predictive plugins
#
# arg1 n-gram order of statistical resources
# arg2 database name
generate_resources()
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
generate_config()
{
#    # use generate_presage_config script from src/resources/profiles
#    ${srcdir}/../../resources/profiles/generate_presage_config.sh ${srcdir}/../../resources/profiles/presage.xml.template presage.xml .
#

    cp -f ${CONFIG_TEMPLATE} ${CONFIG}
    replace_config_entry PREDICTORS DefaultSmoothedNgramPredictor
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

replace_config_entry()
{
    rm -f ${CONFIG}.tmp
    sed -e "s|<$1>.*</$1>|<$1>$2</$1>|" ${CONFIG} > ${CONFIG}.tmp
    mv -f ${CONFIG}.tmp ${CONFIG}
}

####
# Restore configuration file
#
restore_config()
{
    rm -f ${CONFIG} ${CONFIG}.bak
}

####
# Generate control corpus
#
generate_control_corpus()
{
    # just extract some text from the training
    # corpus for the time being      # REVISIT
    cat $TRAINING_CORPUS | head -n 20 | tail -n 10 > $CONTROL_CORPUS
}

####
# simulate
#
# arg1 control corpus file
#
simulate()
{
    if [ -z "$1" ]
    then
	echo "Error: simulate requires an argument"
	exit $ERROR_CODE
    else
	execute "$SIMULATOR -i -q -c $CONFIG $CONTROL_CORPUS"
    fi
}

####
# execute()
# 
# arg1 command line argument to execute
#
execute()
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
clean_up()
{
    if [ -f "$DATABASE" ]
    then
	rm $DATABASE
    fi

    if [ -f "$CONTROL_CORPUS" ]
    then
        rm $CONTROL_CORPUS
    fi
}


####
# Mainline
#
clean_up
generate_config
generate_resources $NGRAM_CARDINALITY $TRAINING_CORPUS
generate_control_corpus
simulate $CONTROL_CORPUS
restore_config
clean_up
