
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


#include "recencyPredictorTest.h"
#include "../common/stringstreamPresageCallback.h"

#include "core/predictorRegistry.h"
#include <math.h>  // for exp()

CPPUNIT_TEST_SUITE_REGISTRATION( RecencyPredictorTest );

const int RecencyPredictorTest::SIZE = 20;
const char* RecencyPredictorTest::NAME      = "RecencyPredictor";
const char* RecencyPredictorTest::PREDICTOR = "Presage.Predictors.RecencyPredictor.PREDICTOR";
const char* RecencyPredictorTest::LOGGER    = "Presage.Predictors.RecencyPredictor.LOGGER";
const char* RecencyPredictorTest::LAMBDA    = "Presage.Predictors.RecencyPredictor.LAMBDA";
const char* RecencyPredictorTest::CUTOFF    = "Presage.Predictors.RecencyPredictor.CUTOFF_THRESHOLD";
const char* RecencyPredictorTest::N_0       = "Presage.Predictors.RecencyPredictor.N_0";

void RecencyPredictorTest::setUp()
{
    config = new Configuration();
    // set context tracker config variables
    config->insert ("Presage.ContextTracker.LOGGER", "ERROR");
    config->insert ("Presage.ContextTracker.SLIDING_WINDOW_SIZE", "80");
    config->insert ("Presage.ContextTracker.LOWERCASE_MODE", "no");
    config->insert ("Presage.ContextTracker.ONLINE_LEARNING", "yes");

    // set predictor registry config variables
    config->insert ("Presage.PredictorRegistry.LOGGER", "ERROR");
    config->insert ("Presage.PredictorRegistry.PREDICTORS", "");
    // set recency predictor config variables
    config->insert (PREDICTOR, NAME);
    config->insert (LOGGER, "ERROR");
    config->insert (LAMBDA, "1");
    config->insert (N_0,    "1");
    config->insert (CUTOFF, "20");

    predictorRegistry = new PredictorRegistry(config);
    stream = new std::stringstream();
    callback = new StringstreamPresageCallback(*stream);
    ct = new ContextTracker(config, predictorRegistry, callback);
}

void RecencyPredictorTest::tearDown()
{
    delete ct;
    delete callback;
    delete stream;
    delete predictorRegistry;
    delete config;
}

void RecencyPredictorTest::testMaxPartialPredictionSize()
{
    RecencyPredictor* predictor = new RecencyPredictor(config, ct, NAME);

    *stream << "foo foobar foobaz foo";

    for (size_t i = 1; i <= 3; i++) {
        Prediction prediction = predictor->predict(i, 0);
        CPPUNIT_ASSERT_EQUAL(i, prediction.size());
    }

    delete predictor;
}

void RecencyPredictorTest::testCutoffThreshold()
{
    *stream << "foo bar foobar baz f";

    {
	config->insert (CUTOFF, "0");
	Prediction expected;
	RecencyPredictor predictor(config, ct, NAME);
	CPPUNIT_ASSERT_EQUAL(expected, predictor.predict(SIZE, 0));
    }

    {
	config->insert (CUTOFF, "1");
	Prediction expected;
	RecencyPredictor predictor(config, ct, NAME);
	CPPUNIT_ASSERT_EQUAL(expected, predictor.predict(SIZE, 0));
    }

    {
	config->insert (CUTOFF, "2");
	Prediction expected;
	RecencyPredictor predictor(config, ct, NAME);
	expected.addSuggestion(Suggestion("foobar", 1.0 * exp(-1.0 * 1))); // foobar is second token (offset 1)
	CPPUNIT_ASSERT_EQUAL(expected, predictor.predict(SIZE, 0));
    }

    {
	config->insert (CUTOFF, "3");
	Prediction expected;
	RecencyPredictor predictor(config, ct, NAME);
	expected.addSuggestion(Suggestion("foobar", 1.0 * exp(-1.0 * 1))); // foobar is second token (offset 1)
	CPPUNIT_ASSERT_EQUAL(expected, predictor.predict(SIZE, 0));
    }

    {
	config->insert (CUTOFF, "4");
	Prediction expected;
	RecencyPredictor predictor(config, ct, NAME);
	expected.addSuggestion(Suggestion("foobar", 1.0 * exp(-1.0 * 1))); // foobar is second token (offset 1)
	expected.addSuggestion(Suggestion("foo",    1.0 * exp(-1.0 * 3))); // skip bar, foo is fourth token (offset 3)
	CPPUNIT_ASSERT_EQUAL(expected, predictor.predict(SIZE, 0));
    }

}
void RecencyPredictorTest::testFilter()
{
    *stream << "foo bar foobar baz f";

    {
	const char* filter[] = {"oob", 0};

	Prediction expected;
	expected.addSuggestion(Suggestion("foobar", 1.0 * exp(-1.0 * 1))); // foobar is second token (offset 1)

	RecencyPredictor predictor(config, ct, NAME);
	CPPUNIT_ASSERT_EQUAL(expected, predictor.predict(SIZE, filter));
    }

    {
	const char* filter[] = {"oo", 0};

	Prediction expected;
	expected.addSuggestion(Suggestion("foobar", 1.0 * exp(-1.0 * 1))); // foobar is second token (offset 1)
	expected.addSuggestion(Suggestion("foo",    1.0 * exp(-1.0 * 3))); // skip bar, foo is fourth token (offset 3)

	RecencyPredictor predictor(config, ct, NAME);
	CPPUNIT_ASSERT_EQUAL(expected, predictor.predict(SIZE, filter));
    }

    {
	const char* filter[] = {"ar", 0};

	Prediction expected;

	RecencyPredictor predictor(config, ct, NAME);
	CPPUNIT_ASSERT_EQUAL(expected, predictor.predict(SIZE, filter));
    }

    {
	const char** filter = 0;

	Prediction expected;
	expected.addSuggestion(Suggestion("foobar", 1.0 * exp(-1.0 * 1))); // foobar is second token (offset 1)
	expected.addSuggestion(Suggestion("foo",    1.0 * exp(-1.0 * 3))); // skip bar, foo is fourth token (offset 3)

	RecencyPredictor predictor(config, ct, NAME);
	CPPUNIT_ASSERT_EQUAL(expected, predictor.predict(SIZE, filter));
    }

}
