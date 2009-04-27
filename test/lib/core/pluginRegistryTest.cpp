
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


#include "pluginRegistryTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( PluginRegistryTest );

void PluginRegistryTest::setUp()
{
    config = new Configuration;
    config->set(Variable("Presage.PluginRegistry.LOGGER"), "ALL");
//    config->set(Variable("Presage.PluginRegistry.PLUGINS"), "SmoothedNgramPlugin RecencyPlugin AbbreviationExpansionPlugin DummyPlugin");
    config->set(Variable("Presage.PluginRegistry.PLUGINS"), "DummyPlugin DummyPlugin DummyPlugin DummyPlugin");

    registry = new PluginRegistry(config);
}

void PluginRegistryTest::tearDown()
{
    delete registry;
    delete config;
}

void PluginRegistryTest::testHasNext()
{
    // test that no plugins are init'd if tracker is null
    PluginRegistry::Iterator it = registry->iterator();

    int count = 0;
    while (it.hasNext()) {
	count++;
	it.next();
    }

    CPPUNIT_ASSERT_EQUAL(0, count);

    // test that plugins are init'd if tracker is not null
    ContextTracker* pointer = static_cast<ContextTracker*>((void*)0xdeadbeef);
    registry->setContextTracker(pointer);
    
    it = registry->iterator();

    count = 0;
    while (it.hasNext()) {
	count++;
	it.next();
    }

    CPPUNIT_ASSERT_EQUAL(4, count);
}

void PluginRegistryTest::testNext()
{
    ContextTracker* pointer = static_cast<ContextTracker*>((void*)0xdeadbeef);
    registry->setContextTracker(pointer);

    PluginRegistry::Iterator it = registry->iterator();
    Plugin* plugin = 0;

    while (it.hasNext()) {
	std::cerr << "entered testNext() hasNext() while" << std::endl;
	plugin = it.next();
    }

    std::cerr << "plugin: " << (void*) plugin << std::endl;

    // since we've iterated till the end of the plugins list, plugin
    // is now pointing to the DummyPlugin, so let's test we got the
    // dummy prediction back
    Prediction prediction = plugin->predict(20, 0);

    CPPUNIT_ASSERT(plugin != 0);
    size_t expected_size = 18;
    CPPUNIT_ASSERT_EQUAL(expected_size, prediction.size());
    CPPUNIT_ASSERT_EQUAL(Suggestion("foo1", 0.99), prediction.getSuggestion(0));
    CPPUNIT_ASSERT_EQUAL(Suggestion("foobar6", 0.74), prediction.getSuggestion(17));
}
