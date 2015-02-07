
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


#include "selectorTest.h"
#include "../common/stringstreamPresageCallback.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SelectorTest );

SelectorTest::TestDataSuite::TestDataSuite()
{}

SelectorTest::TestDataSuite::~TestDataSuite()
{}

void SelectorTest::TestDataSuite::init()
{}

bool SelectorTest::TestDataSuite::hasMoreTestData() const
{
    if (iter != testData.end()) {
	return true;
    } else {
	return false;
    }
}

void SelectorTest::TestDataSuite::nextTestData()
{
    iter++;
}

SelectorTest::TestDataSuite_S6_NR_T0::TestDataSuite_S6_NR_T0()
{
    init();
}

void SelectorTest::TestDataSuite_S6_NR_T0::init()
{
    TestData* td;
    Prediction* ip;
    Prediction* op;

    td = new TestData;
    ip = new Prediction;
    op = new Prediction;
    ip->addSuggestion(Suggestion("foo",        0.9));
    ip->addSuggestion(Suggestion("foo1",       0.8));
    ip->addSuggestion(Suggestion("foo2",       0.7));
    ip->addSuggestion(Suggestion("foo3",       0.6));
    ip->addSuggestion(Suggestion("foo4",       0.5));
    ip->addSuggestion(Suggestion("foo5",       0.4));
    ip->addSuggestion(Suggestion("foo6",       0.3));
    ip->addSuggestion(Suggestion("foobar",     0.2));
    ip->addSuggestion(Suggestion("foobar1",    0.1));
    ip->addSuggestion(Suggestion("foobar2",    0.09));
    ip->addSuggestion(Suggestion("foobar3",    0.08));
    ip->addSuggestion(Suggestion("foobar4",    0.07));
    ip->addSuggestion(Suggestion("foobar5",    0.06));
    ip->addSuggestion(Suggestion("foobar6",    0.05));
    ip->addSuggestion(Suggestion("foobar7",    0.04));
    ip->addSuggestion(Suggestion("foobar8",    0.03));
    ip->addSuggestion(Suggestion("foobar9",    0.02));
    ip->addSuggestion(Suggestion("foobarfoo",  0.01));
    ip->addSuggestion(Suggestion("foobarfoo1", 0.009));
    ip->addSuggestion(Suggestion("foobarfoo2", 0.008));
    ip->addSuggestion(Suggestion("foobarfoo3", 0.007));
    op->addSuggestion(Suggestion("foo",     0.9));
    op->addSuggestion(Suggestion("foo1",    0.8));
    op->addSuggestion(Suggestion("foo2",    0.7));
    op->addSuggestion(Suggestion("foo3",    0.6));
    op->addSuggestion(Suggestion("foo4",    0.5));
    op->addSuggestion(Suggestion("foo5",    0.4));
    td->updateString     = "f";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;

    td = new TestData;
    op = new Prediction;
    op->addSuggestion(Suggestion("foo6",       0.3));
    op->addSuggestion(Suggestion("foobar",     0.2));
    op->addSuggestion(Suggestion("foobar1",    0.1));
    op->addSuggestion(Suggestion("foobar2",    0.09));
    op->addSuggestion(Suggestion("foobar3",    0.08));
    op->addSuggestion(Suggestion("foobar4",    0.07));
    td->updateString     = "o";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;
    
    td = new TestData;
    op = new Prediction;
    op->addSuggestion(Suggestion("foobar5",    0.06));
    op->addSuggestion(Suggestion("foobar6",    0.05));
    op->addSuggestion(Suggestion("foobar7",    0.04));
    op->addSuggestion(Suggestion("foobar8",    0.03));
    op->addSuggestion(Suggestion("foobar9",    0.02));
    op->addSuggestion(Suggestion("foobarfoo",  0.01));
    td->updateString     = "o";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;

    iter = testData.begin();
}

SelectorTest::TestDataSuite_S6_R_T0::TestDataSuite_S6_R_T0()
{
    init();
}

void SelectorTest::TestDataSuite_S6_R_T0::init()
{
    TestData* td;
    Prediction* ip;
    Prediction* op;

    td = new TestData;
    ip = new Prediction;
    op = new Prediction;
    ip->addSuggestion(Suggestion("foo",        0.9));
    ip->addSuggestion(Suggestion("foo1",       0.8));
    ip->addSuggestion(Suggestion("foo2",       0.7));
    ip->addSuggestion(Suggestion("foo3",       0.6));
    ip->addSuggestion(Suggestion("foo4",       0.5));
    ip->addSuggestion(Suggestion("foo5",       0.4));
    ip->addSuggestion(Suggestion("foo6",       0.3));
    ip->addSuggestion(Suggestion("foobar",     0.2));
    ip->addSuggestion(Suggestion("foobar1",    0.1));
    ip->addSuggestion(Suggestion("foobar2",    0.09));
    ip->addSuggestion(Suggestion("foobar3",    0.08));
    ip->addSuggestion(Suggestion("foobar4",    0.07));
    ip->addSuggestion(Suggestion("foobar5",    0.06));
    ip->addSuggestion(Suggestion("foobar6",    0.05));
    ip->addSuggestion(Suggestion("foobar7",    0.04));
    ip->addSuggestion(Suggestion("foobar8",    0.03));
    ip->addSuggestion(Suggestion("foobar9",    0.02));
    ip->addSuggestion(Suggestion("foobarfoo",  0.01));
    ip->addSuggestion(Suggestion("foobarfoo1", 0.009));
    ip->addSuggestion(Suggestion("foobarfoo2", 0.008));
    ip->addSuggestion(Suggestion("foobarfoo3", 0.007));
    op->addSuggestion(Suggestion("foo",     0.9));
    op->addSuggestion(Suggestion("foo1",    0.8));
    op->addSuggestion(Suggestion("foo2",    0.7));
    op->addSuggestion(Suggestion("foo3",    0.6));
    op->addSuggestion(Suggestion("foo4",    0.5));
    op->addSuggestion(Suggestion("foo5",    0.4));
    td->updateString     = "f";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;

    td = new TestData;
    op = new Prediction;
    op->addSuggestion(Suggestion("foo",     0.9));
    op->addSuggestion(Suggestion("foo1",    0.8));
    op->addSuggestion(Suggestion("foo2",    0.7));
    op->addSuggestion(Suggestion("foo3",    0.6));
    op->addSuggestion(Suggestion("foo4",    0.5));
    op->addSuggestion(Suggestion("foo5",    0.4));
    td->updateString     = "o";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;
    
    td = new TestData;
    op = new Prediction;
    op->addSuggestion(Suggestion("foo",     0.9));
    op->addSuggestion(Suggestion("foo1",    0.8));
    op->addSuggestion(Suggestion("foo2",    0.7));
    op->addSuggestion(Suggestion("foo3",    0.6));
    op->addSuggestion(Suggestion("foo4",    0.5));
    op->addSuggestion(Suggestion("foo5",    0.4));
    td->updateString     = "o";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;

    iter = testData.begin();
}

SelectorTest::TestDataSuite_S6_NR_T3::TestDataSuite_S6_NR_T3()
{
    init();
}

void SelectorTest::TestDataSuite_S6_NR_T3::init()
{
    TestData* td;
    Prediction* ip;
    Prediction* op;

    td = new TestData;
    ip = new Prediction;
    op = new Prediction;
    ip->addSuggestion(Suggestion("foo",        0.9));
    ip->addSuggestion(Suggestion("foo1",       0.8));
    ip->addSuggestion(Suggestion("foo2",       0.7));
    ip->addSuggestion(Suggestion("foo3",       0.6));
    ip->addSuggestion(Suggestion("foo4",       0.5));
    ip->addSuggestion(Suggestion("foo5",       0.4));
    ip->addSuggestion(Suggestion("foo6",       0.3));
    ip->addSuggestion(Suggestion("foobar",     0.2));
    ip->addSuggestion(Suggestion("foobar1",    0.1));
    ip->addSuggestion(Suggestion("foobar2",    0.09));
    ip->addSuggestion(Suggestion("foobar3",    0.08));
    ip->addSuggestion(Suggestion("foobar4",    0.07));
    ip->addSuggestion(Suggestion("foobar5",    0.06));
    ip->addSuggestion(Suggestion("foobar6",    0.05));
    ip->addSuggestion(Suggestion("foobar7",    0.04));
    ip->addSuggestion(Suggestion("foobar8",    0.03));
    ip->addSuggestion(Suggestion("foobar9",    0.02));
    ip->addSuggestion(Suggestion("foobarfoo",  0.01));
    ip->addSuggestion(Suggestion("foobarfoo1", 0.009));
    ip->addSuggestion(Suggestion("foobarfoo2", 0.008));
    ip->addSuggestion(Suggestion("foobarfoo3", 0.007));
    op->addSuggestion(Suggestion("foo1",    0.8));
    op->addSuggestion(Suggestion("foo2",    0.7));
    op->addSuggestion(Suggestion("foo3",    0.6));
    op->addSuggestion(Suggestion("foo4",    0.5));
    op->addSuggestion(Suggestion("foo5",    0.4));
    op->addSuggestion(Suggestion("foo6",    0.3));
    td->updateString     = "f";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;

    td = new TestData;
    op = new Prediction;
    op->addSuggestion(Suggestion("foobar",     0.2));
    op->addSuggestion(Suggestion("foobar1",    0.1));
    op->addSuggestion(Suggestion("foobar2",    0.09));
    op->addSuggestion(Suggestion("foobar3",    0.08));
    op->addSuggestion(Suggestion("foobar4",    0.07));
    op->addSuggestion(Suggestion("foobar5",    0.06));
    td->updateString     = "o";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;
    
    td = new TestData;
    op = new Prediction;
    op->addSuggestion(Suggestion("foobar6",    0.05));
    op->addSuggestion(Suggestion("foobar7",    0.04));
    op->addSuggestion(Suggestion("foobar8",    0.03));
    op->addSuggestion(Suggestion("foobar9",    0.02));
    op->addSuggestion(Suggestion("foobarfoo",  0.01));
    op->addSuggestion(Suggestion("foobarfoo1", 0.009));
    td->updateString     = "o";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;

    iter = testData.begin();
}

SelectorTest::TestDataSuite_S6_R_T3::TestDataSuite_S6_R_T3()
{
    init();
}

void SelectorTest::TestDataSuite_S6_R_T3::init()
{
    TestData* td;
    Prediction* ip;
    Prediction* op;

    td = new TestData;
    ip = new Prediction;
    op = new Prediction;
    ip->addSuggestion(Suggestion("foo",        0.9));
    ip->addSuggestion(Suggestion("foo1",       0.8));
    ip->addSuggestion(Suggestion("foo2",       0.7));
    ip->addSuggestion(Suggestion("foo3",       0.6));
    ip->addSuggestion(Suggestion("foo4",       0.5));
    ip->addSuggestion(Suggestion("foo5",       0.4));
    ip->addSuggestion(Suggestion("foo6",       0.3));
    ip->addSuggestion(Suggestion("foobar",     0.2));
    ip->addSuggestion(Suggestion("foobar1",    0.1));
    ip->addSuggestion(Suggestion("foobar2",    0.09));
    ip->addSuggestion(Suggestion("foobar3",    0.08));
    ip->addSuggestion(Suggestion("foobar4",    0.07));
    ip->addSuggestion(Suggestion("foobar5",    0.06));
    ip->addSuggestion(Suggestion("foobar6",    0.05));
    ip->addSuggestion(Suggestion("foobar7",    0.04));
    ip->addSuggestion(Suggestion("foobar8",    0.03));
    ip->addSuggestion(Suggestion("foobar9",    0.02));
    ip->addSuggestion(Suggestion("foobarfoo",  0.01));
    ip->addSuggestion(Suggestion("foobarfoo1", 0.009));
    ip->addSuggestion(Suggestion("foobarfoo2", 0.008));
    ip->addSuggestion(Suggestion("foobarfoo3", 0.007));
    op->addSuggestion(Suggestion("foo1",    0.8));
    op->addSuggestion(Suggestion("foo2",    0.7));
    op->addSuggestion(Suggestion("foo3",    0.6));
    op->addSuggestion(Suggestion("foo4",    0.5));
    op->addSuggestion(Suggestion("foo5",    0.4));
    op->addSuggestion(Suggestion("foo6",    0.3));
    td->updateString     = "f";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;

    td = new TestData;
    op = new Prediction;
    op->addSuggestion(Suggestion("foobar",     0.2));
    op->addSuggestion(Suggestion("foobar1",    0.1));
    op->addSuggestion(Suggestion("foobar2",    0.09));
    op->addSuggestion(Suggestion("foobar3",    0.08));
    op->addSuggestion(Suggestion("foobar4",    0.07));
    op->addSuggestion(Suggestion("foobar5",    0.06));
    td->updateString     = "o";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;
    
    td = new TestData;
    op = new Prediction;
    op->addSuggestion(Suggestion("foobar",     0.2));
    op->addSuggestion(Suggestion("foobar1",    0.1));
    op->addSuggestion(Suggestion("foobar2",    0.09));
    op->addSuggestion(Suggestion("foobar3",    0.08));
    op->addSuggestion(Suggestion("foobar4",    0.07));
    op->addSuggestion(Suggestion("foobar5",    0.06));
    td->updateString     = "o";
    td->inputPrediction  = *ip;
    td->outputPrediction = *op;
    testData.push_back(*td);
    delete td;
    delete op;

    iter = testData.begin();
}

void SelectorTest::setUp()
{
    testStringSuite = new TestStringSuite();
    tds_S6_NR_T0 = new TestDataSuite_S6_NR_T0();
    tds_S6_R_T0 = new TestDataSuite_S6_R_T0();
    tds_S6_NR_T3 = new TestDataSuite_S6_NR_T3();
    tds_S6_R_T3 = new TestDataSuite_S6_R_T3();

    configuration = new Configuration();
    configuration->insert (PredictorRegistry::LOGGER, "ERROR");
    configuration->insert (PredictorRegistry::PREDICTORS, "");
    configuration->insert (ContextTracker::LOGGER, "ERROR");
    configuration->insert (ContextTracker::SLIDING_WINDOW_SIZE, "80");
    configuration->insert (ContextTracker::LOWERCASE_MODE, "no");
    configuration->insert (ContextTracker::ONLINE_LEARNING, "yes");
    configuration->insert (Selector::LOGGER, "ERROR");
    configuration->insert (Selector::SUGGESTIONS, "6");
    configuration->insert (Selector::REPEAT_SUGGESTIONS, "no");
    configuration->insert (Selector::GREEDY_SUGGESTION_THRESHOLD, "0");

    predictorRegistry = new PredictorRegistry(configuration);
    strstream = new std::stringstream();
    callback = new StringstreamPresageCallback(*strstream);
    contextTracker  = new ContextTracker(configuration, predictorRegistry, callback);
    selector = new Selector(configuration, contextTracker);
}

void SelectorTest::tearDown()
{
    delete testStringSuite;
    delete tds_S6_NR_T0;
    delete tds_S6_R_T0;
    delete tds_S6_NR_T3;
    delete tds_S6_R_T3;

    delete selector;
    delete contextTracker;
    delete strstream;
    delete callback;
    delete configuration;
}

void SelectorTest::testSelect(TestDataSuite* tds)
{
    while (tds->hasMoreTestData()) {
	*strstream << tds->getUpdateString();
	std::vector<std::string> selectedTokens;
	selectedTokens = selector->select(tds->getInputPrediction());

	Prediction expected = tds->getOutputPrediction();
	CPPUNIT_ASSERT_EQUAL( (size_t)expected.size(), selectedTokens.size() );

	std::vector<std::string>::const_iterator actual_it = selectedTokens.begin();
	int index = 0;
	while (actual_it != selectedTokens.end()) {
	    CPPUNIT_ASSERT_EQUAL(expected.getSuggestion(index).getWord(),
				 *actual_it);
	    
	    index++;
	    actual_it++;
	}

	contextTracker->update();
	tds->nextTestData();
    }
}

void SelectorTest::testSelect_S6_R_T0()
{
    configuration->find (Selector::SUGGESTIONS)->set_value ("6");
    configuration->find (Selector::REPEAT_SUGGESTIONS)->set_value ("true");
    configuration->find (Selector::GREEDY_SUGGESTION_THRESHOLD)->set_value ("0");

    testSelect(tds_S6_R_T0);
}

void SelectorTest::testSelect_S6_NR_T0()
{
    configuration->find (Selector::SUGGESTIONS)->set_value ("6");
    configuration->find (Selector::REPEAT_SUGGESTIONS)->set_value ("false");
    configuration->find (Selector::GREEDY_SUGGESTION_THRESHOLD)->set_value ("0");

    testSelect(tds_S6_NR_T0);
}

void SelectorTest::testSelect_S6_R_T3()
{
    configuration->find (Selector::SUGGESTIONS)->set_value ("6");
    configuration->find (Selector::REPEAT_SUGGESTIONS)->set_value ("true");
    configuration->find (Selector::GREEDY_SUGGESTION_THRESHOLD)->set_value ("3");

    testSelect(tds_S6_R_T3);
}

void SelectorTest::testSelect_S6_NR_T3()
{
    configuration->find (Selector::SUGGESTIONS)->set_value ("6");
    configuration->find (Selector::REPEAT_SUGGESTIONS)->set_value ("false");
    configuration->find (Selector::GREEDY_SUGGESTION_THRESHOLD)->set_value ("3");

    testSelect(tds_S6_NR_T3);
}
