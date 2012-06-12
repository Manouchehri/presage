
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


#include "crossCheckTokenizerTest.h"

#include <list>
#include <iostream>
#include <assert.h>

CPPUNIT_TEST_SUITE_REGISTRATION( CrossCheckTokenizerTest );

void CrossCheckTokenizerTest::setUp()
{
    blankspaces = " \n\t";
    separators = ".,!";

    stringSuite = new TestStringSuite();
}

void CrossCheckTokenizerTest::tearDown()
{
    delete stringSuite;
}

void CrossCheckTokenizerTest::runCrossCheck(
    ForwardTokenizer& forTok,
    ReverseTokenizer& revTok)
{
    //std::cerr << "CrossCheckTokenizerTest::runCrossCheck()" << std::endl;

    while (stringSuite->hasMoreTestStrings()) {
	std::list<std::string> forwardTokenization;
	std::list<std::string> reverseTokenization;
	while (forTok.hasMoreTokens()) {
	    forwardTokenization.push_back(forTok.nextToken());
	}
	while (revTok.hasMoreTokens()) {
	    reverseTokenization.push_front(revTok.nextToken());
	}

	CPPUNIT_ASSERT( forwardTokenization.size() == reverseTokenization.size() );

	std::list<std::string>::const_iterator forit = forwardTokenization.begin();
	std::list<std::string>::const_iterator revit = reverseTokenization.begin();
	while (forit != forwardTokenization.end() 
               && revit != reverseTokenization.end()) {
	    CPPUNIT_ASSERT( *forit == *revit );
	    forit++;
	    revit++;
	}

	stringSuite->nextTestString();
    }
}

void CrossCheckTokenizerTest::testNextToken()
{
    //std::cerr << "CrossCheckTokenizerTest::testNextToken()" << std::endl;
    assert(stringSuite->currentTestString() != 0);

    std::stringstream forss;
    std::stringstream revss;
    assert(stringSuite->currentTestString() != 0);
    forss << stringSuite->currentTestString()->getstr();
    revss << stringSuite->currentTestString()->getstr();
    ForwardTokenizer forTok(forss, blankspaces, separators);
    ReverseTokenizer revTok(revss, blankspaces, separators);

    runCrossCheck(forTok, revTok);
}

void CrossCheckTokenizerTest::testLowercaseMode()
{
    //std::cerr << "CrossCheckTokenizerTest::testLowercaseMode()" << std::endl;
    assert(stringSuite->currentTestString() != 0);

    std::stringstream forss;
    std::stringstream revss;
    forss << stringSuite->currentTestString()->getstr();
    revss << stringSuite->currentTestString()->getstr();
    ForwardTokenizer forTok(forss, blankspaces, separators);
    ReverseTokenizer revTok(revss, blankspaces, separators);
    forTok.lowercaseMode(true);
    revTok.lowercaseMode(true);

    runCrossCheck(forTok, revTok);
}

void CrossCheckTokenizerTest::testProgress()
{
    //std::cerr << "CrossCheckTokenizerTest::testProgress()" << std::endl;
    std::stringstream forss;
    std::stringstream revss;
    const std::string str("01 02 03 04 05 06 07 08 09 10");
    forss << str;
    revss << str;
    ForwardTokenizer forTok(forss, blankspaces, separators);
    ReverseTokenizer revTok(revss, blankspaces, separators);
    //forTok.lowercaseMode(true);
    //revTok.lowercaseMode(true);

    const double delta = 0.3;

    while (forTok.hasMoreTokens() && revTok.hasMoreTokens()) {
	CPPUNIT_ASSERT( (-delta < (forTok.progress() - revTok.progress()))
			&& ((forTok.progress() - revTok.progress()) < delta));
	//std::cerr << forTok.nextToken() << ' ' << forTok.progress() << '-'
	//          << revTok.nextToken() << ' ' << revTok.progress() << std::endl;
	forTok.nextToken();
	revTok.nextToken();
    }
}
