
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


#include "sqliteDatabaseConnectorTest.h"

// for unlink(), getcwd()
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
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
    CPPUNIT_ASSERT_NO_THROW
	(
	    sqliteDatabaseConnector = new SqliteDatabaseConnector(DEFAULT_DATABASE_FILENAME,
								  DEFAULT_DATABASE_CARDINALITY,
								  DEFAULT_DATABASE_READ_WRITE_MODE)
	);

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
    // This tests that an empty database is created when an instance
    // of SqliteDatabaseConstructor is created.  The SqliteDatabase
    // instantiation is done in the setUp() method and the testing is
    // actually done in the tearDown() method.
}

void SqliteDatabaseConnectorTest::testInsertNgram()
{
    // populate database
    CPPUNIT_ASSERT_NO_THROW(sqliteDatabaseConnector->insertNgram(*unigram, MAGIC_NUMBER));
    CPPUNIT_ASSERT_NO_THROW(sqliteDatabaseConnector->insertNgram(*bigram, MAGIC_NUMBER));
    CPPUNIT_ASSERT_NO_THROW(sqliteDatabaseConnector->insertNgram(*trigram, MAGIC_NUMBER));

    // test no exception occurs when attempting to recreate existing tables
    CPPUNIT_ASSERT_NO_THROW(sqliteDatabaseConnector->createNgramTable(1));
    CPPUNIT_ASSERT_NO_THROW(sqliteDatabaseConnector->createNgramTable(2));
    CPPUNIT_ASSERT_NO_THROW(sqliteDatabaseConnector->createNgramTable(3));
    
    // compare database dump with benchmark string
    std::stringstream benchmark;
    benchmark
#if defined(HAVE_SQLITE3_H)
        << "BEGIN TRANSACTION;"                                                                                        << std::endl
        << "CREATE TABLE _1_gram (word TEXT, count INTEGER, UNIQUE(word) );"                                           << std::endl
        << "INSERT INTO \"_1_gram\" VALUES('foo', 1337);"                                                              << std::endl
        << "CREATE TABLE _2_gram (word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_1, word) );"                      << std::endl
        << "INSERT INTO \"_2_gram\" VALUES('foo', 'bar', 1337);"                                                       << std::endl
        << "CREATE TABLE _3_gram (word_2 TEXT, word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_2, word_1, word) );" << std::endl
        << "INSERT INTO \"_3_gram\" VALUES('foo', 'bar', 'foobar', 1337);"                                             << std::endl
        << "COMMIT;"                                                                                                   << std::endl;
#elif defined(HAVE_SQLITE_H)
	<< "BEGIN TRANSACTION;"                                                                                        << std::endl
	<< "CREATE TABLE _1_gram (word TEXT, count INTEGER, UNIQUE(word) );"                                           << std::endl
	<< "INSERT INTO _1_gram VALUES('foo',1337);"								       << std::endl
	<< "CREATE TABLE _2_gram (word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_1, word) );"		       << std::endl
	<< "INSERT INTO _2_gram VALUES('foo','bar',1337);"							       << std::endl
	<< "CREATE TABLE _3_gram (word_2 TEXT, word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_2, word_1, word) );" << std::endl
	<< "INSERT INTO _3_gram VALUES('foo','bar','foobar',1337);"						       << std::endl
	<< "COMMIT;"                                                                                                   << std::endl;
#endif

    assertDatabaseDumpEqualsBenchmark(benchmark);
}

void SqliteDatabaseConnectorTest::testUpdateNgram()
{
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
#if defined(HAVE_SQLITE3_H)
        << "INSERT INTO \"_1_gram\" VALUES('foo', 2674);"                                                              << std::endl
#elif defined(HAVE_SQLITE_H)
        << "INSERT INTO _1_gram VALUES('foo',2674);"                                                                   << std::endl
#endif
        << "CREATE TABLE _2_gram (word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_1, word) );"                      << std::endl
#if defined(HAVE_SQLITE3_H)
        << "INSERT INTO \"_2_gram\" VALUES('foo', 'bar', 2674);"                                                       << std::endl
#elif defined(HAVE_SQLITE_H)
        << "INSERT INTO _2_gram VALUES('foo','bar',2674);"                                                             << std::endl
#endif
        << "CREATE TABLE _3_gram (word_2 TEXT, word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_2, word_1, word) );" << std::endl
#if defined(HAVE_SQLITE3_H)
        << "INSERT INTO \"_3_gram\" VALUES('foo', 'bar', 'foobar', 2674);"                                             << std::endl
#elif defined(HAVE_SQLITE_H)
        << "INSERT INTO _3_gram VALUES('foo','bar','foobar',2674);"                                                    << std::endl
#endif
        << "COMMIT;"                                                                                                   << std::endl;
    
    assertDatabaseDumpEqualsBenchmark(benchmark);
}

void SqliteDatabaseConnectorTest::testRemoveNgram()
{
    //CPPUNIT_FAIL("Test not written yet!");
}

void SqliteDatabaseConnectorTest::testGetNgramCount()
{
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

    // Add crafted unigram to database to test that WHERE word =
    // "word" bug has been fixed. Fix is using WHERE word = 'word'
    // clause.
    // A keyword in single quotes is interpreted as a literal string
    // if it occurs in a context where a string literal is allowed,
    // otherwise it is understood as an identifier, whereas a
    // keyword in double-quotes is interpreted as an identifier if
    // it matches a known identifier - otherwise it is interpreted
    // as a string literal. The following WHERE word = "word" is a
    // no-op, the correct clause is WHERE word = 'word'.
    Ngram* ngram = new Ngram();
    ngram->push_back("word");
    sqliteDatabaseConnector->insertNgram(*ngram, MAGIC_NUMBER + 1);
    CPPUNIT_ASSERT_EQUAL( MAGIC_NUMBER + 1,
                          sqliteDatabaseConnector->getNgramCount(*ngram) );
    delete ngram;
}

void SqliteDatabaseConnectorTest::testIncrementNgramCount()
{
    // populate database
    sqliteDatabaseConnector->incrementNgramCount(*unigram);
    sqliteDatabaseConnector->incrementNgramCount(*unigram);
    sqliteDatabaseConnector->incrementNgramCount(*unigram);

    sqliteDatabaseConnector->incrementNgramCount(*bigram);
    sqliteDatabaseConnector->incrementNgramCount(*bigram);
    sqliteDatabaseConnector->incrementNgramCount(*bigram);

    sqliteDatabaseConnector->incrementNgramCount(*trigram);
    sqliteDatabaseConnector->incrementNgramCount(*trigram);
    sqliteDatabaseConnector->incrementNgramCount(*trigram);

    // compare database dump with benchmark string
    std::stringstream benchmark;
    benchmark
	<< "BEGIN TRANSACTION;"                                                                                        << std::endl
	<< "CREATE TABLE _1_gram (word TEXT, count INTEGER, UNIQUE(word) );"                                           << std::endl
#if defined(HAVE_SQLITE3_H)
	<< "INSERT INTO \"_1_gram\" VALUES('foo', 3);"                                                                 << std::endl
#elif defined(HAVE_SQLITE_H)
	<< "INSERT INTO _1_gram VALUES('foo',3);"                                                                      << std::endl
#endif
	<< "CREATE TABLE _2_gram (word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_1, word) );"                      << std::endl
#if defined(HAVE_SQLITE3_H)
	<< "INSERT INTO \"_2_gram\" VALUES('foo', 'bar', 3);"                                                          << std::endl
#elif defined(HAVE_SQLITE_H)
	<< "INSERT INTO _2_gram VALUES('foo','bar',3);"                                                                << std::endl
#endif
	<< "CREATE TABLE _3_gram (word_2 TEXT, word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_2, word_1, word) );" << std::endl
#if defined(HAVE_SQLITE3_H)
	<< "INSERT INTO \"_3_gram\" VALUES('foo', 'bar', 'foobar', 3);"                                                << std::endl
#elif defined(HAVE_SQLITE_H)
	<< "INSERT INTO _3_gram VALUES('foo','bar','foobar',3);"                                                       << std::endl
#endif
	<< "COMMIT;"                                                                                                   << std::endl;

    assertDatabaseDumpEqualsBenchmark(benchmark);
}

void SqliteDatabaseConnectorTest::testGetNgramLikeTable()
{
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
    for (size_t i = 0; i < expected->size(); i++) {
	CPPUNIT_ASSERT_EQUAL((*expected)[i].size(), actual[i].size());
	for (size_t j = 0; j < (*expected)[i].size(); j++) {
	    //std::cout << "[assertEqualNgramTable] (" << (*expected)[i][j] << ", " << actual[i][j] << ")" << std::endl;
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
        if (unlink(filename)) {
            perror ("Unable to remove file");
        }
    }
#else
    // fail test
    std::string message = "Unable to remove file: ";
    message += filename;
    CPPUNIT_FAIL( message.c_str() );
    //exit(-1);
#endif
}

void SqliteDatabaseConnectorTest::assertDatabaseDumpEqualsBenchmark(std::stringstream& benchmark) const
{
    // dump database content using `sqlite' client
    std::string command = 
#if defined(HAVE_SQLITE3_H)
	static_cast<std::string>("sqlite3 ")
#elif defined(HAVE_SQLITE_H)
	static_cast<std::string>("sqlite ")
#endif
        + DEFAULT_DATABASE_FILENAME + " \".dump\" > " + DATABASE_DUMP;

    //std::cout << "Executing `" << command << "'" << std::endl;
    int command_exit_value = system(command.c_str());
    CPPUNIT_ASSERT(command_exit_value == 0);

    std::ifstream database_dump_stream(DATABASE_DUMP);

    // NOTE: this block of code has been commented out to account for
    // the fact that different versions of the sqlite/sqlite3 binary
    // return slightly different dumps of the same database. The
    // differences are limited to whitespace, single quotes, or double
    // quotes; but they do make a stream length comparison
    // inconclusive as to determine if two streams are a dump of the
    // same database.
    //
    // assert streams contain equal number of characters
    //
    //benchmark.seekg(0, std::ios::end);
    //database_dump_stream.seekg(0, std::ios::end);
    //CPPUNIT_ASSERT( benchmark.tellg() == database_dump_stream.tellg() );
    //benchmark.seekg(0);
    //database_dump_stream.seekg(0);

    // remove PRAGMAs
    std::string buffer;
    std::stringstream stripped_database_dump_stream;
    while (getline(database_dump_stream, buffer)) {
        // if buffer does not contain substring "PRAGMA"
        if (buffer.find("PRAGMA") == std::string::npos) {
            stripped_database_dump_stream << buffer << std::endl;
        }
    }

    database_dump_stream.close();

    // assert streams contain same characters
    std::string actual;
    std::string expected;
    bool        equal = true;;
    while (getline(benchmark, expected)
	   && getline(stripped_database_dump_stream, actual)) {
        // remove " and ' from strings, because different versions of
        // sqlite insert either double quotes or single quotes or no
        // quotes at all.
        strip_char('"',  actual);
	strip_char('\'', actual);
	strip_char('"',  expected);
	strip_char('\'', expected);
	// also remove space from strings
	// TODO: removing space is not bulletproof, i.e. a really
	// difference in the SQL might slip by unseen.
	strip_char(' ',  actual);
	strip_char(' ',  expected);
	
	equal = (expected == actual);
	    //if (!equal)
	//std::cout << "[expected] " << expected << std::endl
	//	  << "[actual  ] " << actual << std::endl;
	CPPUNIT_ASSERT(equal);
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

void SqliteDatabaseConnectorTest::strip_char(char c, std::string& str) const
{
    //std::cout << "string before: " << str << std::endl;
    std::string::size_type pos = 0;
    while ((pos = str.find(c, pos)) != std::string::npos) {
        str.erase(pos, 1);
        //std::cout << "pos: " << pos << "string: " << str << std::endl;
    }
    //std::cout << "string after : " << str << std::endl;
}
