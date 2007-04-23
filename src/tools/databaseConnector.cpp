
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

#include "databaseConnector.h"

#include <sstream>
#include <stdlib.h>
#include <assert.h>

#ifdef DEBUG
# include <iostream>
# define LOG(x) std::cout << x << std::endl
#else
# define LOG(x) /* x */
#endif

DatabaseConnector::DatabaseConnector()
{}

DatabaseConnector::~DatabaseConnector()
{}

void DatabaseConnector::createNgramTable(const int n) const
{
    if (n > 0) {
	std::stringstream query;
	std::stringstream unique;
	query << "CREATE TABLE _" << n << "_gram (";
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

int DatabaseConnector::getNgramCount(const Ngram ngram) const
{
    std::stringstream query;
    query << "SELECT count "
	  << "FROM _" << ngram.size() << "_gram"
	  << buildWhereClause(ngram) << ";";

    NgramTable result = executeSql(query.str());

    LOG("[DatabaseConnector] NgramTable:");
    for (int i = 0; i < result.size(); i++) {
	for (int j = 0; j < result[i].size(); j++) {
	    LOG(result[i][j] << '\t');
	}
	LOG("");
    }

    // Initialize count to zero and then check that we have at least
    // an entry in the table of ngram counts returned by the
    // executeSql() method. If so, convert it into an integer and
    // return it.
    //
    // REVISIT: make conversion to integer more robust (strtol ??)
    //
    int count = 0;
    if (result.size() > 0) {
	if (result[0].size() > 0) {
	    count = atoi(result[0][0].c_str());
	}
    }

    LOG("[DatabaseConnector] result: ");
    for (int i = 0; i < result.size(); i++) {
	for (int j = 0; j < result[i].size(); j++) {
	    LOG(result[i][j] << '\t');
	}
	LOG("");
    }
	
    return (count > 0 ? count : 0);
}

NgramTable DatabaseConnector::getNgramLikeTable(const Ngram ngram) const
{
    std::stringstream query;
    query << "SELECT " << buildSelectLikeClause(ngram.size()) << " "
	  << "FROM _" << ngram.size() << "_gram"
	  << buildWhereLikeClause(ngram)
	  << " ORDER BY count DESC" << ";";

    return executeSql(query.str());
}

int DatabaseConnector::incrementNgramCount(const Ngram ngram) const
{
    int count = getNgramCount(ngram);

    if (count > 0) {
	// the ngram was found in the database
	updateNgram(ngram, count + 1);
	
	LOG("[DatabaseConnector] Updated ngram to " << count + 1);

    } else {
	// the ngram was not found in the database
	insertNgram(ngram, 1);

	LOG("[DatabaseConnector] Inserted ngram");

    }
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
    for (int i = 0; i < ngram.size(); i++) {
	if (i < ngram.size() - 1) {
	    where_clause << " word_" << ngram.size() - i - 1 << " = \""
			 << sanitizeString(ngram[i]) << "\" AND";
	} else {
	    where_clause << " word = \"" << sanitizeString(ngram[ngram.size() - 1]) << "\"";
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
    for (int i = 0; i < ngram.size(); i++) {
	if (i < ngram.size() - 1) {
	    where_clause << " word_" << ngram.size() - i - 1 << " = \""
			 << sanitizeString(ngram[i]) << "\" AND";
	} else {
	    where_clause << " word LIKE \"" << sanitizeString(ngram[ngram.size() - 1]) << "%\"";
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
    for (int i = 0; i < ngram.size(); i++) {
	if (i < ngram.size() - 1) {
	    values_clause << "\"" << sanitizeString(ngram[i]) << "\", ";
	} else {
	    values_clause << "\"" << sanitizeString(ngram[i]) << "\", " << count << ")";
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
