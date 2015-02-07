
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


#include "newSmoothedNgramPredictorTest.h"
#include "../common/stringstreamPresageCallback.h"

#include "core/predictorRegistry.h"

#include <cstdio>  // for remove()

CPPUNIT_TEST_SUITE_REGISTRATION( NewSmoothedNgramPredictorTest );

const char*  NewSmoothedNgramPredictorTest::DATABASE = "new_database.db";
const size_t NewSmoothedNgramPredictorTest::CARDINALITY = 3;
const bool   NewSmoothedNgramPredictorTest::READ_WRITE_MODE = true;

const int    NewSmoothedNgramPredictorTest::SIZE     = 20;

void NewSmoothedNgramPredictorTest::setUp()
{
    remove(DATABASE);

    // prepare database
    SqliteDatabaseConnector db(DATABASE, CARDINALITY, READ_WRITE_MODE);
    db.createUnigramTable();
    db.createBigramTable();
    db.createTrigramTable();

    config = new Configuration();
    // set context tracker config variables
    config->insert ("Presage.ContextTracker.LOGGER", "ERROR");
    config->insert ("Presage.ContextTracker.SLIDING_WINDOW_SIZE", "80");
    config->insert ("Presage.ContextTracker.LOWERCASE_MODE", "no");
    config->insert ("Presage.ContextTracker.ONLINE_LEARNING", "yes");

    // set predictor registry config variables
    config->insert ("Presage.PredictorRegistry.LOGGER", "ERROR");
    config->insert ("Presage.PredictorRegistry.PREDICTORS", "SmoothedNgramPredictor");
    // set predictor config variables
    config->insert ("Presage.Predictors.SmoothedNgramPredictor.PREDICTOR", "SmoothedNgramPredictor");
    config->insert ("Presage.Predictors.SmoothedNgramPredictor.LOGGER", "ERROR");
    config->insert ("Presage.Predictors.SmoothedNgramPredictor.DELTAS", "0.001 0.01 0.889");
    config->insert ("Presage.Predictors.SmoothedNgramPredictor.DBFILENAME", DATABASE);
    config->insert ("Presage.Predictors.SmoothedNgramPredictor.LEARN", "true");
    config->insert ("Presage.Predictors.SmoothedNgramPredictor.DatabaseConnector.LOGGER", "ERROR");

    predictorRegistry = new PredictorRegistry(config);
    stream = new std::stringstream();
    callback = new StringstreamPresageCallback(*stream);
    ct = new ContextTracker(config, predictorRegistry, callback);
}

void NewSmoothedNgramPredictorTest::tearDown()
{
    delete ct;
    delete callback;
    delete predictorRegistry;
    delete config;

    remove(DATABASE);
}

void NewSmoothedNgramPredictorTest::testOnlineLearning()
{
    // get pointer to predictor
    Predictor* predictor = predictorRegistry->iterator().next();

    {
	*stream << "f";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), actual.size());
    }

    {
	*stream << "o";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), actual.size());
    }

    {
	*stream << "o ";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), actual.size());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), actual.getSuggestion(0).getWord());
	ct->update();
    }

    {
	*stream << "bar";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
    }

    {
	*stream << " ";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), actual.size());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), actual.getSuggestion(0).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("bar"), actual.getSuggestion(1).getWord());
    }

    {
	*stream << "foobar ";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), actual.size());
	CPPUNIT_ASSERT_EQUAL(std::string("foobar"), actual.getSuggestion(0).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), actual.getSuggestion(1).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("bar"), actual.getSuggestion(2).getWord());
    }

    {
	*stream << "f";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), actual.size());
	CPPUNIT_ASSERT_EQUAL(std::string("foobar"), actual.getSuggestion(0).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), actual.getSuggestion(1).getWord());
    }
}

void NewSmoothedNgramPredictorTest::testOfflineLearning()
{
    // turns off online learning
    config->find("Presage.ContextTracker.ONLINE_LEARNING")->set_value("no");

    // get pointer to predictor
    Predictor* predictor = predictorRegistry->iterator().next();

    {
	*stream << "f";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), actual.size());
    }

    {
	*stream << "o";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), actual.size());
    }

    {
	*stream << "o ";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), actual.size());
    }

    {
	*stream << "bar";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
    }

    {
	*stream << " ";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), actual.size());
    }

    {
	*stream << "foobar ";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), actual.size());
    }

    {
	ct->learn(stream->str());
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), actual.size());
	CPPUNIT_ASSERT_EQUAL(std::string("foobar"), actual.getSuggestion(0).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), actual.getSuggestion(1).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("bar"), actual.getSuggestion(2).getWord());
    }

    {
	*stream << "f";
	ct->update();
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), actual.size());
	CPPUNIT_ASSERT_EQUAL(std::string("foobar"), actual.getSuggestion(0).getWord());
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), actual.getSuggestion(1).getWord());
    }
}

void NewSmoothedNgramPredictorTest::testFilter()
{
    // get pointer to predictor
    Predictor* predictor = predictorRegistry->iterator().next();

    std::vector<std::string> change;
    change.push_back("foo");
    change.push_back("bar");
    change.push_back("foobar");
    change.push_back("foz");
    change.push_back("baz");
    change.push_back("fozbaz");
    change.push_back("roo");
    change.push_back("rar");
    change.push_back("roobar");

    // Learn some context so that we have data to create non-empty
    // predictions
    // 
    predictor->learn(change);

    // Alternatively, predictor could have learnt thus...
    //    *stream << "foo bar foobar foz baz fozbaz roo rar roobar ";
    //    ct->update();

    {
	Prediction actual = predictor->predict(SIZE, 0);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(9), actual.size());
    }

    {
	const char* filters[] = {"f", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), actual.size());
    }

    {
	const char* filters[] = {"b", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), actual.size());
    }

    {
	const char* filters[] = {"r", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), actual.size());
    }

    {
	const char* filters[] = {"f", "b", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(6), actual.size());
    }

    {
	const char* filters[] = {"f", "r", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(7), actual.size());
    }

    {
	const char* filters[] = {"f", "b", "r", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(9), actual.size());
    }

    {
	const char* filters[] = {"fo", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), actual.size());
    }

    {
	const char* filters[] = {"foo", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), actual.size());
    }

    {
	const char* filters[] = {"fo", "ba", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(6), actual.size());
    }

    {
	const char* filters[] = {"fo", "ba", "ro", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(8), actual.size());
    }

    {
	const char* filters[] = {"foo", "bar", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), actual.size());
    }

    {
	const char* filters[] = {"foobar", "fozba", "roo", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), actual.size());
    }

    {
	const char* filters[] = {"foobar", "fozbaz", "roobar", 0};
	Prediction actual = predictor->predict(SIZE, filters);
	CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), actual.size());
    }

}
