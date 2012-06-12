
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


#include "predictorRegistryTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( PredictorRegistryTest );

void PredictorRegistryTest::setUp()
{
    config = new Configuration;
    config->insert ("Presage.PredictorRegistry.LOGGER", "ERROR");
    config->insert ("Presage.PredictorRegistry.PREDICTORS", "DummyPredictor1 DummyPredictor2 DummyPredictor3 DummyPredictor4");
    config->insert ("Presage.Predictors.DummyPredictor1.PREDICTOR", "DummyPredictor");
    config->insert ("Presage.Predictors.DummyPredictor2.PREDICTOR", "DummyPredictor");
    config->insert ("Presage.Predictors.DummyPredictor3.PREDICTOR", "DummyPredictor");
    config->insert ("Presage.Predictors.DummyPredictor4.PREDICTOR", "DummyPredictor");

    registry = new PredictorRegistry(config);
}

void PredictorRegistryTest::tearDown()
{
    delete registry;
    delete config;
}

void PredictorRegistryTest::testHasNext()
{
    // test that no predictors are init'd if tracker is null
    PredictorRegistry::Iterator it = registry->iterator();

    int count = 0;
    while (it.hasNext()) {
	count++;
	it.next();
    }

    CPPUNIT_ASSERT_EQUAL(0, count);

    // test that predictors are init'd if tracker is not null
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

void PredictorRegistryTest::testNext()
{
    ContextTracker* pointer = static_cast<ContextTracker*>((void*)0xdeadbeef);
    registry->setContextTracker(pointer);

    PredictorRegistry::Iterator it = registry->iterator();
    Predictor* predictor = 0;

    while (it.hasNext()) {
	predictor = it.next();
    }

    // since we've iterated till the end of the predictors list, predictor
    // is now pointing to the DummyPredictor, so let's test we got the
    // dummy prediction back
    Prediction prediction = predictor->predict(20, 0);

    CPPUNIT_ASSERT(predictor != 0);
    size_t expected_size = 18;
    CPPUNIT_ASSERT_EQUAL(expected_size, prediction.size());
    CPPUNIT_ASSERT_EQUAL(Suggestion("foo1", 0.99), prediction.getSuggestion(0));
    CPPUNIT_ASSERT_EQUAL(Suggestion("foobar6", 0.74), prediction.getSuggestion(17));
}
