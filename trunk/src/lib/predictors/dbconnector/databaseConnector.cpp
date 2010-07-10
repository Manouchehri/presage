
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


#include "databaseConnector.h"

#include <sstream>
#include <stdlib.h>
#include <assert.h>

DatabaseConnector::DatabaseConnector()
    : logger("DatabaseConnector", std::cerr)
{}

DatabaseConnector::DatabaseConnector(const std::string& log_level)
    : logger("DatabaseConnector", std::cerr, log_level)
{}

DatabaseConnector::~DatabaseConnector()
{}

void DatabaseConnector::createNgramTable(const int n) const
{
    if (n > 0) {
	std::stringstream query;
	std::stringstream unique;
	query << "CREATE TABLE";
// This #ifdef does not belong here, but unfortunately SQLite 2.x does
// not support the IF NOT EXISTS SQL clause.
#ifndef HAVE_SQLITE_H
	query << " IF NOT EXISTS";
#endif
	query << " _" << n << "_gram (";
	for (int i = n - 1; i >= 0; i--) {
	    if (i != 0) {
		unique << "word_" << i << ", ";
		query << "word_" << i << " TEXT, ";
	    } else {
		unique << "word";
		query << "word TEXT, count INTEGER, UNIQUE(" << unique.str() << ") );";
	    }
	}

	executeSql(query.str());
    } else {
	// TODO
	// throw exception
    }
}

int DatabaseConnector::getUnigramCountsSum() const
{
    std::string query = "SELECT SUM(count) FROM _1_gram;";

    NgramTable result = executeSql(query);

    logger << DEBUG << "NgramTable:";
    for (size_t i = 0; i < result.size(); i++) {
	for (size_t j = 0; j < result[i].size(); j++) {
	    logger << DEBUG << result[i][j] << '\t';
	}
    logger << DEBUG << endl;
    }

    return extractFirstInteger(result);
}

int DatabaseConnector::getNgramCount(const Ngram ngram) const
{
    std::stringstream query;
    query << "SELECT count "
	  << "FROM _" << ngram.size() << "_gram"
	  << buildWhereClause(ngram) << ";";

    NgramTable result = executeSql(query.str());

    logger << DEBUG << "NgramTable:";
    for (size_t i = 0; i < result.size(); i++) {
	for (size_t j = 0; j < result[i].size(); j++) {
	    logger << DEBUG << result[i][j] << '\t';
	}
	logger << DEBUG << endl;
    }

    return extractFirstInteger(result);
}

NgramTable DatabaseConnector::getNgramLikeTable(const Ngram ngram, int limit) const
{
    std::stringstream query;
    query << "SELECT " << buildSelectLikeClause(ngram.size()) << " "
	  << "FROM _" << ngram.size() << "_gram"
	  << buildWhereLikeClause(ngram)
	  << " ORDER BY count DESC";
    if (limit < 0) {
	query << ";";
    } else {
	query << " LIMIT " << limit << ';';
    }

    return executeSql(query.str());
}

NgramTable DatabaseConnector::getNgramLikeTableFiltered(const Ngram ngram, const char** filter, int limit) const
{
    std::stringstream query;
    query << "SELECT " << buildSelectLikeClause(ngram.size()) << " "
          << "FROM _" << ngram.size() << "_gram"
          << buildWhereLikeClauseFiltered(ngram,filter)
          << " ORDER BY count DESC";
    if (limit < 0) {
        query << ";";
    } else {
        query << " LIMIT " << limit << ';';
    }

    return executeSql(query.str());
}

int DatabaseConnector::incrementNgramCount(const Ngram ngram) const
{
    int count = getNgramCount(ngram);

    if (count > 0) {
	// the ngram was found in the database
	updateNgram(ngram, ++count);

	logger << DEBUG << "Updated ngram to " << count << endl;

    } else {
	// the ngram was not found in the database
        count = 1;
	insertNgram(ngram, count);

	logger << DEBUG << "Inserted ngram" << endl;

    }
    return count;
}

void DatabaseConnector::removeNgram(const Ngram ngram) const
{}

void DatabaseConnector::insertNgram(const Ngram ngram, const int count) const
{
    std::stringstream query;

    query << "INSERT INTO _" << ngram.size() << "_gram "
	  << buildValuesClause(ngram, count)
	  << ";";

    executeSql(query.str());
}

void DatabaseConnector::updateNgram(const Ngram ngram, const int count) const
{
    std::stringstream query;

    query << "UPDATE _" << ngram.size() << "_gram "
	  << "SET count = " << count
	  << buildWhereClause(ngram) << ";";

    executeSql(query.str());
}

std::string DatabaseConnector::buildWhereClause(const Ngram ngram) const
{
    std::stringstream where_clause;
    where_clause << " WHERE";
    for (size_t i = 0; i < ngram.size(); i++) {
	if (i < ngram.size() - 1) {
	    where_clause << " word_" << ngram.size() - i - 1 << " = '"
			 << sanitizeString(ngram[i]) << "' AND";
	} else {
	    where_clause << " word = '" << sanitizeString(ngram[ngram.size() - 1]) << "'";
	}
    }
    return where_clause.str();
}

// TODO REVISIT refactor: this is same as buildWhereClause, except for
//                        "word = " instead of "word LIKE "
std::string DatabaseConnector::buildWhereLikeClause(const Ngram ngram) const
{
    std::stringstream where_clause;
    where_clause << " WHERE";
    for (size_t i = 0; i < ngram.size(); i++) {
	if (i < ngram.size() - 1) {
	    where_clause << " word_" << ngram.size() - i - 1 << " = '"
			 << sanitizeString(ngram[i]) << "' AND";
	} else {
	    where_clause << " word LIKE '" << sanitizeString(ngram[ngram.size() - 1]) << "%'";
	}
    }
    return where_clause.str();
}

std::string DatabaseConnector::buildWhereLikeClauseFiltered(const Ngram ngram, const char** filter) const
{
    std::stringstream where_clause;
    where_clause << " WHERE";
    for (size_t i = 0; i < ngram.size(); i++) {
        if (i < ngram.size() - 1) {
            where_clause << " word_" << ngram.size() - i - 1 << " = '"
                         << sanitizeString(ngram[i]) << "' AND";
        } else {
	    if(filter == 0)
		where_clause << " word LIKE '" << sanitizeString(ngram[ngram.size() - 1]) << "%'";
	    else {
		std::string true_prefix = sanitizeString(ngram[ngram.size() - 1]);
		where_clause << " (";
		for (int j = 0; filter[j] != 0; j++) {
//		for(size_t j=0; j < filter.size()-1; j++)
		    if (j) {
			where_clause << " OR ";
		    }
		    where_clause << " word LIKE '" << true_prefix << filter[j] << "%'";
		}
//		where_clause << " word LIKE '" << true_prefix <<"%' )";
		where_clause << ')';
	    }
        }
    }
    return where_clause.str();
}


std::string DatabaseConnector::buildSelectLikeClause(const int cardinality) const
{
    assert(cardinality > 0);

    std::stringstream result;
    for (int i = cardinality - 1; i >= 0; i--) {
	if (i != 0) {
	    result << "word_" << i << ", ";
	} else {
	    result << "word, count";
	}
    }

    return result.str();
}

std::string DatabaseConnector::buildValuesClause(const Ngram ngram, const int count) const
{
    std::stringstream values_clause;
    values_clause << "VALUES(";
    for (size_t i = 0; i < ngram.size(); i++) {
	if (i < ngram.size() - 1) {
	    values_clause << "'" << sanitizeString(ngram[i]) << "', ";
	} else {
	    values_clause << "'" << sanitizeString(ngram[i]) << "', " << count << ")";
	}
    }
    return values_clause.str();
}

std::string DatabaseConnector::sanitizeString(const std::string str) const
{
    // TODO
    // just return the string for the time being
    // REVISIT
    // TO BE DONE
    // TBD
    return str;
}

int DatabaseConnector::extractFirstInteger(const NgramTable& table) const
{
    // Initialize count to zero and then check that we have at least
    // an entry in the table of ngram counts returned by the
    // executeSql() method. If so, convert it into an integer and
    // return it.
    //
    // REVISIT: make conversion to integer more robust (strtol ??)
    //
    int count = 0;
    if (table.size() > 0) {
	if (table[0].size() > 0) {
	    count = atoi(table[0][0].c_str());
	}
    }

    logger << DEBUG << "table: ";
    for (size_t i = 0; i < table.size(); i++) {
	for (size_t j = 0; j < table[i].size(); j++) {
	    logger << DEBUG << table[i][j] << '\t';
	}
	logger << DEBUG << endl;
    }

    return (count > 0 ? count : 0);
}

void DatabaseConnector::beginTransaction() const
{
    executeSql("BEGIN TRANSACTION;");
}

void DatabaseConnector::endTransaction() const
{
    executeSql("END TRANSACTION;");
}
