
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

SqliteDatabaseConnector::SqliteDatabaseConnector(const std::string database_name)
    : DatabaseConnector()
{
    db_name = database_name;
    openDatabase();
}

SqliteDatabaseConnector::SqliteDatabaseConnector(const std::string database_name, const std::string logger_level)
    : DatabaseConnector(logger_level)
{
    db_name = database_name;
    openDatabase();
}

SqliteDatabaseConnector::~SqliteDatabaseConnector()
{
    closeDatabase();
}

void SqliteDatabaseConnector::openDatabase()
{
#if defined(HAVE_SQLITE3_H)
    int result = sqlite3_open(db_name.c_str(), &db);
    if (result != SQLITE_OK) {
	std::string error = sqlite3_errmsg(db);
	logger << ERROR << "Unable to open database: " << db_name << " : " << endl;
	throw SqliteDatabaseConnectorException(error);
    }
#elif defined(HAVE_SQLITE_H)
    char* errormsg = 0;
    db = sqlite_open(db_name.c_str(), 0, &errormsg);
    if (db == 0) {
	std::string error;
	if (errormsg != 0) {
	    error = errormsg;
	}
#ifdef HAVE_STDLIB_H
        free(errormsg);
#endif
	logger << ERROR << "Unable to open database: " << db_name << " : " << endl;
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
	       << query << "' on database: '" << db_name
	       << "' : " << error << endl;
	throw SqliteDatabaseConnectorException(error);
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

    //logger << DEBUG << "building ngram: ";
    Ngram ngram;
    for (int i = 0; i < argc; i++) {
	ngram.push_back(argv[i]);
	//logger << DEBUG << "(" << columnNames[i] << ":" << argv[i] << ")" << '\t';
    }
    //logger << DEBUG << endl;

    query_result.push_back(ngram);

    return SQLITE_OK;
}
