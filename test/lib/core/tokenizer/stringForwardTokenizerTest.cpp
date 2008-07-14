
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


#include "stringForwardTokenizerTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( StringForwardTokenizerTest );

void StringForwardTokenizerTest::setUp()
{
    blankspaces = " \n\t";
    separators = ".,!";

    stringSuite = new TestStringSuite();
}

void StringForwardTokenizerTest::tearDown()
{
    delete stringSuite;
}

void StringForwardTokenizerTest::testNextToken()
{
    int i;

    while (stringSuite->hasMoreTestStrings()) {
        //std::stringstream ss;
        //ss << stringSuite->currentTestString()->getstr();
	//ForwardTokenizer tok(ss, blankspaces, separators);

        std::stringbuf sb;
	sb.str(stringSuite->currentTestString()->getstr());

        std::stringstream ss(sb.str());

        ForwardTokenizer tok(ss, blankspaces, separators);

	for (i = 0; tok.hasMoreTokens(); i++) {
	    CPPUNIT_ASSERT_EQUAL( stringSuite->currentTestString()->token(i),
				  tok.nextToken() );
	}
	CPPUNIT_ASSERT_EQUAL( stringSuite->currentTestString()->tokencount(),
			      i );

	stringSuite->nextTestString();
    }
}
