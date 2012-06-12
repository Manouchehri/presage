
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


#include "reverseTokenizerTest.h"

#include <iostream>
#include <assert.h>

CPPUNIT_TEST_SUITE_REGISTRATION( ReverseTokenizerTest );

void ReverseTokenizerTest::setUp()
{
    blankspaces = " \n\t";
    separators = ".,!";

    stringSuite = new TestStringSuite();
}

void ReverseTokenizerTest::tearDown()
{
    delete stringSuite;
}

void ReverseTokenizerTest::testConstructor()
{
    std::stringstream ss;
    ReverseTokenizer tok1(ss, blankspaces, separators);
    
    CPPUNIT_ASSERT( tok1.blankspaceChars() == blankspaces );
    CPPUNIT_ASSERT( tok1.separatorChars() == separators );
}

void ReverseTokenizerTest::testNextToken()
{
    int i;
    while (stringSuite->hasMoreTestStrings()) {
	std::stringstream ss;
	assert(stringSuite->currentTestString() != 0);
	ss << stringSuite->currentTestString()->getstr();
	ReverseTokenizer tok(ss, blankspaces, separators);
        int tokenCount = stringSuite->currentTestString()->tokencount();
        //std::cerr << "[string] " << ss.str() << std::endl;
	for (i = 0; tok.hasMoreTokens(); i++) {
            //CPPUNIT_ASSERT_EQUAL( stringSuite->currentTestString()->token(tokenCount-1-i),
            //tok.nextToken() );
            std::string token = tok.nextToken();
            //std::cerr << "[token] " << token
            //          << " [expected] " << stringSuite->currentTestString()->token(tokenCount-1-i)
            //          << std::endl;
	    CPPUNIT_ASSERT_EQUAL( stringSuite->currentTestString()->token(tokenCount-1-i),
				  token );
	}
	stringSuite->nextTestString();
    }
}

void ReverseTokenizerTest::testCountTokens()
{
    while (stringSuite->hasMoreTestStrings()) {
	std::stringstream ss;
	assert(stringSuite->currentTestString() != 0);
	ss << stringSuite->currentTestString()->getstr();
	ReverseTokenizer tok(ss, blankspaces, separators);
        CPPUNIT_ASSERT_EQUAL(stringSuite->currentTestString()->tokencount(),
                             tok.countTokens());
	stringSuite->nextTestString();
    }
}

void ReverseTokenizerTest::testLowercaseMode()
{
    int i;
    while (stringSuite->hasMoreTestStrings()) {
	std::stringstream ss;
	assert(stringSuite->currentTestString() != 0);
	ss << stringSuite->currentTestString()->getstr();
	ReverseTokenizer tok(ss, blankspaces, separators);
	tok.lowercaseMode(true);
        int tokenCount = stringSuite->currentTestString()->tokencount();
	std::string str;
	for (i = 0; tok.hasMoreTokens(); i++) {
	    str = stringSuite->currentTestString()->token(tokenCount - 1 - i);
	    // convert string to lowercase
	    for (std::string::iterator it = str.begin();
		 it != str.end();
		 it++) {
		*it = tolower(*it);
	    }
	    CPPUNIT_ASSERT_EQUAL( str,
				  tok.nextToken() );
	}
	stringSuite->nextTestString();
    }
}

void ReverseTokenizerTest::testProgress()
{
    std::stringstream ss;
    ss << "01 02 03 04 05 06 07 08 09 10";
    ReverseTokenizer tok(ss, " ", " ");
    double expectedProgress = 0.0;
    const double expectedProgressIncrement = 0.1;
    const double delta = 0.07;
    while (tok.hasMoreTokens()) {
        expectedProgress += expectedProgressIncrement;
        tok.nextToken();
        //std::cerr << "[expected] " << expectedProgress 
        //          << " [progress] " << tok.progress() << std::endl;
        CPPUNIT_ASSERT( (expectedProgress - delta) < tok.progress() );
        CPPUNIT_ASSERT( tok.progress() < (expectedProgress + delta) );
    }
}

void ReverseTokenizerTest::testRepeatability()
{
    const int REPETITIONS = 3;

    std::stringstream ss;
    std::streambuf* sb = ss.rdbuf();
    int index = 0;

    std::string str = "foo bar foobar";
    const int TOKENS = 3;
    std::string tokens[3];
    tokens[0] = "foobar";
    tokens[1] = "bar";
    tokens[2] = "foo";

    // insert test string into stream's streambuf
    std::string::const_iterator str_it = str.begin();
    while (str_it != str.end()) {
	sb->sputc(*str_it);
	str_it++;
    }

    // build expected stringstream
    std::stringstream expected_ss;
    while (index < REPETITIONS) {
	for (int i = 0; i < TOKENS; i++) {
	    expected_ss << tokens[i] << std::endl;
	}
	index++;
    }

    // tokenize the same stream REPETITIONS times and store the resuls
    // in actual_ss for later comparison with expected_ss
    std::stringstream actual_ss;
    for (int i = 0; i < REPETITIONS; i++ ) {
	index = 0;
	ReverseTokenizer rT(ss, blankspaces, separators);
	//std::cerr << "[stream] " << rT.streamToString() << std::endl;
	while (rT.hasMoreTokens()) {
	    std::string token = rT.nextToken();
	    //std::cerr << "[repeatability] " << token << std::endl;
	    actual_ss << token << std::endl;
	    CPPUNIT_ASSERT_EQUAL( tokens[index], token );
	    index++;
	}
        //std::cerr << "ss.str() " << ss.str() << std::endl;
    }

    CPPUNIT_ASSERT_EQUAL( expected_ss.str(), actual_ss.str() );
}
