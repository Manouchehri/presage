
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

#include "sqliteDatabaseConnector.h"

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

std::string SqliteDatabaseConnector::executeSql(const std::string query) const
{
    std::string answer;
    
    char* sql_error_message = 0;

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
    std::string& query_result = *static_cast<std::string*>(pArg);

    for (int i = 0; i < argc; i++) {
	query_result += argv[i];
	if (i < argc - 1) {
	    query_result += '\t';
	} else {
	    query_result += '\n';
	}
    }
}
