
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


#include "dejavuPluginTest.h"
#include <math.h>  // for exp()

CPPUNIT_TEST_SUITE_REGISTRATION( DejavuPluginTest );

const int DejavuPluginTest::SIZE = 20;
const char* DejavuPluginTest::LOGGER  = "Presage.Plugins.DejavuPlugin.LOGGER";
const char* DejavuPluginTest::TRIGGER = "Presage.Plugins.DejavuPlugin.TRIGGER";
const char* DejavuPluginTest::MEMORY  = "Presage.Plugins.DejavuPlugin.MEMORY";

void DejavuPluginTest::setUp()
{
    config = new Configuration();
    // set context tracker config variables
    config->set(Variable("Presage.ContextTracker.LOGGER"), Value("ERROR"));
    config->set(Variable("Presage.ContextTracker.MAX_BUFFER_SIZE"), Value("1024"));
    // set dejavu plugin config variables
    config->set(LOGGER,  "ALL");
    config->set(TRIGGER, "3");
    config->set(MEMORY,  "memory.txt");

    ct = new ContextTracker(config);
}

void DejavuPluginTest::tearDown()
{
    delete ct;
    delete config;
}

void DejavuPluginTest::testPredict()
{
    ct->update("polly wants a cracker ");

    DejavuPlugin plugin(config, ct);
    
    {
	ct->update("polly ");
	Prediction expected;
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE));
    }

    {
	ct->update("wants ");
	Prediction expected;
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE));
    }

    {
	ct->update("a ");
	Prediction expected;
	expected.addSuggestion(Suggestion("cracker", 1.0));
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE));
    }

    ct->update("soda ");

    {
	ct->update("polly ");
	Prediction expected;
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE));
    }

    {
	ct->update("wants ");
	Prediction expected;
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE));
    }

    {
	ct->update("a ");
	Prediction expected;
	expected.addSuggestion(Suggestion("cracker", 1.0));
	expected.addSuggestion(Suggestion("soda",    1.0));
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE));
    }
}
