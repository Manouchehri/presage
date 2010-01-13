
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
#include "../common/stringstreamPresageCallback.h"

#include "core/pluginRegistry.h"
#include <math.h>  // for exp()
#include <cstdio>  // for remove()

CPPUNIT_TEST_SUITE_REGISTRATION( DejavuPluginTest );

const int DejavuPluginTest::SIZE = 20;
const char* DejavuPluginTest::LOGGER  = "Presage.Plugins.DejavuPlugin.LOGGER";
const char* DejavuPluginTest::TRIGGER = "Presage.Plugins.DejavuPlugin.TRIGGER";
const char* DejavuPluginTest::MEMORY  = "Presage.Plugins.DejavuPlugin.MEMORY";
const char* DejavuPluginTest::MEMORY_FILENAME = "memory.txt";

void DejavuPluginTest::setUp()
{
    config = new Configuration();
    // set context tracker config variables
    config->set(Variable("Presage.ContextTracker.LOGGER"), Value("ERROR"));
    config->set(Variable("Presage.ContextTracker.SLIDING_WINDOW_SIZE"), Value("80"));
    // set plugin registry config variables
    config->set(Variable("Presage.PluginRegistry.LOGGER"), Value("ERROR"));
    config->set(Variable("Presage.PluginRegistry.PLUGINS"), Value("DejavuPlugin"));
    // set dejavu plugin config variables
    config->set(LOGGER,  "ALL");
    config->set(TRIGGER, "3");
    config->set(MEMORY,  MEMORY_FILENAME);

    pluginRegistry = new PluginRegistry(config);
    stream = new std::stringstream();
    callback = new StringstreamPresageCallback(*stream);
    ct = new ContextTracker(config, pluginRegistry, callback);

    remove(MEMORY_FILENAME);
}

void DejavuPluginTest::tearDown()
{
    delete ct;
    delete callback;
    delete stream;
    delete pluginRegistry;
    delete config;

    remove(MEMORY_FILENAME);
}

void DejavuPluginTest::testPredict()
{
    *stream << "polly wants a cracker ";
    ct->update();

    // get pointer to dejavu plugin
    Plugin* plugin = pluginRegistry->iterator().next();
    
    {
	*stream << "polly ";
	Prediction expected;
	CPPUNIT_ASSERT_EQUAL(expected, plugin->predict(SIZE, 0));
	ct->update();
    }

    {
	*stream << "wants ";
	Prediction expected;
	CPPUNIT_ASSERT_EQUAL(expected, plugin->predict(SIZE, 0));
	ct->update();
    }

    {
	*stream << "a ";
	Prediction expected;
	expected.addSuggestion(Suggestion("cracker", 1.0));
	CPPUNIT_ASSERT_EQUAL(expected, plugin->predict(SIZE, 0));
	ct->update();
    }

    *stream << "soda ";
    ct->update();

    {
	*stream << "polly ";
	Prediction expected;
	CPPUNIT_ASSERT_EQUAL(expected, plugin->predict(SIZE, 0));
	ct->update();
    }

    {
	*stream << "wants ";
	Prediction expected;
	CPPUNIT_ASSERT_EQUAL(expected, plugin->predict(SIZE, 0));
	ct->update();
    }

    {
	*stream << "a ";
	Prediction expected;
	expected.addSuggestion(Suggestion("cracker", 1.0));
	expected.addSuggestion(Suggestion("soda",    1.0));
	CPPUNIT_ASSERT_EQUAL(expected, plugin->predict(SIZE, 0));
	ct->update();
    }

    *stream << "cake ";
    ct->update();

    {
	*stream << "polly ";
	Prediction expected;
	CPPUNIT_ASSERT_EQUAL(expected, plugin->predict(SIZE, 0));
	ct->update();
    }

    {
	*stream << "wants ";
	Prediction expected;
	CPPUNIT_ASSERT_EQUAL(expected, plugin->predict(SIZE, 0));
	ct->update();
    }

    {
	*stream << "a ";
	Prediction expected;
	expected.addSuggestion(Suggestion("cake",    1.0));
	expected.addSuggestion(Suggestion("cracker", 1.0));
	expected.addSuggestion(Suggestion("soda",    1.0));
	CPPUNIT_ASSERT_EQUAL(expected, plugin->predict(SIZE, 0));
	ct->update();
    }
}
