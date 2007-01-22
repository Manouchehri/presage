
/******************************************************************************\
 *                                                                            *
 * Soothsayer, an extensible predictive text entry system                     *
 * ------------------------------------------------------                     *
 *                                                                            *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>             *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*
 *                                                                            *
\******************************************************************************/        


#ifndef SOOTH_DATABASECONNECTORTEST
#define SOOTH_DATABASECONNECTORTEST

#include <cppunit/extensions/HelperMacros.h>

#include "tools/databaseConnector.h"

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
    void testInsertNgram();
    void testUpdateNgram();
    void testRemoveNgram();

    class DatabaseConnectorImpl : public DatabaseConnector {
    public:
	DatabaseConnectorImpl(std::string* query)
	    : m_query(query),
	      MAGIC_NUMBER(GLOBAL_MAGIC_NUMBER)
	    {}
	~DatabaseConnectorImpl() {}
	
	virtual void openDatabase() {}
	virtual void closeDatabase() {}
	/** Stores SQL query in m_query data member.
	 *
	 * This fakes an SQL query execution. Useful to test that the
	 * correct SQL query are generated.
	 */
	virtual std::string executeSql(const std::string query) const {
	    std::cout << "[query] " << query << std::endl;
	    *m_query = query;
	    std::stringstream ss;
	    ss << MAGIC_NUMBER;
	    return ss.str();
	}
	
    private:
	int MAGIC_NUMBER;
	std::string* const m_query;
    };

private:
    DatabaseConnector* databaseConnector;
    std::string* lastQuery;

    Ngram* unigram;
    Ngram* bigram;
    Ngram* trigram;

    CPPUNIT_TEST_SUITE( DatabaseConnectorTest );
//    CPPUNIT_TEST( testSanitizeString          );
    CPPUNIT_TEST( testCreateNgramTable        );
    CPPUNIT_TEST( testGetNgramCount           );
    CPPUNIT_TEST( testInsertNgram             );
    CPPUNIT_TEST( testUpdateNgram             );
    CPPUNIT_TEST( testRemoveNgram             );
    CPPUNIT_TEST_SUITE_END();
};

#endif // SOOTH_DATABASECONNECTORTEST
