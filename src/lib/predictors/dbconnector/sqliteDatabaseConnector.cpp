
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


#include "sqliteDatabaseConnector.h"

#ifdef HAVE_STDLIB_H
# include <stdlib.h> // for free()
#endif

SqliteDatabaseConnector::SqliteDatabaseConnector(const std::string database_name,
						 const size_t cardinality,
						 const bool read_write)
  : DatabaseConnector(database_name, cardinality, read_write)
{
    openDatabase();
}

SqliteDatabaseConnector::SqliteDatabaseConnector(const std::string database_name,
						 const size_t cardinality,
						 const bool read_write,
						 const std::string logger_level)
  : DatabaseConnector(database_name, cardinality, read_write, logger_level)
{
    openDatabase();
}

SqliteDatabaseConnector::~SqliteDatabaseConnector()
{
    closeDatabase();
}

void SqliteDatabaseConnector::openDatabase()
{
#if defined(HAVE_SQLITE3_H)
    int rc;

    if (get_read_write_mode())
    {
	// attempt to open read-write, no create
	rc = sqlite3_open_v2(get_database_filename().c_str(),
			     &db,
			     SQLITE_OPEN_READWRITE,
			     NULL);

	if (rc != SQLITE_OK) {
	    // attempt to open read-write, create
	    rc = sqlite3_open_v2(get_database_filename().c_str(),
				 &db,
				 SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
				 NULL);

	    logger << WARN << "Created new language model database: " << get_database_filename() << endl;
	}

	if (rc == SQLITE_OK) {
	    // create n-gram tables up to specified cardinality if they
	    // don't yet exist
	    for (size_t cardinality = 1;
		 cardinality <= get_cardinality ();
		 cardinality++)
	    {
		createNgramTable(cardinality);
	    }
	}
    }
    else
    {
	// open read-only, no create
	rc = sqlite3_open_v2(get_database_filename().c_str(),
			     &db,
			     SQLITE_OPEN_READONLY,
			     NULL);
    }

    // throw exception if database cannot be opened/created
    if (rc != SQLITE_OK) {
	std::string error = sqlite3_errmsg(db);
	logger << ERROR << "Unable to create/open database: " << get_database_filename() << endl;
	throw SqliteDatabaseConnectorException(PRESAGE_SQLITE_OPEN_DATABASE_ERROR, error);
    }


#elif defined(HAVE_SQLITE_H)
    char* errormsg = 0;
    db = sqlite_open(get_database_filename().c_str(), 0, &errormsg);
    if (db == 0) {
	std::string error;
	if (errormsg != 0) {
	    error = errormsg;
	}
#ifdef HAVE_STDLIB_H
        free(errormsg);
#endif
	logger << ERROR << "Unable to open database: " << get_database_filename() << " : " << endl;
	throw SqliteDatabaseConnectorException(error);
    }
#endif

}

void SqliteDatabaseConnector::closeDatabase()
{
    if (db) {
#if defined(HAVE_SQLITE3_H)
	sqlite3_close(db);
#elif defined(HAVE_SQLITE_H)
	sqlite_close(db);
#endif
    }
}

NgramTable SqliteDatabaseConnector::executeSql(const std::string query) const
{
    NgramTable answer;
    
    char* sqlite_error_msg = 0;

    logger << DEBUG << "executing query: " << query << endl;
#if defined(HAVE_SQLITE3_H)
    int result = sqlite3_exec(
#elif defined(HAVE_SQLITE_H)
    int result = sqlite_exec(
#endif
	db,
	query.c_str(),
	callback,
	&answer,
	&sqlite_error_msg
    );

    if (result != SQLITE_OK) {
	std::string error;
	if (sqlite_error_msg != 0) {
	    error = sqlite_error_msg;
	}
#if defined(HAVE_SQLITE3_H)
        sqlite3_free(sqlite_error_msg);
#elif defined(HAVE_SQLITE_H)
# ifdef HAVE_STDLIB_H
        free(sqlite_error_msg);
# endif
#endif
	logger << ERROR << "Error executing SQL: '" 
	       << query << "' on database: '" << get_database_filename()
	       << "' : " << error << endl;
	throw SqliteDatabaseConnectorException(PRESAGE_SQLITE_EXECUTE_SQL_ERROR, error);
    }

    return answer;
}

int SqliteDatabaseConnector::callback(
    void *pArg,
    int argc,
    char **argv,
    char **columnNames)
{
    NgramTable& query_result = *static_cast<NgramTable*>(pArg);

    //std::cerr << "building ngram: ";
    Ngram ngram;
    for (int i = 0; i < argc; i++) {
	if (argv[i] != NULL) {
	    //std::cerr << "(" << columnNames[i] << ":" << argv[i] << ")" << '\t';
	    ngram.push_back(argv[i]);
	}
	else
	{
	    //std::cerr << "argv[" << i << "] is NULL" << std::endl;
	    ngram.push_back(""); // empty string to represent NULL value
	}
    }
    //std::cerr << std::endl;

    query_result.push_back(ngram);

    return SQLITE_OK;
}
