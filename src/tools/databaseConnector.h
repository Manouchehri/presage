
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

#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <map>
#include <vector>
#include <string>

typedef std::vector<std::string> Ngram;
typedef std::vector<Ngram> NgramTable;

/** Provides the interface to database creation, updating and querying operations.
 *
 */
class DatabaseConnector {
public:
    DatabaseConnector();
    virtual ~DatabaseConnector();

    /** Creates a table to store N-grams.
     */
    void createNgramTable(const int n) const;
    void createUnigramTable() const { createNgramTable(1); }
    void createBigramTable()  const { createNgramTable(2); }
    void createTrigramTable() const { createNgramTable(3); }

    /** Returns an integer equal to the specified ngram count.
     */
    int getNgramCount(const Ngram ngram) const;

    /** Returns a table of ngrams matching the specified ngram-like query.
     */
    NgramTable getNgramLikeTable(const Ngram ngram) const;

    /** Increments the specified ngram count and returns the updated count.
     *
     * If the ngram does not yet exit in the database, it is created
     * and its count is set to one.
     */
    int incrementNgramCount(const Ngram ngram) const;

    /** Insert ngram into database and sets its count.
     */
    void insertNgram(const Ngram ngram, const int count) const;
    
    /** Updates ngram count.
     */
    void updateNgram(const Ngram ngram, const int count) const;

    /** Removes the ngram from the database
     */
    void removeNgram(const Ngram ngram) const;

protected:
    // Following functions to be overridden by derived classes.
    virtual void openDatabase()                                   = 0;
    virtual void closeDatabase()                                  = 0;
    virtual NgramTable executeSql(const std::string query) const = 0;

private:
    /** Returns a string containing the column specifiers for the SQL
     *  SELECT clause built for an ngram of specified cardinality.
     */
    std::string buildSelectLikeClause(const int cardinality) const;

    /** Returns a string containing an SQL WHERE clause built for the ngram.
     */
    std::string buildWhereClause(const Ngram ngram) const;

    /** Returns a string containing an SQL WHERE clause built for the ngram,
     *  where the last comparison is a LIKE clauses instead of = clause.
     */
    std::string buildWhereLikeClause(const Ngram ngram) const;

    /** Returns a string containing an SQL VALUES clause built for the ngram.
     */
    std::string buildValuesClause(const Ngram ngram, const int count) const;

    /** Sanitizes ngram, guards against SQL code injection.
     */
    std::string sanitizeString(const std::string) const;
};

#endif // DATABASECONNECTOR_H
