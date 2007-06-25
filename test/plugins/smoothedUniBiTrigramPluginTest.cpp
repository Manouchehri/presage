
/******************************************************************************\
 *                                                                            *
 * Soothsayer, an extensible predictive text entry system                     *
 * ------------------------------------------------------                     *
 *                                                                            *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>             *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*
 *                                                                            *
\******************************************************************************/        

// for unlink(), getcwd()
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include "smoothedUniBiTrigramPluginTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SmoothedUniBiTrigramPluginTest );

const std::string SmoothedUniBiTrigramPluginTest::DATABASE = "database.db";

const std::string SmoothedUniBiTrigramPluginTest::FOO("foo");
const std::string SmoothedUniBiTrigramPluginTest::BAR("bar");
const std::string SmoothedUniBiTrigramPluginTest::FOOBAR("foobar");

void SmoothedUniBiTrigramPluginTest::setUp()
{
    // prepare database
    SqliteDatabaseConnector db(DATABASE);

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

void SmoothedUniBiTrigramPluginTest::tearDown()
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

ConfigMap SmoothedUniBiTrigramPluginTest::prepareConfigMap(const char* config[]) const
{
    ConfigMap map;
    map["UNIGRAM_WEIGHT"]              = config[0];
    map["BIGRAM_WEIGHT"]               = config[1];
    map["TRIGRAM_WEIGHT"]              = config[2];
    map["MAX_PARTIAL_PREDICTION_SIZE"] = config[3];
    map["DBFILENAME"]                  = config[4];
    
    return map;
}

Prediction SmoothedUniBiTrigramPluginTest::runPredict(const char** config, const char** history) const
{
    // convert configuration details into an object that the mock
    // Profile object can understand - in this case, an std::map of
    // std::string's.
    ConfigMap configMap = prepareConfigMap(config);

    // pass the configuration map to the mock profile object. The cast
    // to TiXmlDocument* is necessary since only the implementation of
    // the mock Profile class is different - the interface cannot be
    // changed. The mock Profile object's constructor will cast its
    // argument back to ConfigMap* before use.
    Profile profile((TiXmlDocument*) &configMap);

    // similarly, the mock HistoryTracker object's interface is
    // unchanged, therefore casting argument.
    HistoryTracker ht((const char*) history);

    // creating plugin object to test using the mock HistoryTracker
    // and Profile objects.
    SmoothedUniBiTrigramPlugin plugin(ht, &profile);

    // return the prediction for comparison with expected results.
    return plugin.predict();
}

void SmoothedUniBiTrigramPluginTest::assertCorrectPrediction(const char** config,
							     const char** history,
							     const int expected_prediction_size,
							     const std::string* expected_prediction_words) const
{
    Prediction prediction = runPredict(config, history);

    std::cout << "assertCorrectPrediction: " << prediction << std::endl;

    CPPUNIT_ASSERT_EQUAL(expected_prediction_size, prediction.size());
    
    for (int i = 0; i < expected_prediction_size; i++) {
	CPPUNIT_ASSERT_EQUAL(expected_prediction_words[i], prediction.getSuggestion(i).getWord());
    }
}

void SmoothedUniBiTrigramPluginTest::testUnigramWeight()
{
    const char* config[] = { "1.0", "0.0", "0.0", "6", DATABASE.c_str() };

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

void SmoothedUniBiTrigramPluginTest::testBigramWeight()
{
    const char* config[] = { "0.0", "1.0", "0.0", "6", DATABASE.c_str() };

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

void SmoothedUniBiTrigramPluginTest::testTrigramWeight()
{
    const char* config[] = { "0.0", "0.0", "1.0", "6", DATABASE.c_str() };

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

void SmoothedUniBiTrigramPluginTest::testUnigramBigramWeight()
{
    const char* config[] = { "0.0001", "0.9999", "0.0", "6", DATABASE.c_str() };

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

void SmoothedUniBiTrigramPluginTest::testUnigramTrigramWeight()
{
    const char* config[] = { "0.0001", "0.0", "0.9999", "6", DATABASE.c_str() };

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

void SmoothedUniBiTrigramPluginTest::testBigramTrigramWeight()
{
    const char* config[] = { "0.0", "0.0001", "0.9999", "6", DATABASE.c_str() };

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

void SmoothedUniBiTrigramPluginTest::testUnigramBigramTrigramWeight()
{
    const char* config[] = { "0.000001", "0.001", "0.998999", "6", DATABASE.c_str() };

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

void SmoothedUniBiTrigramPluginTest::testMaxPartialPredictionSize()
{
    // test that maximum partial prediction size config value is
    // honoured by checking that returned prediction size does not
    // exceed specified partial prediction size.
    for (int size = 0; size <= 3; size++) {
	std::stringstream ss; // used to convert int to C-string
	ss << size;
	std::string size_str = ss.str();

	// assign equal weight to uni/bi/tri-grams; only vary maximum
	// partial prediction size.
	const char* config[] = { "0.33", "0.33", "0.33", size_str.c_str(), DATABASE.c_str() };
	const char* history[] = { "", "", "" };
	
	Prediction prediction = runPredict(config, history);
	CPPUNIT_ASSERT_EQUAL(size, prediction.size());
    }
}
