
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

#include "../../core/utility.h"

#include <list>
#include <sstream>
#include <stdlib.h>
#include <assert.h>

DatabaseConnector::DatabaseConnector(const std::string database_name,
				     const size_t cardinality,
				     const bool read_write)
    : logger("DatabaseConnector", std::cerr)
{
    set_database_filename (database_name);
    set_cardinality (cardinality);
    set_read_write_mode (read_write);
}

DatabaseConnector::DatabaseConnector(const std::string database_name,
				     const size_t cardinality,
				     const bool read_write,
				     const std::string& log_level)
    : logger("DatabaseConnector", std::cerr, log_level)
{
    set_database_filename (database_name);
    set_cardinality (cardinality);
    set_read_write_mode (read_write);
}

DatabaseConnector::~DatabaseConnector()
{}

void DatabaseConnector::createNgramTable(const size_t n) const
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
//                for(size_t j=0; j < filter.size()-1; j++)
                    if (j) {
                        where_clause << " OR ";
                    }
                    where_clause << " word LIKE '" << true_prefix << filter[j] << "%'";
                }
//                where_clause << " word LIKE '" << true_prefix <<"%' )";
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

void DatabaseConnector::rollbackTransaction() const
{
    executeSql("ROLLBACK TRANSACTION;");
}

std::string DatabaseConnector::get_database_filename () const
{
    return database_filename;
}

std::string DatabaseConnector::set_database_filename (const std::string& filename)
{
    std::string prev_filename = database_filename;

    database_filename = expand_variables (filename);

    // make an attempt at determining whether directory where language
    // model database is located exists and try to create it if it
    // does not... only cater for one directory level to create it.
    //
    std::string dir = Utility::dirname (database_filename);
    if (! dir.empty()) {
	// check that specified directory exists and accessible
	if (! Utility::is_directory_usable (dir)) {
	    // create it if not
	    Utility::create_directory (dir);
	}
    }

    return prev_filename;
}

std::string DatabaseConnector::expand_variables (std::string filepath) const
{
    // scan the filepath for variables, which follow the same pattern
    // as shell variables - strings enclosed in '${' and '}'
    //
    const std::string start_marker = "${";
    const std::string   end_marker = "}";

    std::list<std::string> variables;

    std::string::size_type pos_start = filepath.find (start_marker);
    while (pos_start != std::string::npos)
    {
	std::string::size_type pos_end = filepath.find (end_marker, pos_start);
	if (pos_end != std::string::npos) {
	    variables.push_back (filepath.substr(pos_start + start_marker.size(), pos_end - end_marker.size() - pos_start - 1));
	}

	pos_start = filepath.find (start_marker, pos_end);
    }

    for (std::list<std::string>::const_iterator it = variables.begin();
	 it != variables.end();
	 it++)
    {
	substitute_variable_in_string(*it, filepath);
    }

    return filepath;
}

void DatabaseConnector::substitute_variable_in_string (const std::string& variable_name, std::string& filepath) const
{
    std::string variable_token = "${" + variable_name + "}";

    for (std::string::size_type pos = filepath.find (variable_token);
         pos != std::string::npos;
         pos = filepath.find (variable_token, pos))
    {
        const char* value = getenv(variable_name.c_str());
        if (value)
        {
            filepath.replace (pos,
                              variable_token.size(),
                              value);
        }
        else
        {
	    // handle "special" variables
	    if (variable_name == "HOME")
	    {
		value = getenv("USERPROFILE");
		if (value)
		{
		    filepath.replace (pos,
				      variable_token.size(),
				      value);
		}
	    }
	    else
	    {
		// FIXME: maybe throw exception instead of leaving
		// variable name in string?
		//
		filepath.replace (pos,
				  variable_token.size(),
				  variable_name);
	    }
	}
    }
}

void DatabaseConnector::set_cardinality (const size_t card)
{
    cardinality = card;
}

size_t DatabaseConnector::get_cardinality () const
{
    return cardinality;
}

void DatabaseConnector::set_read_write_mode (const bool read_write)
{
    read_write_mode = read_write;
}

bool DatabaseConnector::get_read_write_mode () const
{
    return read_write_mode;
}
