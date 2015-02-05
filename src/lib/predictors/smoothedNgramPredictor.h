
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


#ifndef PRESAGE_SMOOTHEDNGRAMPREDICTOR
#define PRESAGE_SMOOTHEDNGRAMPREDICTOR

#include "predictor.h"
#include "../core/logger.h"
#include "../core/dispatcher.h"
#include "dbconnector/sqliteDatabaseConnector.h"

#include <assert.h>

#if defined(HAVE_SQLITE3_H) 
# include <sqlite3.h>
#elif defined(HAVE_SQLITE_H)
# include <sqlite.h>
#else
# error "SQLite is required. Please install SQLite."
#endif


/** Smoothed n-gram statistical predictor.
 *
 */
class SmoothedNgramPredictor : public Predictor, public Observer {
public:
    SmoothedNgramPredictor(Configuration*, ContextTracker*, const char*);
    ~SmoothedNgramPredictor();

    virtual Prediction predict(const size_t size, const char** filter) const;

    virtual void learn(const std::vector<std::string>& change);

    virtual void update (const Observable* variable);

private:
    std::string LOGGER;
    std::string DBFILENAME;
    std::string DELTAS;
    std::string LEARN;
    std::string DATABASE_LOGGER;

    unsigned int count(const std::vector<std::string>& tokens, int offset, int ngram_size) const;
    void check_learn_consistency(const Ngram& name) const;

    void set_dbfilename (const std::string& filename);
    void set_deltas (const std::string& deltas);
    void set_database_logger_level (const std::string& level);
    void set_learn (const std::string& learn_mode);

    void init_database_connector_if_ready ();

    DatabaseConnector*  db;
    std::string         dbfilename;
    std::string         dbloglevel;
    std::vector<double> deltas;
    size_t              cardinality; // cardinality == what is the n in n-gram?
    bool                learn_mode;
    bool                learn_mode_set;

    Dispatcher<SmoothedNgramPredictor> dispatcher;
};

#endif // PRESAGE_SMOOTHEDNGRAMPREDICTOR
