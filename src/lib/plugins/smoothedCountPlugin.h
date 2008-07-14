
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
                                                                             *
                                                                **********(*)*/


#ifndef SOOTH_SMOOTHEDCOUNTPLUGIN
#define SOOTH_SMOOTHEDCOUNTPLUGIN

#include "plugins/plugin.h"

#include <assert.h>
#include <stdlib.h>    // double atof( const char* )

#if defined(HAVE_SQLITE3_H) 
# include <sqlite3.h>
#elif defined(HAVE_SQLITE_H)
# include <sqlite.h>
#else
# error "SQLite is required. Please install SQLite."
#endif

/** Smoothed count statistical plugin
 *
 */
class SmoothedCountPlugin : public Plugin {
public:
    SmoothedCountPlugin(Configuration*, ContextTracker*);
    ~SmoothedCountPlugin();

    virtual Prediction predict(const size_t) const;

    virtual void learn();
    virtual void extract();
    virtual void train();

private:
    std::string strtolower( const std::string& ) const;

#if defined(HAVE_SQLITE_H)
    sqlite* db;
#elif defined(HAVE_SQLITE3_H)
    sqlite3* db;
#endif

    double      UNIGRAM_WEIGHT;
    double      BIGRAM_WEIGHT;
    double      TRIGRAM_WEIGHT;
    std::string DBFILENAME;
    int         MAX_PARTIAL_PREDICTION_SIZE;

};


/** Structure used to passed all necessary data through to callback function buildPrediction.
 *
 * This structure is meant to be passed as fourth argument of function
 * sqlite_exec. This argument is passed straight to function register
 * as callback function by sqlite_exec. The fourth argument has to be
 * a pointer to void. Function buildPrediction receives this void
 * pointer as its first argument. It is cast back to CallBack* before
 * each data member is used.
 *
 * predPtr is a pointer to a Prediction object used to hold partial prediction results.
 * predSize is an integer used to indicate the maximum amount of suggestions to retrieve.
 *
 */
struct CallbackData {
    Prediction* predPtr;
    int predSize;
};

/** Sqlite callback function that builds a prediction objects from the results returned by sqlite_exec call.
 *
 */
int buildPrediction( void*, int, char**, char** );


#endif // SOOTH_SMOOTHEDCOUNTPLUGIN
