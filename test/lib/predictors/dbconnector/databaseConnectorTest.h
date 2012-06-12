
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


#ifndef PRESAGE_DATABASECONNECTORTEST
#define PRESAGE_DATABASECONNECTORTEST

#include <cppunit/extensions/HelperMacros.h>

#include "predictors/dbconnector/databaseConnector.h"

#include <iostream>
#include <sstream>

const int GLOBAL_MAGIC_NUMBER = 1337;
//DatabaseConnectorTest::DatabaseConnectorImpl::MAGIC_NUMBER = GLOBAL_MAGIC_NUMBER;

class DatabaseConnectorTest : public CppUnit::TestFixture { 
public:
    void setUp();
    void tearDown();

//    void testSanitizeString();
    void testCreateNgramTable();
    void testGetNgramCount();
    void testGetNgramLikeTable();
    void testGetNgramLikeTableLimit();
    void testInsertNgram();
    void testUpdateNgram();
    void testRemoveNgram();

    /** Mock DatabaseConnector implementation object.
     * 
     * The virtual executeSql() method stores the SQL query string
     * into a pointer to a string that is accessible to
     * DatabaseConnectorTest class. This string is compared against a
     * benchmarked SQL string.
     *
     * The same magic number is also always returned as a count.
     *
     */
    class DatabaseConnectorImpl : public DatabaseConnector {
    public:
	DatabaseConnectorImpl(std::string* query)
	    : DatabaseConnector("", 0, false),
	    m_query(query),
	    MAGIC_NUMBER(GLOBAL_MAGIC_NUMBER)
	    {}
	virtual ~DatabaseConnectorImpl() {}
	
	virtual void openDatabase() {}
	virtual void closeDatabase() {}
	/** Stores SQL query in m_query data member.
	 *
	 * This fakes an SQL query execution. Useful to test that the
	 * correct SQL query are generated.
	 */
	virtual NgramTable executeSql(const std::string query) const {
	    //std::cout << "[query] " << query << std::endl;
	    *m_query = query;
	    std::stringstream ss;
	    ss << MAGIC_NUMBER;
	    Ngram ngram;
	    ngram.push_back(ss.str());
	    NgramTable ngramTable;
	    ngramTable.push_back(ngram);
	    return ngramTable;
	}
	
    private:
	std::string* const m_query;
	int          MAGIC_NUMBER;
    };

    /** Mock DatabaseConnector implementation object for getNgramCountLike().
     * 
     * The virtual executeSql() method stores the SQL query string
     * into a pointer to a string that is accessible to
     * DatabaseConnectorTest class. This string is compared against a
     * benchmarked SQL string.
     *
     * An mock ngram is returned.
     *
     */
    class DatabaseConnectorLikeImpl : public DatabaseConnectorImpl {
    public:
	DatabaseConnectorLikeImpl(std::string* query)
	    : DatabaseConnectorImpl(query)
	    {}
	~DatabaseConnectorLikeImpl() {}
	
	virtual void openDatabase() {}
	virtual void closeDatabase() {}
	/** Stores SQL query in m_query data member.
	 *
	 * This fakes an SQL query execution. Useful to test that the
	 * correct SQL query are generated.
	 */
	virtual NgramTable executeSql(const std::string query) const {
	    DatabaseConnectorImpl::executeSql(query);
	    NgramTable ngramTable;

	    Ngram* ngram = new Ngram();
	    ngram->push_back("foo");
	    ngram->push_back("bar");
	    ngram->push_back("foobar");
	    ngram->push_back("3");
	    ngramTable.push_back(*ngram);
	    delete ngram;
	    
	    ngram = new Ngram();
	    ngram->push_back("bar");
	    ngram->push_back("foo");
	    ngram->push_back("foobar");
	    ngram->push_back("33");
	    ngramTable.push_back(*ngram);
	    delete ngram;

	    ngram = new Ngram();
	    ngram->push_back("foobar");
	    ngram->push_back("bar");
	    ngram->push_back("foo");
	    ngram->push_back("333");
	    ngramTable.push_back(*ngram);
	    delete ngram;

	    return ngramTable;
	}
	
    private:

    };

private:
    void assertCorrectMockNgramTable(NgramTable ngramcount);

    DatabaseConnector* databaseConnector;
    std::string* lastQuery;

    Ngram* unigram;
    Ngram* bigram;
    Ngram* trigram;

    CPPUNIT_TEST_SUITE( DatabaseConnectorTest );
//    CPPUNIT_TEST( testSanitizeString          );
    CPPUNIT_TEST( testCreateNgramTable        );
    CPPUNIT_TEST( testGetNgramCount           );
    CPPUNIT_TEST( testGetNgramLikeTable       );
    CPPUNIT_TEST( testGetNgramLikeTableLimit  );
    CPPUNIT_TEST( testInsertNgram             );
    CPPUNIT_TEST( testUpdateNgram             );
    CPPUNIT_TEST( testRemoveNgram             );
    CPPUNIT_TEST_SUITE_END();
};

#endif // PRESAGE_DATABASECONNECTORTEST
