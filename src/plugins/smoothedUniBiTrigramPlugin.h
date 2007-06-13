
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>           *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either version 2            *
 * of the License, or (at your option) any later version.                    *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software               *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.*
 *                                                                           *
\*****************************************************************************/


#ifndef SOOTH_SMOOTHEDUNIBITRIGRAMPLUGIN
#define SOOTH_SMOOTHEDUNIBITRIGRAMPLUGIN

#include "plugins/plugin.h"
#include "core/utility.h"    // strtolower

#include <string>

#include <sqlite.h>
#include <assert.h>
#include <stdlib.h>    // double atof( const char* )
//#include <dlfcn.h>     // dlopen() and friends...
//#include <ltdl.h>
#include <math.h>      // log()

#include "dirs.h"

#include "tools/sqliteDatabaseConnector.h"


static const char DEFAULT_DATABASE_LOCATION[] = localstatedir;
static const char DEFAULT_DATABASE_FILENAME[] = "database_en.db";

/** Smoothed unigram/bigram/trigram statistical plugin.
 *
 */
class SmoothedUniBiTrigramPlugin : public Plugin {
public:
    SmoothedUniBiTrigramPlugin(HistoryTracker&, Profile*);
    ~SmoothedUniBiTrigramPlugin();

    virtual Prediction predict() const;

    virtual void learn();
    virtual void extract();
    virtual void train();

private:
    DatabaseConnector* db;
    //SqliteDatabaseConnector* db;
    std::string database;

    double      UNIGRAM_WEIGHT;
    double      BIGRAM_WEIGHT;
    double      TRIGRAM_WEIGHT;
    std::string DBFILENAME;
    int         MAX_PARTIAL_PREDICTION_SIZE;

    // sqlite API is an external library. Plugins are compiled into
    // position independent code, so references to external symbols are not
    // resolved at compilation time. Symbols need to be dinamically loaded
    // at execution time. In other words, a cascaded dynamic loading takes
    // place.
    // These declarations provide data members necessary to cascaded dl.
//	lt_dlhandle libsqlite;
//
//	typedef sqlite* sqlite_open_t( const char*,
//				       int,
//				       char** );
//	typedef int sqlite_exec_t( sqlite*,
//				   const char*,
//				   sqlite_callback, 
//				   void*,
//				   char** );
//	typedef void sqlite_close_t( sqlite* );
//
//	sqlite_open_t* sqlite_open_handle;
//	sqlite_exec_t* sqlite_exec_handle;
//	sqlite_close_t* sqlite_close_handle;
    // end cascaded dynamic loading data members.

};


// Class factory functions
extern "C" SmoothedUniBiTrigramPlugin* create (HistoryTracker&, Profile*);
extern "C" void                        destroy(SmoothedUniBiTrigramPlugin*);


#endif // SOOTH_SMOOTHEDUNIBITRIGRAMPLUGIN
