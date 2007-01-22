
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

#include <iostream>
#include <sstream>
#include <stdlib.h>

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

    std::string result = executeSql(query.str());
    int count = atoi(result.c_str());

    std::cout << "[DatabaseConnector] result: " << result
	      << "count: " << count << std::endl;

    return (count > 0 ? count : 0);
}

int DatabaseConnector::incrementNgramCount(const Ngram ngram) const
{
    int count = getNgramCount(ngram);

    if (count > 0) {
	// the ngram was found in the database
	updateNgram(ngram, count + 1);
	
	std::cout << "Updated ngram to " << count + 1 << std::endl;

    } else {
	// the ngram was not found in the database
	insertNgram(ngram, 1);

	std::cout << "Inserted ngram" << std::endl;

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
    for (int i = ngram.size() - 1; i >= 0; i--) {
	if (i != 0) {
	    where_clause << " word_" << i << " = \""
			 << sanitizeString(ngram[i]) << "\" AND";
	} else {
	    where_clause << " word = \"" << sanitizeString(ngram[0]) << "\"";
	}
    }
    return where_clause.str();
}

std::string DatabaseConnector::buildValuesClause(const Ngram ngram, const int count) const
{
    std::stringstream values_clause;
    values_clause << "VALUES(";
    for (int i = ngram.size() - 1; i >= 0; i--) {
	if (i != 0) {
	    values_clause << "\"" << sanitizeString(ngram[i]) << "\", ";
	} else {
	    values_clause << "\"" << sanitizeString(ngram[0]) << "\", " << count << ")";
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
