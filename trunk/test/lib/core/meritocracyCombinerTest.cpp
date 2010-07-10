
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


#include "meritocracyCombinerTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MeritocracyCombinerTest );

void MeritocracyCombinerTest::setUp()
{
    combiner = new MeritocracyCombiner();

    p1 = new Prediction();
    p1->addSuggestion(Suggestion("foo1", 1.0));
    p1->addSuggestion(Suggestion("foo2", 0.8));
    p1->addSuggestion(Suggestion("foo3", 0.6));
    p1->addSuggestion(Suggestion("foo4", 0.4));
    p1->addSuggestion(Suggestion("foo5", 0.2));
    p1->addSuggestion(Suggestion("foo6", 0.0));
    					   
    p2 = new Prediction();		   
    p2->addSuggestion(Suggestion("bar1", 0.9));
    p2->addSuggestion(Suggestion("bar2", 0.7));
    p2->addSuggestion(Suggestion("bar3", 0.5));
    p2->addSuggestion(Suggestion("bar4", 0.3));
    p2->addSuggestion(Suggestion("bar5", 0.1));
    p2->addSuggestion(Suggestion("bar6", 0.0));
    
    p3 = new Prediction();
    p3->addSuggestion(Suggestion("foobar1", 0.95));
    p3->addSuggestion(Suggestion("foobar2", 0.75));
    p3->addSuggestion(Suggestion("foobar3", 0.55));
    p3->addSuggestion(Suggestion("foobar4", 0.35));
    p3->addSuggestion(Suggestion("foobar5", 0.15));
    p3->addSuggestion(Suggestion("foobar6", 0.05));

    p12 = new Prediction();
    p12->addSuggestion(Suggestion("foo1", 1.0));
    p12->addSuggestion(Suggestion("foo2", 0.8));
    p12->addSuggestion(Suggestion("foo3", 0.6));
    p12->addSuggestion(Suggestion("foo4", 0.4));
    p12->addSuggestion(Suggestion("foo5", 0.2));
    p12->addSuggestion(Suggestion("foo6", 0.0));
    p12->addSuggestion(Suggestion("bar1", 0.9));
    p12->addSuggestion(Suggestion("bar2", 0.7));
    p12->addSuggestion(Suggestion("bar3", 0.5));
    p12->addSuggestion(Suggestion("bar4", 0.3));
    p12->addSuggestion(Suggestion("bar5", 0.1));
    p12->addSuggestion(Suggestion("bar6", 0.0));
    
    p13 = new Prediction();
    p13->addSuggestion(Suggestion("foo1", 1.0));
    p13->addSuggestion(Suggestion("foo2", 0.8));
    p13->addSuggestion(Suggestion("foo3", 0.6));
    p13->addSuggestion(Suggestion("foo4", 0.4));
    p13->addSuggestion(Suggestion("foo5", 0.2));
    p13->addSuggestion(Suggestion("foo6", 0.0));
    p13->addSuggestion(Suggestion("foobar1", 0.95));
    p13->addSuggestion(Suggestion("foobar2", 0.75));
    p13->addSuggestion(Suggestion("foobar3", 0.55));
    p13->addSuggestion(Suggestion("foobar4", 0.35));
    p13->addSuggestion(Suggestion("foobar5", 0.15));
    p13->addSuggestion(Suggestion("foobar6", 0.05));

    p23 = new Prediction();		   
    p23->addSuggestion(Suggestion("bar1", 0.9));
    p23->addSuggestion(Suggestion("bar2", 0.7));
    p23->addSuggestion(Suggestion("bar3", 0.5));
    p23->addSuggestion(Suggestion("bar4", 0.3));
    p23->addSuggestion(Suggestion("bar5", 0.1));
    p23->addSuggestion(Suggestion("bar6", 0.0));
    p23->addSuggestion(Suggestion("foobar1", 0.95));
    p23->addSuggestion(Suggestion("foobar2", 0.75));
    p23->addSuggestion(Suggestion("foobar3", 0.55));
    p23->addSuggestion(Suggestion("foobar4", 0.35));
    p23->addSuggestion(Suggestion("foobar5", 0.15));
    p23->addSuggestion(Suggestion("foobar6", 0.05));

    p123 = new Prediction();
    p123->addSuggestion(Suggestion("foo1", 1.0));
    p123->addSuggestion(Suggestion("foo2", 0.8));
    p123->addSuggestion(Suggestion("foo3", 0.6));
    p123->addSuggestion(Suggestion("foo4", 0.4));
    p123->addSuggestion(Suggestion("foo5", 0.2));
    p123->addSuggestion(Suggestion("foo6", 0.0));
    p123->addSuggestion(Suggestion("bar1", 0.9));
    p123->addSuggestion(Suggestion("bar2", 0.7));
    p123->addSuggestion(Suggestion("bar3", 0.5));
    p123->addSuggestion(Suggestion("bar4", 0.3));
    p123->addSuggestion(Suggestion("bar5", 0.1));
    p123->addSuggestion(Suggestion("bar6", 0.0));
    p123->addSuggestion(Suggestion("foobar1", 0.95));
    p123->addSuggestion(Suggestion("foobar2", 0.75));
    p123->addSuggestion(Suggestion("foobar3", 0.55));
    p123->addSuggestion(Suggestion("foobar4", 0.35));
    p123->addSuggestion(Suggestion("foobar5", 0.15));
    p123->addSuggestion(Suggestion("foobar6", 0.05));
}

void MeritocracyCombinerTest::tearDown()
{
    delete combiner;

    delete p1;
    delete p2;
    delete p3;

    delete p12;
    delete p23;
    delete p13;

    delete p123;
}

void MeritocracyCombinerTest::testCombineSinglePrediction()
{
    std::vector< Prediction > predictions;

    predictions.push_back(*p1);
    CPPUNIT_ASSERT_EQUAL(*p1, combiner->combine(predictions));
    predictions.clear();

    predictions.push_back(*p2);
    CPPUNIT_ASSERT_EQUAL(*p2, combiner->combine(predictions));
    predictions.clear();

    predictions.push_back(*p3);
    CPPUNIT_ASSERT_EQUAL(*p3, combiner->combine(predictions));
    predictions.clear();
}

void MeritocracyCombinerTest::testCombineTwoPredictions()
{
    std::vector< Prediction > predictions;

    predictions.push_back(*p1);
    predictions.push_back(*p2);
    CPPUNIT_ASSERT_EQUAL(*p12, combiner->combine(predictions));
    predictions.clear();

    predictions.push_back(*p2);
    predictions.push_back(*p3);
    CPPUNIT_ASSERT_EQUAL(*p23, combiner->combine(predictions));
    predictions.clear();

    predictions.push_back(*p1);
    predictions.push_back(*p3);
    CPPUNIT_ASSERT_EQUAL(*p13, combiner->combine(predictions));
    predictions.clear();
}

void MeritocracyCombinerTest::testCombineThreePredictions()
{
    std::vector< Prediction > predictions;
    predictions.push_back(*p1);
    predictions.push_back(*p2);
    predictions.push_back(*p3);
    CPPUNIT_ASSERT_EQUAL(*p123, combiner->combine(predictions));
}
