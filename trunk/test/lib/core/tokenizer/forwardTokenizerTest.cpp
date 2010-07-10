
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


#include "forwardTokenizerTest.h"

#include <iostream>
#include <assert.h>

CPPUNIT_TEST_SUITE_REGISTRATION( ForwardTokenizerTest );

void ForwardTokenizerTest::setUp()
{
    blankspaces = " \n\t";
    separators = ".,!";

    stringSuite = new TestStringSuite();
}

void ForwardTokenizerTest::tearDown()
{
    delete stringSuite;
}

void ForwardTokenizerTest::testConstructor()
{
    std::stringstream ss;
    ForwardTokenizer tok1(ss, blankspaces, separators);
    
    CPPUNIT_ASSERT( tok1.blankspaceChars() == blankspaces );
    CPPUNIT_ASSERT( tok1.separatorChars() == separators );
}

void ForwardTokenizerTest::testNextToken()
{
    //std::cerr << "ForwardTokenizerTest::testNextToken()" << std::endl;

    int i;
    while (stringSuite->hasMoreTestStrings()) {
	//std::cerr << "Entered loop!" << std::endl;
	std::stringstream ss;
	assert(stringSuite->currentTestString() != 0);
	//std::cerr << "stringSuite->currentTestString():" << stringSuite->currentTestString() << std::endl;
	ss << stringSuite->currentTestString()->getstr();
	//std::cerr << "Tokenizing string: " << ss.str() << std::endl;
	ForwardTokenizer tok(ss, blankspaces, separators);
	for (i = 0; tok.hasMoreTokens(); i++) {
	    CPPUNIT_ASSERT_EQUAL( stringSuite->currentTestString()->token(i),
				  tok.nextToken() );
	}
	stringSuite->nextTestString();
    }
}

void ForwardTokenizerTest::testCountTokens()
{
    //std::cerr << "ForwardTokenizerTest::testCountTokens()" << std::endl;

    while (stringSuite->hasMoreTestStrings()) {
	std::stringstream ss;
	assert(stringSuite->currentTestString() != 0);
	ss << stringSuite->currentTestString()->getstr();
	ForwardTokenizer tok(ss, blankspaces, separators);
        CPPUNIT_ASSERT_EQUAL(stringSuite->currentTestString()->tokencount(),
                             tok.countTokens());
	stringSuite->nextTestString();
    }
}

void ForwardTokenizerTest::testLowercaseMode()
{
    //std::cerr << "ForwardTokenizerTest::testLowercaseMode()" << std::endl;

    int i;
    while (stringSuite->hasMoreTestStrings()) {
	std::stringstream ss;
	assert(stringSuite->currentTestString() != 0);
	ss << stringSuite->currentTestString()->getstr();
	ForwardTokenizer tok(ss, blankspaces, separators);
	tok.lowercaseMode(true);
	std::string str;
	for (i = 0; tok.hasMoreTokens(); i++) {
	    str = stringSuite->currentTestString()->token(i);
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

void ForwardTokenizerTest::testProgress()
{
    //std::cerr << "ForwardTokenizerTest::testProgress()" << std::endl;

    std::stringstream ss;
    ss << "01 02 03 04 05 06 07 08 09 10";
    ForwardTokenizer tok(ss, " ", " ");
    double expectedProgress = 0.0;
    const double expectedProgressIncrement = 0.1;
    const double delta = 0.035;
    while (tok.hasMoreTokens()) {
        expectedProgress += expectedProgressIncrement;
        tok.nextToken();
        //std::cerr << "[expected] " << expectedProgress 
        //          << " [progress] " << tok.progress() << std::endl;
        CPPUNIT_ASSERT( (expectedProgress - delta) < tok.progress() );
        CPPUNIT_ASSERT( tok.progress() < (expectedProgress + delta) );
    }
}

void ForwardTokenizerTest::testRepeatability()
{
    //std::cerr << "ForwardTokenizerTest::testRepeatability()" << std::endl;

    const int REPETITIONS = 3;

    std::stringstream ss;
    std::streambuf* sb = ss.rdbuf();
    int index = 0;

    std::string str = "foo bar foobar";
    const int TOKENS = 3;
    std::string tokens[3];
    tokens[0] = "foo";
    tokens[1] = "bar";
    tokens[2] = "foobar";

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

    std::stringstream actual_ss;
    for (int i = 0; i < REPETITIONS; i++ ) {
	index = 0;
	ForwardTokenizer fT(ss, blankspaces, separators);
	//std::cerr << "[stream] " << fT.streamToString() << std::endl;
	while (fT.hasMoreTokens()) {
	    std::string token = fT.nextToken();
	    //std::cerr << "[repeatability] " << token << std::endl;
	    actual_ss << token << std::endl;
	    CPPUNIT_ASSERT_EQUAL( tokens[index], token );
	    index++;
	}
    }

    CPPUNIT_ASSERT_EQUAL( expected_ss.str(), actual_ss.str() );
}
