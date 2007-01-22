
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
 *                                                  c                          *
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


#include "databaseConnectorTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( DatabaseConnectorTest );

void DatabaseConnectorTest::setUp()
{
    lastQuery = new std::string;
    databaseConnector = new DatabaseConnectorImpl(lastQuery);

    unigram = new Ngram;
    unigram->push_back("foo");

    bigram = new Ngram;
    bigram->push_back("foo");
    bigram->push_back("bar");

    trigram = new Ngram;
    trigram->push_back("foo");
    trigram->push_back("bar");
    trigram->push_back("foobar");
}

void DatabaseConnectorTest::tearDown()
{
    delete databaseConnector;
    delete lastQuery;

    delete unigram;
    delete bigram;
    delete trigram;
}

void DatabaseConnectorTest::testCreateNgramTable()
{
    std::cout << "DatabaseConnectorTest::testCreateNgramTable()" << std::endl;

    for (int i = 1; i < 4; i++) {
	databaseConnector->createNgramTable(i);
    }
}

void DatabaseConnectorTest::testGetNgramCount()
{
    std::cout << "DatabaseConnectorTest::testGetNgramCount()" << std::endl;

    CPPUNIT_ASSERT_EQUAL( GLOBAL_MAGIC_NUMBER,
			  databaseConnector->getNgramCount(*unigram) );
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT count FROM _1_gram WHERE word = \"foo\";"),
			  *lastQuery );

    CPPUNIT_ASSERT_EQUAL( GLOBAL_MAGIC_NUMBER,
			  databaseConnector->getNgramCount(*bigram) );
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT count FROM _2_gram WHERE word_1 = \"bar\" AND word = \"foo\";"),
			  *lastQuery );


    CPPUNIT_ASSERT_EQUAL( GLOBAL_MAGIC_NUMBER,
			  databaseConnector->getNgramCount(*trigram) );
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT count FROM _3_gram WHERE word_2 = \"foobar\" AND word_1 = \"bar\" AND word = \"foo\";"),
			  *lastQuery );
}

void DatabaseConnectorTest::testInsertNgram()
{
    std::cout << "DatabaseConnectorTest::testInsertNgram()" << std::endl;

    databaseConnector->insertNgram(*unigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("INSERT INTO _1_gram VALUES(\"foo\", 1337);"),
			  *lastQuery );
    
    databaseConnector->insertNgram(*bigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("INSERT INTO _2_gram VALUES(\"bar\", \"foo\", 1337);"),
			  *lastQuery );

    databaseConnector->insertNgram(*trigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("INSERT INTO _3_gram VALUES(\"foobar\", \"bar\", \"foo\", 1337);"),
			  *lastQuery );
}

void DatabaseConnectorTest::testUpdateNgram()
{
    std::cout << "DatabaseConnectorTest::testUpdateNgram()" << std::endl;

    databaseConnector->updateNgram(*unigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("UPDATE _1_gram SET count = 1337 WHERE word = \"foo\";"),
			  *lastQuery );

    databaseConnector->updateNgram(*bigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("UPDATE _2_gram SET count = 1337 WHERE word_1 = \"bar\" AND word = \"foo\";"),
			  *lastQuery );
			  
    databaseConnector->updateNgram(*trigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("UPDATE _3_gram SET count = 1337 WHERE word_2 = \"foobar\" AND word_1 = \"bar\" AND word = \"foo\";"),
			  *lastQuery );
}

void DatabaseConnectorTest::testRemoveNgram()
{
    std::cout << "DatabaseConnectorTest::testRemoveNgram()" << std::endl;

    databaseConnector->removeNgram(*unigram);
    databaseConnector->removeNgram(*bigram);
    databaseConnector->removeNgram(*trigram);
}

//void DatabaseConnectorTest::testSanitizeString()
//{
//    // TODO
//}
