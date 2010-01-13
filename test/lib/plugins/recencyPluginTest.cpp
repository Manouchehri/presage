
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


#include "recencyPluginTest.h"
#include "../common/stringstreamPresageCallback.h"

#include "core/pluginRegistry.h"
#include <math.h>  // for exp()

CPPUNIT_TEST_SUITE_REGISTRATION( RecencyPluginTest );

const int RecencyPluginTest::SIZE = 20;
const char* RecencyPluginTest::LOGGER = "Presage.Plugins.RecencyPlugin.LOGGER";
const char* RecencyPluginTest::LAMBDA = "Presage.Plugins.RecencyPlugin.LAMBDA";
const char* RecencyPluginTest::CUTOFF = "Presage.Plugins.RecencyPlugin.CUTOFF_THRESHOLD";
const char* RecencyPluginTest::N_0    = "Presage.Plugins.RecencyPlugin.N_0";

void RecencyPluginTest::setUp()
{
    //std::cerr << "RecencyPluginTest::setUp()" << std::endl;

    config = new Configuration();
    // set context tracker config variables
    config->set(Variable("Presage.ContextTracker.LOGGER"), Value("ERROR"));
    config->set(Variable("Presage.ContextTracker.SLIDING_WINDOW_SIZE"), Value("80"));
    // set plugin registry config variables
    config->set(Variable("Presage.PluginRegistry.LOGGER"), Value("ERROR"));
    config->set(Variable("Presage.PluginRegistry.PLUGINS"), Value(""));
    // set recency plugin config variables
    config->set(LOGGER, "ALL");
    config->set(LAMBDA, "1");
    config->set(N_0,    "1");
    config->set(CUTOFF, "20");

    pluginRegistry = new PluginRegistry(config);
    stream = new std::stringstream();
    callback = new StringstreamPresageCallback(*stream);
    ct = new ContextTracker(config, pluginRegistry, callback);
}

void RecencyPluginTest::tearDown()
{
    //std::cerr << "RecencyPluginTest::tearDown()" << std::endl;
    
    delete ct;
    delete callback;
    delete stream;
    delete pluginRegistry;
    delete config;
}

void RecencyPluginTest::testMaxPartialPredictionSize()
{
    std::cerr << "RecencyPluginTest::testMaxPartialPredictionSize()" << std::endl;

    RecencyPlugin* plugin = new RecencyPlugin(config, ct);

    *stream << "foo foobar foobaz foo";

    for (size_t i = 1; i <= 3; i++) {
        Prediction prediction = plugin->predict(i, 0);
        CPPUNIT_ASSERT_EQUAL(i, prediction.size());
        std::cout << prediction << std::endl;
    }

    delete plugin;
}

void RecencyPluginTest::testCutoffThreshold()
{
    std::cerr << "RecencyPluginTest::testCutoffThreshold()" << std::endl;

    *stream << "foo bar foobar baz f";

    {
	config->set(CUTOFF, "0");
	Prediction expected;
	RecencyPlugin plugin(config, ct);
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE, 0));
    }

    {
	config->set(CUTOFF, "1");
	Prediction expected;
	RecencyPlugin plugin(config, ct);
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE, 0));
    }

    {
	config->set(CUTOFF, "2");
	Prediction expected;
	RecencyPlugin plugin(config, ct);
	expected.addSuggestion(Suggestion("foobar", 1.0 * exp(-1.0 * 1))); // foobar is second token (offset 1)
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE, 0));
    }

    {
	config->set(CUTOFF, "3");
	Prediction expected;
	RecencyPlugin plugin(config, ct);
	expected.addSuggestion(Suggestion("foobar", 1.0 * exp(-1.0 * 1))); // foobar is second token (offset 1)
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE, 0));
    }

    {
	config->set(CUTOFF, "4");
	Prediction expected;
	RecencyPlugin plugin(config, ct);
	expected.addSuggestion(Suggestion("foobar", 1.0 * exp(-1.0 * 1))); // foobar is second token (offset 1)
	expected.addSuggestion(Suggestion("foo",    1.0 * exp(-1.0 * 3))); // skip bar, foo is fourth token (offset 3)
	CPPUNIT_ASSERT_EQUAL(expected, plugin.predict(SIZE, 0));
    }

}
