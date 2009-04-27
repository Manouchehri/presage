
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


#include "newSmoothedNgramPluginTest.h"

#include "core/pluginRegistry.h"

#include <cstdio>  // for remove()

CPPUNIT_TEST_SUITE_REGISTRATION( NewSmoothedNgramPluginTest );

const char* NewSmoothedNgramPluginTest::DATABASE = "database.db";
const int   NewSmoothedNgramPluginTest::SIZE     = 20;

void NewSmoothedNgramPluginTest::setUp()
{
    remove(DATABASE);

    // prepare database
    SqliteDatabaseConnector db(DATABASE);
    db.createUnigramTable();
    db.createBigramTable();
    db.createTrigramTable();

    config = new Configuration();
    // set context tracker config variables
    config->set(Variable("Presage.ContextTracker.LOGGER"), Value("ERROR"));
    config->set(Variable("Presage.ContextTracker.MAX_BUFFER_SIZE"), Value("1024"));
    // set plugin registry config variables
    config->set(Variable("Presage.PluginRegistry.LOGGER"), Value("ERROR"));
    config->set(Variable("Presage.PluginRegistry.PLUGINS"), Value("SmoothedNgramPlugin"));
    // set plugin config variables
    config->set(Variable("Presage.Plugins.SmoothedNgramPlugin.LOGGER"), "ALL");
    config->set(Variable("Presage.Plugins.SmoothedNgramPlugin.DELTAS"), "0.001 0.01 0.889");
    config->set(Variable("Presage.Plugins.SmoothedNgramPlugin.DBFILENAME"), DATABASE);
    config->set(Variable("Presage.Plugins.SmoothedNgramPlugin.LEARN"), "true");
    config->set(Variable("Presage.Plugins.SmoothedNgramPlugin.DatabaseConnector.LOGGER"), "ERROR");

    pluginRegistry = new PluginRegistry(config);

    ct = new ContextTracker(config, pluginRegistry);
}

void NewSmoothedNgramPluginTest::tearDown()
{
    delete ct;
    delete pluginRegistry;
    delete config;

    remove(DATABASE);
}

void NewSmoothedNgramPluginTest::testLearning()
{
    // get pointer to dejavu plugin
    Plugin* plugin = pluginRegistry->iterator().next();

    {
	ct->update("f");
	Prediction actual = plugin->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), actual.size());
    }

    {
	ct->update("o");
	Prediction actual = plugin->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), actual.size());
    }

    {
	ct->update("o ");
	Prediction actual = plugin->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), actual.size());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), actual.getSuggestion(0).getWord());
    }

    {
	ct->update("bar");
	Prediction actual = plugin->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), actual.size());
    }

    {
	ct->update(" ");
	Prediction actual = plugin->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), actual.size());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), actual.getSuggestion(0).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("bar"), actual.getSuggestion(1).getWord());
    }

    {
	ct->update("foobar ");
	Prediction actual = plugin->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), actual.size());
	CPPUNIT_ASSERT_EQUAL(std::string("foobar"), actual.getSuggestion(0).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), actual.getSuggestion(1).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("bar"), actual.getSuggestion(2).getWord());
    }

    {
	ct->update("f");
	Prediction actual = plugin->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), actual.size());
	CPPUNIT_ASSERT_EQUAL(std::string("foobar"), actual.getSuggestion(0).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), actual.getSuggestion(1).getWord());
    }
}
