
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


#include "dejavuPredictorTest.h"
#include "../common/stringstreamPresageCallback.h"

#include "core/predictorRegistry.h"
#include <math.h>  // for exp()
#include <cstdio>  // for remove()

CPPUNIT_TEST_SUITE_REGISTRATION( DejavuPredictorTest );

const int DejavuPredictorTest::SIZE = 20;
const char* DejavuPredictorTest::NAME       = "DejavuPredictor";
const char* DejavuPredictorTest::PREDICTOR  = "Presage.Predictors.DejavuPredictor.PREDICTOR";
const char* DejavuPredictorTest::LOGGER     = "Presage.Predictors.DejavuPredictor.LOGGER";
const char* DejavuPredictorTest::TRIGGER    = "Presage.Predictors.DejavuPredictor.TRIGGER";
const char* DejavuPredictorTest::MEMORY     = "Presage.Predictors.DejavuPredictor.MEMORY";
const char* DejavuPredictorTest::MEMORY_FILENAME = "memory.txt";

void DejavuPredictorTest::setUp()
{
    config = new Configuration();
    // set context tracker config variables
    config->insert ("Presage.ContextTracker.LOGGER", "ERROR");
    config->insert ("Presage.ContextTracker.SLIDING_WINDOW_SIZE", "80");
    config->insert ("Presage.ContextTracker.LOWERCASE_MODE", "no");
    config->insert ("Presage.ContextTracker.ONLINE_LEARNING", "yes");
    // set predictor registry config variables
    config->insert ("Presage.PredictorRegistry.LOGGER", "ERROR");
    config->insert ("Presage.PredictorRegistry.PREDICTORS", "DejavuPredictor");
    // set dejavu predictor config variables
    config->insert (PREDICTOR, NAME);
    config->insert (LOGGER, "ERROR");
    config->insert (TRIGGER, "3");
    config->insert (MEMORY, MEMORY_FILENAME);

    predictorRegistry = new PredictorRegistry(config);
    stream = new std::stringstream();
    callback = new StringstreamPresageCallback(*stream);
    ct = new ContextTracker(config, predictorRegistry, callback);

    remove(MEMORY_FILENAME);
}

void DejavuPredictorTest::tearDown()
{
    delete ct;
    delete callback;
    delete stream;
    delete predictorRegistry;
    delete config;

    remove(MEMORY_FILENAME);
}

void DejavuPredictorTest::testPredict()
{
    *stream << "polly wants a cracker ";
    ct->update();

    // get pointer to dejavu predictor
    Predictor* predictor = predictorRegistry->iterator().next();
    
    {
        *stream << "polly ";
        Prediction expected;
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, 0));
        ct->update();
    }

    {
        *stream << "wants ";
        Prediction expected;
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, 0));
        ct->update();
    }

    {
        *stream << "a ";
        Prediction expected;
        expected.addSuggestion(Suggestion("cracker", 1.0));
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, 0));
        ct->update();
    }

    *stream << "soda ";
    ct->update();

    {
        *stream << "polly ";
        Prediction expected;
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, 0));
        ct->update();
    }

    {
        *stream << "wants ";
        Prediction expected;
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, 0));
        ct->update();
    }

    {
        *stream << "a ";
        Prediction expected;
        expected.addSuggestion(Suggestion("cracker", 1.0));
        expected.addSuggestion(Suggestion("soda",    1.0));
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, 0));
        ct->update();
    }

    *stream << "cake ";
    ct->update();

    {
        *stream << "polly ";
        Prediction expected;
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, 0));
        ct->update();
    }

    {
        *stream << "wants ";
        Prediction expected;
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, 0));
        ct->update();
    }

    {
        *stream << "a ";
        Prediction expected;
        expected.addSuggestion(Suggestion("cake",    1.0));
        expected.addSuggestion(Suggestion("cracker", 1.0));
        expected.addSuggestion(Suggestion("soda",    1.0));
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, 0));
        ct->update();
    }

    *stream << "crumble ";
    ct->update();

    {
        // test filter
        const char* filter[] = { "cra", "so", 0 };

        *stream << "polly wants a ";
        Prediction expected;
        expected.addSuggestion(Suggestion("cracker",    1.0));
        expected.addSuggestion(Suggestion("soda",    1.0));
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, filter));
        ct->update();
    }

    *stream << "break ";
    ct->update();

    {
        // test filter
        const char* filter[] = { "r", 0 };

        *stream << "polly wants a c";
        Prediction expected;
        expected.addSuggestion(Suggestion("cracker",    1.0));
        expected.addSuggestion(Suggestion("crumble",    1.0));
        CPPUNIT_ASSERT_EQUAL(expected, predictor->predict(SIZE, filter));
        ct->update();
    }

    *stream << "uddle ";
    ct->update();
}
