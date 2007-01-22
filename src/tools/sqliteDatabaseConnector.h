
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

#ifndef SQLITEDATABASECONNECTOR_H
#define SQLITEDATABASECONNECTOR_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_SQLITE_H
# include <sqlite.h>
#else
# error "SQLite is required. Please install SQLite."
#endif

#include "databaseConnector.h"

class SqliteDatabaseConnector : public DatabaseConnector {
  public:
    SqliteDatabaseConnector(const std::string db);
    ~SqliteDatabaseConnector();

    virtual void openDatabase();
    virtual void closeDatabase();
    virtual std::string executeSql(const std::string query) const;

  protected:

  private:
    static int callback(void *pArg, int argc, char **argv, char **columnNames);

    std::string db_name;
    sqlite* db;

};

#endif // SQLITEDATABASECONNECTOR_H
