
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


#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../../core/logger.h"

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
    DatabaseConnector(const std::string database_name,
		      const size_t cardinality,
		      const bool read_write);
    DatabaseConnector(const std::string database_name,
		      const size_t cardinality,
		      const bool read_write,
		      const std::string& log_level);
    virtual ~DatabaseConnector();

    /** Creates a table to store N-grams.
     */
    void createNgramTable(const size_t cardinality) const;
    void createUnigramTable() const { createNgramTable(1); }
    void createBigramTable()  const { createNgramTable(2); }
    void createTrigramTable() const { createNgramTable(3); }

    /** Returns an integer equal to the sum of the counts of all unigrams.
     */
    int getUnigramCountsSum() const;

    /** Returns an integer equal to the specified ngram count.
     */
    int getNgramCount(const Ngram ngram) const;

    /** Returns a table of ngrams matching the specified ngram-like query.
     */
    NgramTable getNgramLikeTable(const Ngram ngram, int limit = -1) const;

    /** Returns a table of ngrams matching the specified ngram-like query and satisfying the given filter.
     */
    NgramTable getNgramLikeTableFiltered(const Ngram ngram, const char** filter, int limit = -1) const;

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

    /** Marks the beginning of an SQL transaction.
     *
     */
    virtual void beginTransaction() const;

    /** Marks the end of an SQL transaction.
     *
     */
    virtual void endTransaction() const;

    /** Rolls back an SQL transaction.
     *
     */
    virtual void rollbackTransaction() const;

protected:
    // Following functions to be overridden by derived classes.
    virtual void openDatabase()                                  = 0;
    virtual void closeDatabase()                                 = 0;
    virtual NgramTable executeSql(const std::string query) const = 0;

    std::string get_database_filename () const;
    std::string set_database_filename (const std::string& filename);

    void   set_cardinality (const size_t cardinality);
    size_t get_cardinality () const;

    void set_read_write_mode (const bool read_write);
    bool get_read_write_mode () const;

    Logger<char> logger;

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

    /** Same as buildWhereLikeClause but considers also a filter on the subsequent letter
     */
    std::string buildWhereLikeClauseFiltered(const Ngram ngram,const char** filter) const;

    /** Returns a string containing an SQL VALUES clause built for the ngram.
     */
    std::string buildValuesClause(const Ngram ngram, const int count) const;

    /** Sanitizes ngram, guards against SQL code injection.
     */
    std::string sanitizeString(const std::string) const;

    /** Returns the first element of the ngramtable as an integer.
     */
    int extractFirstInteger(const NgramTable&) const;


    std::string expand_variables (std::string filename) const;
    void substitute_variable_in_string (const std::string& variable_name, std::string& filepath) const;

    std::string database_filename;
    size_t cardinality;
    bool read_write_mode;

};

#endif // DATABASECONNECTOR_H
