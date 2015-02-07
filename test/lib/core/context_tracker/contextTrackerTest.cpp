
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


#include "contextTrackerTest.h"
#include "../../common/stringstreamPresageCallback.h"

#include <string>
#include <assert.h>

CPPUNIT_TEST_SUITE_REGISTRATION( ContextTrackerTest );

void ContextTrackerTest::setUp()
{
    testStringSuite = new TestStringSuite();

    configuration = new Configuration();
    configuration->insert (PredictorRegistry::LOGGER, "ERROR");
    configuration->insert (PredictorRegistry::PREDICTORS, "");
    configuration->insert (ContextTracker::LOGGER, "ERROR");
    configuration->insert (ContextTracker::SLIDING_WINDOW_SIZE, "80");
    configuration->insert (ContextTracker::LOWERCASE_MODE, "no");
    configuration->insert (ContextTracker::ONLINE_LEARNING, "yes");

    predictorRegistry = new PredictorRegistry(configuration);
}

void ContextTrackerTest::tearDown()
{
    delete testStringSuite;

    delete predictorRegistry;
    delete configuration;
}


void ContextTrackerTest::testConstructor()
{}

void ContextTrackerTest::testGetPrefix()
{
    while (testStringSuite->hasMoreTestStrings()) {
	std::stringstream buffer;
	StringstreamPresageCallback callback(buffer);
	ContextTracker hT(configuration, predictorRegistry, &callback);
	buffer << testStringSuite->currentTestString()->getstr();

	assert(testStringSuite->currentTestString() != 0);
	//std::cerr << "Test getting prefix from string: "
	//	  << testStringSuite->currentTestString()->getstr() 
	//	  << std::endl;

	std::string actual_prefix   = hT.getPrefix();
	std::string expected_prefix = testStringSuite->currentTestString()->token(testStringSuite->currentTestString()->tokencount() - 1);

	//std::cerr << "actual_prefix  : " << actual_prefix << std::endl;
	//std::cerr << "expected_prefix: " << expected_prefix << std::endl;
	
	CPPUNIT_ASSERT_EQUAL( expected_prefix, actual_prefix );

	//std::cerr << "past assert" << std::endl;

	testStringSuite->nextTestString();
    }
}

void ContextTrackerTest::testGetToken()
{
    while (testStringSuite->hasMoreTestStrings()) {
	std::stringstream buffer;
	StringstreamPresageCallback callback(buffer);
	ContextTracker hT(configuration, predictorRegistry, &callback);
	buffer << testStringSuite->currentTestString()->getstr();

	assert(testStringSuite->currentTestString() != 0);
	//std::cerr << "pastStream: " << hT.getPastStream() << std::endl;
        
        const int TOKENS = testStringSuite->currentTestString()->tokencount();
        for (int i = 0; i < TOKENS; i++) {

            std::string actual_token   = hT.getToken(i);
            std::string expected_token = testStringSuite->currentTestString()->token(TOKENS - i - 1);
            
            //std::cerr << "expected_token: " << expected_token << std::endl;
            //std::cerr << "actual_token  : " << actual_token << std::endl;
	
            CPPUNIT_ASSERT_EQUAL( expected_token, actual_token );
        }

        testStringSuite->nextTestString();
    }
}


void ContextTrackerTest::testGetFutureStream()
{}

void ContextTrackerTest::testGetPastStream()
{
    while (testStringSuite->hasMoreTestStrings()) {
	std::stringstream buffer;
	StringstreamPresageCallback callback(buffer);
	ContextTracker hT(configuration, predictorRegistry, &callback);
	std::string str = testStringSuite->currentTestString()->getstr();
	std::string partial_str;
        for (size_t i = 0; i < str.size(); i++) {
	    std::string strchar;
	    strchar.push_back(str[i]);
	    buffer << strchar;
	    partial_str.push_back(str[i]);

            CPPUNIT_ASSERT_EQUAL( partial_str, hT.getPastStream() );
        }

        testStringSuite->nextTestString();
    }
}

void ContextTrackerTest::testToString()
{}

void ContextTrackerTest::testContextChange()
{
    std::stringstream buffer;
    PresageCallback* callback = new StringstreamPresageCallback(buffer);
    ContextTracker* contextTracker = new ContextTracker(configuration,
							predictorRegistry, 
							callback);

    const std::string line   = "foo bar foobar, foo   bar! Foobar foo bar... foobar. ";
    const std::string change = "10010001000000100001000001000000010001000100000000010";

    for (size_t i = 0; i < line.size(); i++) {
	std::string temp;
	temp.push_back(line[i]);
	buffer << temp;

	bool expected = (change[i] == '0' ? false : true);
	
	//std::cerr << "contextChange: " << expected
	//	  << " - context: " << contextTracker->getPastStream() << '|' << std::endl;

	std::stringstream ss;
	ss << "Error detected at: " << line.substr(0, i) << '|';
	CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), expected, contextTracker->contextChange());
	contextTracker->update();
    }

    delete contextTracker;
    delete callback;
}

void ContextTrackerTest::testCumulativeContextChange()
{
    std::stringstream buffer;
    PresageCallback* callback = new StringstreamPresageCallback(buffer);
    ContextTracker* contextTracker = new ContextTracker(configuration, predictorRegistry, callback);

    const char* TRUE = "true";
    const char* FALSE = "false";
    const char* dataSuite[] = {
	"f", TRUE,         // f|
	"o", FALSE,        // fo|
	"o", FALSE,        // foo|
	" ", TRUE,         // foo |
	"ba", FALSE,       // foo ba|
	"r", FALSE,        // foo bar|
	" ", TRUE,         // foo bar |
	"foo", FALSE,      // foo bar foo|
	"bar ", TRUE,      // foo bar foobar |
	"foo ", TRUE,      // foo bar foobar foo |
	"b", FALSE,        // foo bar foobar foo b|
	"ar ", TRUE,       // foo bar foobar foo bar |
	"foobar ", TRUE,   // foo bar foobar foo bar foobar |
	0, 0
    };
    
    int i = 0;
    while (dataSuite[i] != 0 && dataSuite[i+1] != 0) {
	buffer << dataSuite[i];

	bool expected = (dataSuite[i+1] == FALSE ? false : true);

	std::stringstream ss;
	ss << "cumulativeContextChange: " << expected
	   << " - context: " << contextTracker->getPastStream() << '|' << std::endl;
	CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), expected, contextTracker->contextChange());
	contextTracker->update();

	i += 2;
    }

    delete contextTracker;
}
