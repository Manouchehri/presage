
/*********************************************************
 *  Soothsayer, an extensible predictive text entry system
 *  ------------------------------------------------------
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

CPPUNIT_TEST_SUITE_REGISTRATION( RecencyPluginTest );

void RecencyPluginTest::setUp()
{
    //std::cerr << "RecencyPluginTest::setUp()" << std::endl;

    config = new Configuration();
    config->set(Variable("Soothsayer.ContextTracker.LOGGER"), Value("ERROR"));
    config->set(Variable("Soothsayer.ContextTracker.MAX_BUFFER_SIZE"), Value("1024"));

    ct = new ContextTracker(config);
}

void RecencyPluginTest::tearDown()
{
    //std::cerr << "RecencyPluginTest::tearDown()" << std::endl;

    delete ct;
    delete config;
}

void RecencyPluginTest::testMaxPartialPredictionSize()
{
    //std::cerr << "RecencyPluginTest::testMaxPartialPredictionSize()" << std::endl;

    RecencyPlugin* plugin = new RecencyPlugin(config, ct);

    ct->update("foo foobar foobaz foo");

    for (int i = 1; i <= 3; i++) {
        Prediction prediction = plugin->predict(i);
        CPPUNIT_ASSERT_EQUAL(i, prediction.size());
        std::cout << prediction << std::endl;
    }

    delete plugin;
}
