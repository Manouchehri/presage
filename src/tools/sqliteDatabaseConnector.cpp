
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>            *
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

#include "sqliteDatabaseConnector.h"

#ifdef DEBUG
# include <iostream>
# define LOG(x) std::cout << x << std::endl
#else
# define LOG(x) /* x */
#endif

SqliteDatabaseConnector::SqliteDatabaseConnector(const std::string database_name)
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
    db = sqlite_open(db_name.c_str(), 0, 0);
}

void SqliteDatabaseConnector::closeDatabase()
{
    if (db) {
	sqlite_close(db);
    }
}

NgramTable SqliteDatabaseConnector::executeSql(const std::string query) const
{
    NgramTable answer;
    
    char* sql_error_message = 0;

    LOG("[SqliteDatabaseConnector] executing query: " << query);
    int result = sqlite_exec(db,
			     query.c_str(),
			     callback,
			     &answer,
			     &sql_error_message);

    delete sql_error_message;

    return answer;
}

int SqliteDatabaseConnector::callback(
    void *pArg,
    int argc,
    char **argv,
    char **columnNames)
{
    NgramTable& query_result = *static_cast<NgramTable*>(pArg);

    LOG("[SqliteDatabaseConnector] building ngram: ");
    Ngram ngram;
    for (int i = 0; i < argc; i++) {
	ngram.push_back(argv[i]);
	LOG("(" << columnNames[i] << ":" << argv[i] << ")" << '\t');
    }
    LOG("");

    query_result.push_back(ngram);

    return SQLITE_OK;
}
