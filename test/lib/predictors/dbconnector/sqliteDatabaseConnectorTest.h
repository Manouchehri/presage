
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


#ifndef PRESAGE_SQLITEDATABASECONNECTORTEST
#define PRESAGE_SQLITEDATABASECONNECTORTEST

#include <cppunit/extensions/HelperMacros.h>

#include "predictors/dbconnector/sqliteDatabaseConnector.h"

#include <iostream>
#include <sstream>

static const char   DEFAULT_DATABASE_FILENAME[] = "database.db";
static const size_t DEFAULT_DATABASE_CARDINALITY = 3;
static const bool   DEFAULT_DATABASE_READ_WRITE_MODE = true;
static const char   DATABASE_DUMP[] = "database.dump";

class SqliteDatabaseConnectorTest : public CppUnit::TestFixture { 
public:
    void setUp();
    void tearDown();

    void testConstructor();
    void testInsertNgram();
    void testUpdateNgram();
    void testRemoveNgram();
    void testGetNgramCount();
    void testIncrementNgramCount();
    void testGetNgramLikeTable();

private:
    void assertExistsAndRemoveFile(const char* filename) const;
    void assertDatabaseDumpEqualsBenchmark(std::stringstream& benchmark) const;
    void assertEqualNgramTable(const NgramTable* const, const NgramTable&);

    void strip_char(char c, std::string& str) const;

    DatabaseConnector* sqliteDatabaseConnector;

    Ngram* unigram;
    Ngram* bigram;
    Ngram* trigram;

    Ngram* unigram1;
    Ngram* bigram1;
    Ngram* trigram1;

    static const int MAGIC_NUMBER;

    CPPUNIT_TEST_SUITE( SqliteDatabaseConnectorTest );
    CPPUNIT_TEST( testConstructor                   );
    CPPUNIT_TEST( testInsertNgram                   );
    CPPUNIT_TEST( testUpdateNgram                   );
    CPPUNIT_TEST( testRemoveNgram                   );
    CPPUNIT_TEST( testGetNgramCount                 );
    CPPUNIT_TEST( testIncrementNgramCount           );
    CPPUNIT_TEST( testGetNgramLikeTable             );
    CPPUNIT_TEST_SUITE_END();
};

const int SqliteDatabaseConnectorTest::MAGIC_NUMBER = 1337;

#endif // PRESAGE_SQLITEDATABASECONNECTORTEST
