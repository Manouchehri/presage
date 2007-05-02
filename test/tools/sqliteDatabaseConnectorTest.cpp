
/******************************************************************************\
 *                                                                            *
 * Soothsayer, an extensible predictive text entry system                     *
 * ------------------------------------------------------                     *
 *                                                                            *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>             *
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


#include "sqliteDatabaseConnectorTest.h"

// for unlink(), getcwd()
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

// for opendir(), readdir(), closedir(), DIR, dirent
#if HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#include <stdio.h>
#include <stdlib.h>

#include <fstream>

#include <assert.h>

CPPUNIT_TEST_SUITE_REGISTRATION( SqliteDatabaseConnectorTest );

void SqliteDatabaseConnectorTest::setUp()
{
    sqliteDatabaseConnector = new SqliteDatabaseConnector(DEFAULT_DATABASE_FILENAME);

    unigram = new Ngram;
    unigram->push_back("foo");

    bigram = new Ngram;
    bigram->push_back("foo");
    bigram->push_back("bar");

    trigram = new Ngram;
    trigram->push_back("foo");
    trigram->push_back("bar");
    trigram->push_back("foobar");

    unigram1 = new Ngram;
    unigram1->push_back("foo1");

    bigram1 = new Ngram;
    bigram1->push_back("foo");
    bigram1->push_back("bar1");

    trigram1 = new Ngram;
    trigram1->push_back("foo");
    trigram1->push_back("bar");
    trigram1->push_back("foobar1");
}

void SqliteDatabaseConnectorTest::tearDown()
{
    delete sqliteDatabaseConnector;

    delete unigram;
    delete bigram;
    delete trigram;

    delete unigram1;
    delete bigram1;
    delete trigram1;

    assertExistsAndRemoveFile(DEFAULT_DATABASE_FILENAME);
}

void SqliteDatabaseConnectorTest::testConstructor()
{
    std::cout << "SqliteDatabaseConnectorTest::testConstructor()" << std::endl;

    // This tests that an empty database is created when an instance
    // of SqliteDatabaseConstructor is created.  The SqliteDatabase
    // instantiation is done in the setUp() method and the testing is
    // actually done in the tearDown() method.
}

void SqliteDatabaseConnectorTest::testInsertNgram()
{
    std::cout << "SqliteDatabaseConnectorTest::testInsertNgram()" << std::endl;

    // test that no insertion occurs since tables have not been
    // yet created
    sqliteDatabaseConnector->insertNgram(*unigram, MAGIC_NUMBER);
    sqliteDatabaseConnector->insertNgram(*bigram, MAGIC_NUMBER);
    sqliteDatabaseConnector->insertNgram(*trigram, MAGIC_NUMBER);

    // populate database
    sqliteDatabaseConnector->createNgramTable(1);
    sqliteDatabaseConnector->insertNgram(*unigram, MAGIC_NUMBER);

    sqliteDatabaseConnector->createNgramTable(2);
    sqliteDatabaseConnector->insertNgram(*bigram, MAGIC_NUMBER);

    sqliteDatabaseConnector->createNgramTable(3);
    sqliteDatabaseConnector->insertNgram(*trigram, MAGIC_NUMBER);
    
    // compare database dump with benchmark string
    std::stringstream benchmark;
    benchmark
	<< "BEGIN TRANSACTION;"                                                                                        << std::endl
	<< "CREATE TABLE _1_gram (word TEXT, count INTEGER, UNIQUE(word) );"                                           << std::endl
	<< "INSERT INTO _1_gram VALUES('foo',1337);"								       << std::endl
	<< "CREATE TABLE _2_gram (word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_1, word) );"		       << std::endl
	<< "INSERT INTO _2_gram VALUES('foo','bar',1337);"							       << std::endl
	<< "CREATE TABLE _3_gram (word_2 TEXT, word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_2, word_1, word) );" << std::endl
	<< "INSERT INTO _3_gram VALUES('foo','bar','foobar',1337);"						       << std::endl
	<< "COMMIT;"                                                                                                   << std::endl;

    assertDatabaseDumpEqualsBenchmark(benchmark);
}

void SqliteDatabaseConnectorTest::testUpdateNgram()
{
    std::cout << "SqliteDatabaseConnectorTest::testUpdateNgram()" << std::endl;

    // test that no insertion occurs since tables have not been
    // yet created
    sqliteDatabaseConnector->updateNgram(*unigram, MAGIC_NUMBER);
    sqliteDatabaseConnector->updateNgram(*bigram, MAGIC_NUMBER);
    sqliteDatabaseConnector->updateNgram(*trigram, MAGIC_NUMBER);

    // populate database
    sqliteDatabaseConnector->createNgramTable(1);
    sqliteDatabaseConnector->insertNgram(*unigram, MAGIC_NUMBER);
    sqliteDatabaseConnector->updateNgram(*unigram, MAGIC_NUMBER * 2);

    sqliteDatabaseConnector->createNgramTable(2);
    sqliteDatabaseConnector->insertNgram(*bigram, MAGIC_NUMBER);
    sqliteDatabaseConnector->updateNgram(*bigram, MAGIC_NUMBER * 2);

    sqliteDatabaseConnector->createNgramTable(3);
    sqliteDatabaseConnector->insertNgram(*trigram, MAGIC_NUMBER);
    sqliteDatabaseConnector->updateNgram(*trigram, MAGIC_NUMBER * 2);
    
    // compare database dump with benchmark string
    std::stringstream benchmark;
    benchmark
        << "BEGIN TRANSACTION;"                                                                                        << std::endl
        << "CREATE TABLE _1_gram (word TEXT, count INTEGER, UNIQUE(word) );"                                           << std::endl
        << "INSERT INTO _1_gram VALUES('foo',2674);"                                                                   << std::endl
        << "CREATE TABLE _2_gram (word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_1, word) );"                      << std::endl
        << "INSERT INTO _2_gram VALUES('foo','bar',2674);"                                                             << std::endl
        << "CREATE TABLE _3_gram (word_2 TEXT, word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_2, word_1, word) );" << std::endl
        << "INSERT INTO _3_gram VALUES('foo','bar','foobar',2674);"                                                    << std::endl
        << "COMMIT;"                                                                                                   << std::endl;
    
    assertDatabaseDumpEqualsBenchmark(benchmark);
}

void SqliteDatabaseConnectorTest::testRemoveNgram()
{
    //CPPUNIT_FAIL("Test not written yet!");
}

void SqliteDatabaseConnectorTest::testGetNgramCount()
{
    std::cout << "SqliteDatabaseConnectorTest::testGetNgramCount()" << std::endl;

    // populate database
    sqliteDatabaseConnector->createNgramTable(1);
    sqliteDatabaseConnector->insertNgram(*unigram, MAGIC_NUMBER);

    sqliteDatabaseConnector->createNgramTable(2);
    sqliteDatabaseConnector->insertNgram(*bigram, MAGIC_NUMBER);

    sqliteDatabaseConnector->createNgramTable(3);
    sqliteDatabaseConnector->insertNgram(*trigram, MAGIC_NUMBER);
    
    CPPUNIT_ASSERT_EQUAL( MAGIC_NUMBER,
			  sqliteDatabaseConnector->getNgramCount(*unigram) );
    CPPUNIT_ASSERT_EQUAL( MAGIC_NUMBER,
			  sqliteDatabaseConnector->getNgramCount(*bigram) );
    CPPUNIT_ASSERT_EQUAL( MAGIC_NUMBER, 
			  sqliteDatabaseConnector->getNgramCount(*trigram) );
}

void SqliteDatabaseConnectorTest::testIncrementNgramCount()
{
    std::cout << "SqliteDatabaseConnectorTest::testIncrementNgramCount()" << std::endl;

    // test that no insertion occurs since tables have not been
    // yet created
    sqliteDatabaseConnector->incrementNgramCount(*unigram);
    sqliteDatabaseConnector->incrementNgramCount(*bigram);
    sqliteDatabaseConnector->incrementNgramCount(*trigram);

    // populate database
    sqliteDatabaseConnector->createNgramTable(1);
    sqliteDatabaseConnector->incrementNgramCount(*unigram);
    sqliteDatabaseConnector->incrementNgramCount(*unigram);
    sqliteDatabaseConnector->incrementNgramCount(*unigram);

    sqliteDatabaseConnector->createNgramTable(2);
    sqliteDatabaseConnector->incrementNgramCount(*bigram);
    sqliteDatabaseConnector->incrementNgramCount(*bigram);
    sqliteDatabaseConnector->incrementNgramCount(*bigram);

    sqliteDatabaseConnector->createNgramTable(3);
    sqliteDatabaseConnector->incrementNgramCount(*trigram);
    sqliteDatabaseConnector->incrementNgramCount(*trigram);
    sqliteDatabaseConnector->incrementNgramCount(*trigram);
    
    // compare database dump with benchmark string
    std::stringstream benchmark;
    benchmark
	<< "BEGIN TRANSACTION;"                                                                                        << std::endl
	<< "CREATE TABLE _1_gram (word TEXT, count INTEGER, UNIQUE(word) );"                                           << std::endl
	<< "INSERT INTO _1_gram VALUES('foo',3);"                                                                      << std::endl
	<< "CREATE TABLE _2_gram (word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_1, word) );"                      << std::endl
	<< "INSERT INTO _2_gram VALUES('foo','bar',3);"                                                                << std::endl
	<< "CREATE TABLE _3_gram (word_2 TEXT, word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_2, word_1, word) );" << std::endl
	<< "INSERT INTO _3_gram VALUES('foo','bar','foobar',3);"                                                       << std::endl
	<< "COMMIT;"                                                                                                   << std::endl;

    assertDatabaseDumpEqualsBenchmark(benchmark);
}

void SqliteDatabaseConnectorTest::testGetNgramLikeTable()
{
    std::cout << "SqliteDatabaseConnectorTest::testGetNgramLikeTable()" << std::endl;

    // populate database
    sqliteDatabaseConnector->createNgramTable(1);
    sqliteDatabaseConnector->insertNgram(*unigram, MAGIC_NUMBER);

    sqliteDatabaseConnector->createNgramTable(2);
    sqliteDatabaseConnector->insertNgram(*bigram, MAGIC_NUMBER);

    sqliteDatabaseConnector->createNgramTable(3);
    sqliteDatabaseConnector->insertNgram(*trigram, MAGIC_NUMBER);

    NgramTable* expected_uni;
    NgramTable* expected_bi;
    NgramTable* expected_tri;
    NgramTable actual;

    Ngram* ngram;

    expected_uni = new NgramTable();
    ngram = new Ngram();
    ngram->push_back("foo");
    ngram->push_back("1337");
    expected_uni->push_back(*ngram);
    actual = sqliteDatabaseConnector->getNgramLikeTable(*unigram);
    assertEqualNgramTable(expected_uni, actual);
    delete ngram;

    expected_bi = new NgramTable();
    ngram = new Ngram();
    ngram->push_back("foo");
    ngram->push_back("bar");
    ngram->push_back("1337");
    expected_bi->push_back(*ngram);
    actual = sqliteDatabaseConnector->getNgramLikeTable(*bigram);
    assertEqualNgramTable(expected_bi, actual);
    delete ngram;

    expected_tri = new NgramTable();
    ngram = new Ngram();
    ngram->push_back("foo");
    ngram->push_back("bar");
    ngram->push_back("foobar");
    ngram->push_back("1337");
    expected_tri->push_back(*ngram);
    actual = sqliteDatabaseConnector->getNgramLikeTable(*trigram);
    assertEqualNgramTable(expected_tri, actual);
    delete ngram;

    // add similar ngrams to database
    sqliteDatabaseConnector->insertNgram(*unigram1, MAGIC_NUMBER - 1);
    sqliteDatabaseConnector->insertNgram(*bigram1, MAGIC_NUMBER - 1);
    sqliteDatabaseConnector->insertNgram(*trigram1, MAGIC_NUMBER - 1);

    ngram = new Ngram();
    ngram->push_back("foo1");
    ngram->push_back("1336");
    expected_uni->push_back(*ngram);
    actual = sqliteDatabaseConnector->getNgramLikeTable(*unigram);
    assertEqualNgramTable(expected_uni, actual);
    delete ngram;

    ngram = new Ngram();
    ngram->push_back("foo");
    ngram->push_back("bar1");
    ngram->push_back("1336");
    expected_bi->push_back(*ngram);
    actual = sqliteDatabaseConnector->getNgramLikeTable(*bigram);
    assertEqualNgramTable(expected_bi, actual);
    delete ngram;

    ngram = new Ngram();
    ngram->push_back("foo");
    ngram->push_back("bar");
    ngram->push_back("foobar1");
    ngram->push_back("1336");
    expected_tri->push_back(*ngram);
    actual = sqliteDatabaseConnector->getNgramLikeTable(*trigram);
    assertEqualNgramTable(expected_tri, actual);
    delete ngram;

    delete expected_uni;
    delete expected_bi;
    delete expected_tri;
}

void SqliteDatabaseConnectorTest::assertEqualNgramTable(const NgramTable* const expected, const NgramTable& actual)
{
    CPPUNIT_ASSERT_EQUAL(expected->size(), actual.size());
    for (int i = 0; i < expected->size(); i++) {
	CPPUNIT_ASSERT_EQUAL((*expected)[i].size(), actual[i].size());
	for (int j = 0; j < (*expected)[i].size(); j++) {
	    std::cout << "[assertEqualNgramTable] (" << (*expected)[i][j] << ", " << actual[i][j] << ")" << std::endl;
	    CPPUNIT_ASSERT((*expected)[i][j] == actual[i][j]);
	}
    }
}

void SqliteDatabaseConnectorTest::assertExistsAndRemoveFile(const char* filename) const
{
#ifdef HAVE_DIRENT_H
    bool result = false;
    DIR* dp;
    struct dirent *ep;

    // check if file exists in current directory
    dp = opendir ("./");
    if (dp != NULL) {
        while ((ep = readdir (dp)) && (result == 0)) {
            if (strcmp(ep->d_name, filename) == 0) {
                result = true;
            }
        }
        (void) closedir (dp);
    } else {
        perror ("[SqliteDatabaseConnectorTest] Couldn't open current directory!");
    }

    CPPUNIT_ASSERT( result == true );
#else
    // if we cannot determine file existance, let us assume that file
    // exists
    bool result = true;
#endif

#ifdef HAVE_UNISTD_H
    // remove file if it exists
    if (result) {
        result = unlink(filename);
        assert(result == 0);
    }
#else
    // fail test
    std::string message = "Unable to remove database file ";
    message += filename;
    CPPUNIT_FAIL( message.c_str() );
    //exit(-1);
#endif
}

void SqliteDatabaseConnectorTest::assertDatabaseDumpEqualsBenchmark(std::stringstream& benchmark) const
{
    // dump database content using `sqlite' client
    std::string command = static_cast<std::string>("sqlite ")
        + DEFAULT_DATABASE_FILENAME + " '.dump' > " + DATABASE_DUMP;

    int result = system(command.c_str());
    CPPUNIT_ASSERT( result == 0 );

    std::ifstream database_dump_stream(DATABASE_DUMP);

    // assert streams contain equal number of characters
    benchmark.seekg(0, std::ios::end);
    database_dump_stream.seekg(0, std::ios::end);
    CPPUNIT_ASSERT( benchmark.tellg() == database_dump_stream.tellg() );
    benchmark.seekg(0);
    database_dump_stream.seekg(0);

    // assert streams contain same characters
    std::string actual;
    std::string expected;
    bool        equal = true;;
    while (database_dump_stream >> actual && benchmark >> expected) {
	equal = (expected == actual);
	//if (!equal)
	    std::cout << "[expected} " << expected << " [actual] " << actual << std::endl;
	CPPUNIT_ASSERT( equal );
    }

    assertExistsAndRemoveFile(DATABASE_DUMP);

////
// Using pipes instead of writing to a temp file.
// Currently does not work.
//
//    std::string command = static_cast<std::string>("sqlite ") + DEFAULT_DATABASE_FILENAME + " '.dump' > " + DATABASE_DUMP;
//    FILE* pipe = popen(command.c_str(), "r");
//    while (!feof(pipe) && !ferror(pipe)) {
//        std::cout << static_cast<char>(fgetc(pipe));
//    }
//
//    CPPUNIT_ASSERT( pipe != NULL );
//    if (pipe) {
//        int expected;
//        int actual;
//        while(benchmark.good() && !feof(pipe) && !ferror(pipe)) {
//            expected = benchmark.get();
//            actual = fgetc(pipe);
//
//            std::cout << "[expected] " << static_cast<char>(expected)
//                      << " [actual] " << static_cast<char>(actual) << std::endl;
//
//
//            CPPUNIT_ASSERT_EQUAL( expected, actual );
//        }
//
//        pclose(pipe);
//    } else {
//        CPPUNIT_FAIL( "Unable to open pipe to execute `sqlite'" );
//    }
}
