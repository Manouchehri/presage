
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


#ifndef SQLITEDATABASECONNECTOR_H
#define SQLITEDATABASECONNECTOR_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if defined(HAVE_SQLITE3_H) 
# include <sqlite3.h>
#elif defined(HAVE_SQLITE_H)
# include <sqlite.h>
#else
# error "SQLite is required. Please install SQLite."
#endif

#include "databaseConnector.h"
#include "../../presageException.h"

class SqliteDatabaseConnector : public DatabaseConnector {
  public:
    SqliteDatabaseConnector(const std::string db,
			    const size_t cardinality,
			    const bool read_write);
    SqliteDatabaseConnector(const std::string db,
			    const size_t cardinality,
			    const bool read_write,
			    const std::string logger_level);
    ~SqliteDatabaseConnector();

    virtual void openDatabase();
    virtual void closeDatabase();
    virtual NgramTable executeSql(const std::string query) const;

    class SqliteDatabaseConnectorException : public PresageException {
    public:
	SqliteDatabaseConnectorException(presage_error_code_t code, const std::string& errormsg) throw() : PresageException(code, errormsg) { }
	virtual ~SqliteDatabaseConnectorException() throw() { }

    private:
	SqliteDatabaseConnectorException() throw() : PresageException(PRESAGE_ERROR, "") {};

    };

  protected:

  private:
    static int callback(void *pArg, int argc, char **argv, char **columnNames);

#if defined(HAVE_SQLITE3_H)
    sqlite3* db;
#elif defined(HAVE_SQLITE_H)
    sqlite* db;
#endif

};

#endif // SQLITEDATABASECONNECTOR_H
