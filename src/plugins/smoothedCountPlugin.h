
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>            *
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


#ifndef SOOTH_SMOOTHEDCOUNTPLUGIN
#define SOOTH_SMOOTHEDCOUNTPLUGIN

#include "plugins/plugin.h"

#include <sqlite.h>
#include <assert.h>
#include <stdlib.h>    // double atof( const char* )
#include <string>
//#include <dlfcn.h>     // dlopen() and friends...

/** Smoothed count statistical plugin
 *
 */
class SmoothedCountPlugin : public Plugin {
public:
    SmoothedCountPlugin( HistoryTracker& );
    ~SmoothedCountPlugin();

    virtual Prediction predict() const;

    virtual void learn();
    virtual void extract();
    virtual void train();

private:
    std::string strtolower( const std::string& ) const;

    sqlite* db;

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
extern "C" Plugin* create( HistoryTracker& );
extern "C" void destroy( Plugin* );




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
