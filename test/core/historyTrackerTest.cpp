
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


#include "historyTrackerTest.h"
#include "core/historyTracker.h"
#include "core/profileManager.h"

#include <string>
#include <assert.h>

CPPUNIT_TEST_SUITE_REGISTRATION( HistoryTrackerTest );

void HistoryTrackerTest::setUp()
{
    testStringSuite = new TestStringSuite();

    profileManager = new ProfileManager();
    profileManager->buildProfile();
    profile = profileManager->getProfile();
}

void HistoryTrackerTest::tearDown()
{
    delete testStringSuite;

    delete profile;
    delete profileManager;
}


void HistoryTrackerTest::testConstructor()
{}

void HistoryTrackerTest::testUpdate()
{
    std::cerr << "HistoryTrackerTest::testUpdate()" << std::endl;

    HistoryTracker hT(profile);
    hT.update("foo bar foobar");
    std::cerr << "prefix: " << hT.getPrefix() << std::endl;
    std::cerr << "token : " << hT.getToken(1) << std::endl;
}

void HistoryTrackerTest::testGetPrefix()
{
    std::cerr << "HistoryTrackerTest::testGetPrefix()" << std::endl;

    while (testStringSuite->hasMoreTestStrings()) {
	HistoryTracker hT(profile);
	hT.update(testStringSuite->currentTestString()->getstr());

	assert(testStringSuite->currentTestString() != 0);
	std::cerr << "Test getting prefix from string: "
		  << testStringSuite->currentTestString()->getstr() 
		  << std::endl;

	std::string actual_prefix   = hT.getPrefix();
	std::string expected_prefix = testStringSuite->currentTestString()->token(testStringSuite->currentTestString()->tokencount() - 1);

	std::cerr << "actual_prefix  : " << actual_prefix << std::endl;
	std::cerr << "expected_prefix: " << expected_prefix << std::endl;
	
	CPPUNIT_ASSERT_EQUAL( expected_prefix, actual_prefix );

	std::cerr << "past assert" << std::endl;

	testStringSuite->nextTestString();
    }
}

void HistoryTrackerTest::testGetToken()
{
    std::cerr << "HistoryTrackerTest::testGetToken()" << std::endl;

    while (testStringSuite->hasMoreTestStrings()) {
	HistoryTracker hT(profile);
	hT.update(testStringSuite->currentTestString()->getstr());

	assert(testStringSuite->currentTestString() != 0);
	std::cerr << "pastStream: " << hT.getPastStream() << std::endl;
        
        const int TOKENS = testStringSuite->currentTestString()->tokencount();
        for (int i = 0; i < TOKENS; i++) {

            std::string actual_token   = hT.getToken(i);
            std::string expected_token = testStringSuite->currentTestString()->token(TOKENS - i - 1);
            
            std::cerr << "expected_token: " << expected_token << std::endl;
            std::cerr << "actual_token  : " << actual_token << std::endl;
	
            CPPUNIT_ASSERT_EQUAL( expected_token, actual_token );
        }

        testStringSuite->nextTestString();
    }
}


void HistoryTrackerTest::testGetFutureStream()
{}

void HistoryTrackerTest::testGetPastStream()
{
    std::cerr << "HistoryTrackerTest::testGetPastBuffer()" << std::endl;

    while (testStringSuite->hasMoreTestStrings()) {
	HistoryTracker hT(profile);
	std::string str = testStringSuite->currentTestString()->getstr();
	std::string partial_str;
        for (int i = 0; i < str.size(); i++) {
	    std::string strchar;
	    strchar.push_back(str[i]);
	    hT.update(strchar);
	    partial_str.push_back(str[i]);

            CPPUNIT_ASSERT_EQUAL( partial_str, hT.getPastStream() );
        }

        testStringSuite->nextTestString();
    }
}

void HistoryTrackerTest::testToString()
{}

void HistoryTrackerTest::testGetMaxBufferSize()
{}

void HistoryTrackerTest::testSetMaxBufferSize()
{}

void HistoryTrackerTest::testContextChange()
{
    HistoryTracker* historyTracker = new HistoryTracker(profile);

    const std::string line   = "foo bar foobar, foo   bar! Foobar foo bar... foobar. ";
    const std::string change = "00010001000000110001110001100000010001000111100000011";
    // See TODO in HistoryTracker.cpp
    // const std::string change = "00010001000000100001000001000000010001000100000000010";

    for (int i = 0; i < line.size(); i++) {
	std::string temp;
	temp.push_back(line[i]);
	historyTracker->update (temp);

	bool expected = (change[i] == '0' ? false : true);
	
	std::cerr << "contextChange: " << expected
		  << " - context: " << historyTracker->getPastStream() << '|' << std::endl;
	CPPUNIT_ASSERT_EQUAL(expected, historyTracker->contextChange());
    }

    delete historyTracker;
}

void HistoryTrackerTest::testCumulativeContextChange()
{
    HistoryTracker* historyTracker = new HistoryTracker(profile);

    const char* TRUE = "true";
    const char* FALSE = "false";
    const char* dataSuite[] = {
	"f", FALSE,
	"o", FALSE,
	"o", FALSE,
	" ", TRUE,
	"ba", FALSE,
	"r", FALSE,
	" ", TRUE,
	"foo", FALSE,
	"bar ", TRUE,
	"foo ", TRUE,
	"b", FALSE,
	"ar ", TRUE,
	"foobar ", TRUE,
	0, 0
    };
    
    int i = 0;
    while (dataSuite[i] != 0 && dataSuite[i+1] != 0) {
	historyTracker->update (dataSuite[i]);

	bool expected = (dataSuite[i+1] == FALSE ? false : true);

	std::cerr << "cumulativeContextChange: " << expected
		  << " - context: " << historyTracker->getPastStream() << '|' << std::endl;
	CPPUNIT_ASSERT_EQUAL(expected, historyTracker->contextChange());

	i += 2;
    }

    delete historyTracker;
}
