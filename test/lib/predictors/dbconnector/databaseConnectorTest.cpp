
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
    for (int i = 1; i < 4; i++) {
	databaseConnector->createNgramTable(i);
    }
}

void DatabaseConnectorTest::testGetNgramCount()
{
    CPPUNIT_ASSERT_EQUAL( GLOBAL_MAGIC_NUMBER,
			  databaseConnector->getNgramCount(*unigram) );
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT count FROM _1_gram WHERE word = 'foo';"),
			  *lastQuery );

    CPPUNIT_ASSERT_EQUAL( GLOBAL_MAGIC_NUMBER,
			  databaseConnector->getNgramCount(*bigram) );
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT count FROM _2_gram WHERE word_1 = 'foo' AND word = 'bar';"),
			  *lastQuery );

    CPPUNIT_ASSERT_EQUAL( GLOBAL_MAGIC_NUMBER,
			  databaseConnector->getNgramCount(*trigram) );
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT count FROM _3_gram WHERE word_2 = 'foo' AND word_1 = 'bar' AND word = 'foobar';"),
			  *lastQuery );
}

void DatabaseConnectorTest::assertCorrectMockNgramTable(NgramTable ngramTable)
{
    CPPUNIT_ASSERT_EQUAL(size_t(3), ngramTable.size());

    CPPUNIT_ASSERT(ngramTable[0][0] == "foo");
    CPPUNIT_ASSERT(ngramTable[0][1] == "bar");
    CPPUNIT_ASSERT(ngramTable[0][2] == "foobar");
    CPPUNIT_ASSERT(ngramTable[0][3] == "3");
    
    CPPUNIT_ASSERT(ngramTable[1][0] == "bar");
    CPPUNIT_ASSERT(ngramTable[1][1] == "foo");
    CPPUNIT_ASSERT(ngramTable[1][2] == "foobar");
    CPPUNIT_ASSERT(ngramTable[1][3] == "33");
    
    CPPUNIT_ASSERT(ngramTable[2][0] == "foobar");
    CPPUNIT_ASSERT(ngramTable[2][1] == "bar");
    CPPUNIT_ASSERT(ngramTable[2][2] == "foo");
    CPPUNIT_ASSERT(ngramTable[2][3] == "333"); 

}

void DatabaseConnectorTest::testGetNgramLikeTable()
{
    std::string* lastLikeQuery = new std::string();
    DatabaseConnectorLikeImpl* databaseConnectorLike = new DatabaseConnectorLikeImpl(lastLikeQuery);

    assertCorrectMockNgramTable(databaseConnectorLike->getNgramLikeTable(*unigram));
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT word, count FROM _1_gram WHERE word LIKE 'foo%' ORDER BY count DESC;"),
			  *lastLikeQuery );

    databaseConnectorLike->getNgramLikeTable(*bigram);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT word_1, word, count FROM _2_gram WHERE word_1 = 'foo' AND word LIKE 'bar%' ORDER BY count DESC;"),
			  *lastLikeQuery );

    databaseConnectorLike->getNgramLikeTable(*trigram);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT word_2, word_1, word, count FROM _3_gram WHERE word_2 = 'foo' AND word_1 = 'bar' AND word LIKE 'foobar%' ORDER BY count DESC;"),
			  *lastLikeQuery );

    delete lastLikeQuery;
    delete databaseConnectorLike;
}

void DatabaseConnectorTest::testGetNgramLikeTableLimit()
{
    std::string* lastLikeQuery = new std::string();
    DatabaseConnectorLikeImpl* databaseConnectorLike = new DatabaseConnectorLikeImpl(lastLikeQuery);

    assertCorrectMockNgramTable(databaseConnectorLike->getNgramLikeTable(*unigram, GLOBAL_MAGIC_NUMBER));
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT word, count FROM _1_gram WHERE word LIKE 'foo%' ORDER BY count DESC LIMIT 1337;"),
			  *lastLikeQuery );

    databaseConnectorLike->getNgramLikeTable(*bigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT word_1, word, count FROM _2_gram WHERE word_1 = 'foo' AND word LIKE 'bar%' ORDER BY count DESC LIMIT 1337;"),
			  *lastLikeQuery );

    databaseConnectorLike->getNgramLikeTable(*trigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("SELECT word_2, word_1, word, count FROM _3_gram WHERE word_2 = 'foo' AND word_1 = 'bar' AND word LIKE 'foobar%' ORDER BY count DESC LIMIT 1337;"),
			  *lastLikeQuery );

    delete lastLikeQuery;
    delete databaseConnectorLike;
}

void DatabaseConnectorTest::testInsertNgram()
{
    databaseConnector->insertNgram(*unigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("INSERT INTO _1_gram VALUES('foo', 1337);"),
			  *lastQuery );
    
    databaseConnector->insertNgram(*bigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("INSERT INTO _2_gram VALUES('foo', 'bar', 1337);"),
			  *lastQuery );

    databaseConnector->insertNgram(*trigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("INSERT INTO _3_gram VALUES('foo', 'bar', 'foobar', 1337);"),
			  *lastQuery );
}

void DatabaseConnectorTest::testUpdateNgram()
{
    databaseConnector->updateNgram(*unigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("UPDATE _1_gram SET count = 1337 WHERE word = 'foo';"),
			  *lastQuery );

    databaseConnector->updateNgram(*bigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("UPDATE _2_gram SET count = 1337 WHERE word_1 = 'foo' AND word = 'bar';"),
			  *lastQuery );
			  
    databaseConnector->updateNgram(*trigram, GLOBAL_MAGIC_NUMBER);
    CPPUNIT_ASSERT_EQUAL( static_cast<std::string>("UPDATE _3_gram SET count = 1337 WHERE word_2 = 'foo' AND word_1 = 'bar' AND word = 'foobar';"),
			  *lastQuery );
}

void DatabaseConnectorTest::testRemoveNgram()
{
    databaseConnector->removeNgram(*unigram);
    databaseConnector->removeNgram(*bigram);
    databaseConnector->removeNgram(*trigram);
}

//void DatabaseConnectorTest::testSanitizeString()
//{
//    // TODO
//}
