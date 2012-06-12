
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



#include "smoothedNgramPredictorTest.h"

// for unlink(), getcwd()
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( SmoothedNgramPredictorTest );

const std::string SmoothedNgramPredictorTest::DATABASE = "database.db";
const size_t      SmoothedNgramPredictorTest::CARDINALITY = 3;
const bool        SmoothedNgramPredictorTest::READ_WRITE_MODE = true;

const std::string SmoothedNgramPredictorTest::FOO("foo");
const std::string SmoothedNgramPredictorTest::BAR("bar");
const std::string SmoothedNgramPredictorTest::FOOBAR("foobar");

void SmoothedNgramPredictorTest::setUp()
{
    // prepare database
    SqliteDatabaseConnector db(DATABASE, CARDINALITY, READ_WRITE_MODE);

    // unigrams
    db.createUnigramTable();
    {
	Ngram ngram;
	ngram.push_back(FOO);
	db.insertNgram(ngram, 100000000);
    }
    {
	Ngram ngram;
	ngram.push_back(BAR);
	db.insertNgram(ngram, 10000000);
    }
    {
	Ngram ngram;
	ngram.push_back(FOOBAR);
	db.insertNgram(ngram, 1000000);
    }

    // bigrams
    db.createBigramTable();
    {
	Ngram ngram;
	ngram.push_back(FOO);
	ngram.push_back(BAR);
	db.insertNgram(ngram, 100000);
    }
    {
	Ngram ngram;
	ngram.push_back(BAR);
	ngram.push_back(FOOBAR);
	db.insertNgram(ngram, 10000);
    }
    {
	Ngram ngram;
	ngram.push_back(FOOBAR);
	ngram.push_back(FOO);
	db.insertNgram(ngram, 1000);
    }

    // trigrams
    db.createTrigramTable();
    {
	Ngram ngram;
	ngram.push_back(FOO);
	ngram.push_back(BAR);
	ngram.push_back(FOOBAR);
	db.insertNgram(ngram, 100);
    }
    {
	Ngram ngram;
	ngram.push_back(BAR);
	ngram.push_back(FOOBAR);
	ngram.push_back(FOO);
	db.insertNgram(ngram, 10);
    }
    {
	Ngram ngram;
	ngram.push_back(FOOBAR);
	ngram.push_back(FOO);
	ngram.push_back(BAR);
	db.insertNgram(ngram, 1);
    }

    // Now the database should look like this:
    // foo                  100000000
    // bar                  10000000
    // foobar               1000000
    // foo    bar           100000
    // bar    foobar        10000
    // foobar foo           1000
    // foo    bar    foobar 100
    // bar    foobar foo    10
    // foobar foo    bar    1
}

void SmoothedNgramPredictorTest::tearDown()
{
#ifdef HAVE_UNISTD_H
    // remove file
    int result = unlink(DATABASE.c_str());
    assert(result == 0);
#else
    // fail test
    std::string message = "Unable to remove database file " + DATABASE;
    CPPUNIT_FAIL( message.c_str() );
#endif
}

Configuration* SmoothedNgramPredictorTest::prepareConfiguration(const char* config[]) const
{
    Configuration* configuration = new Configuration();
    configuration->insert ("Presage.Predictors.TestSmoothedNgramPredictor.PREDICTOR", "TestSmoothedNgramPredictor");
    configuration->insert ("Presage.Predictors.TestSmoothedNgramPredictor.LOGGER", "ERROR");
    configuration->insert ("Presage.Predictors.TestSmoothedNgramPredictor.DELTAS", config[0]);
    configuration->insert ("Presage.Predictors.TestSmoothedNgramPredictor.DBFILENAME", config[1]);
    configuration->insert ("Presage.Predictors.TestSmoothedNgramPredictor.DatabaseConnector.LOGGER", "ERROR");
    configuration->insert ("Presage.Predictors.TestSmoothedNgramPredictor.LEARN", "FALSE");
    
    return configuration;
}

void SmoothedNgramPredictorTest::assertCorrectPrediction(const char** config,
							 const char** history,
							 const size_t expected_prediction_size,
							 const std::string* expected_prediction_words) const
{
    Prediction prediction = runPredict(config, history);

    CPPUNIT_ASSERT_EQUAL(expected_prediction_size, prediction.size());
    
    for (size_t i = 0; i < expected_prediction_size; i++) {
	CPPUNIT_ASSERT_EQUAL(expected_prediction_words[i], prediction.getSuggestion(i).getWord());
    }
}

Predictor* SmoothedNgramPredictorTest::createPredictor(Configuration* config,
						       ContextTracker* contextTracker) const
{
    return new SmoothedNgramPredictor(config, contextTracker, "TestSmoothedNgramPredictor");
}

void SmoothedNgramPredictorTest::testUnigramWeight()
{
    const char* config[] = { "1.0 0.0 0.0", DATABASE.c_str() };

    {
	const char* history[] = { "foo", "bar", "" };
	const std::string expected_prediction_words[] = { FOO, BAR, FOOBAR };
	unsigned int expected_prediction_size = 3;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "f" };
	const std::string expected_prediction_words[] = { FOO, FOOBAR };
	unsigned int expected_prediction_size = 2;
	
	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "ba" };
	const std::string expected_prediction_words[] = { BAR };
	unsigned int expected_prediction_size = 1;
	
	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "", "", "ba" };
	const std::string expected_prediction_words[] = { BAR };
	unsigned int expected_prediction_size = 1;
	
	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "foobar" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;
	
	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "foobarwoz" };
	const std::string expected_prediction_words[] = { };
	unsigned int expected_prediction_size = 0;
	
	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
}

void SmoothedNgramPredictorTest::testBigramWeight()
{
    const char* config[] = { "0.0 1.0 0.0", DATABASE.c_str() };

    {
	const char* history[] = { "foo", "bar", "" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "", "bar", "" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foobar", "bar", "" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foobar", "bar", "fo" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foobar", "foo", "" };
	const std::string expected_prediction_words[] = { BAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "bar", "foobar", "" };
	const std::string expected_prediction_words[] = { FOO };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
}

void SmoothedNgramPredictorTest::testTrigramWeight()
{
    const char* config[] = { "0.0 0.0 1.0", DATABASE.c_str() };

    {
	const char* history[] = { "foo", "bar", "" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "foo" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "foobar" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foobar", "foo", "ba" };
	const std::string expected_prediction_words[] = { BAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "bar", "foobar", "" };
	const std::string expected_prediction_words[] = { FOO };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
}

void SmoothedNgramPredictorTest::testUnigramBigramWeight()
{
    const char* config[] = { "0.0001 0.9999 0.0", DATABASE.c_str() };

    // unigram weight is much lower han bigram weight in order to
    // ensure that results are not masked by unigram results.

    {
	const char* history[] = { "foo", "bar", "" };
	const std::string expected_prediction_words[] = { FOOBAR, FOO, BAR };
	unsigned int expected_prediction_size = 3;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "fo" };
	const std::string expected_prediction_words[] = { FOOBAR, FOO };
	unsigned int expected_prediction_size = 2;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "foob" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
}

void SmoothedNgramPredictorTest::testUnigramTrigramWeight()
{
    const char* config[] = { "0.0001 0.0 0.9999", DATABASE.c_str() };

    // unigram weight is much lower han trigram weight in order to
    // ensure that results are not masked by unigram results.

    {
	const char* history[] = { "foo", "bar", "" };
	const std::string expected_prediction_words[] = { FOOBAR, FOO, BAR };
	unsigned int expected_prediction_size = 3;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "fo" };
	const std::string expected_prediction_words[] = { FOOBAR, FOO };
	unsigned int expected_prediction_size = 2;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "foob" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
}

void SmoothedNgramPredictorTest::testBigramTrigramWeight()
{
    const char* config[] = { "0.0 0.0001 0.9999", DATABASE.c_str() };

    // unigram weight is much lower han trigram weight in order to
    // ensure that results are not masked by unigram results.

    {
	const char* history[] = { "foo", "bar", "" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "fo" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "foob" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
}

void SmoothedNgramPredictorTest::testUnigramBigramTrigramWeight()
{
    const char* config[] = { "0.000001 0.001 0.998999", DATABASE.c_str() };

    // unigram weight is much lower han trigram weight in order to
    // ensure that results are not masked by unigram results.

    {
	const char* history[] = { "foo", "bar", "" };
	const std::string expected_prediction_words[] = { FOOBAR, FOO, BAR };
	unsigned int expected_prediction_size = 3;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "fo" };
	const std::string expected_prediction_words[] = { FOOBAR, FOO };
	unsigned int expected_prediction_size = 2;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
    {
	const char* history[] = { "foo", "bar", "foob" };
	const std::string expected_prediction_words[] = { FOOBAR };
	unsigned int expected_prediction_size = 1;

	assertCorrectPrediction(config, history, expected_prediction_size, expected_prediction_words);
    }
}

void SmoothedNgramPredictorTest::testMaxPartialPredictionSize()
{
    // test that maximum partial prediction size config value is
    // honoured by checking that returned prediction size does not
    // exceed specified partial prediction size.
    for (size_t size = 0; size <= 3; size++) {

	// assign equal weight to uni/bi/tri-grams; only vary maximum
	// partial prediction size.
	const char* config[] = { "0.33 0.33 0.33", DATABASE.c_str() };
	const char* history[] = { "", "", "" };
	
	Prediction prediction = runPredict(config, history, size);
	CPPUNIT_ASSERT_EQUAL(size, prediction.size());
    }
}
