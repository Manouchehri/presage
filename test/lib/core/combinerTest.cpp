
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


#include "combinerTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CombinerTest );

void CombinerTest::setUp()
{
    sugg1Ptr = new Suggestion("foo", 0.1);
    sugg2Ptr = new Suggestion("foo", 0.3);
    sugg3Ptr = new Suggestion("foo", 0.5);
    sugg123Ptr = new Suggestion("foo", 0.9);
    sugg4Ptr = new Suggestion("bar", 0.2);
    sugg5Ptr = new Suggestion("bar", 0.4);
    sugg6Ptr = new Suggestion("bar", 0.6);
    sugg456Ptr = new Suggestion("bar", 1.0);

    prediction1 = new Prediction();
    prediction1->addSuggestion(*sugg1Ptr);
    prediction1->addSuggestion(*sugg4Ptr);
    
    // same as prediction1
    expected_prediction1 = new Prediction();
    expected_prediction1->addSuggestion(*sugg1Ptr);
    expected_prediction1->addSuggestion(*sugg4Ptr);    

    prediction2 = new Prediction();
    prediction2->addSuggestion(*sugg1Ptr);
    prediction2->addSuggestion(*sugg2Ptr);
    prediction2->addSuggestion(*sugg3Ptr);

    // only one foo suggestion, sum of probs
    expected_prediction2 = new Prediction();
    expected_prediction2->addSuggestion(*sugg123Ptr);

    prediction3 = new Prediction();
    prediction3->addSuggestion(*sugg1Ptr);
    prediction3->addSuggestion(*sugg2Ptr);
    prediction3->addSuggestion(*sugg3Ptr);
    prediction3->addSuggestion(*sugg4Ptr);

    // only one foo suggestion, sum of probs and one bar suggestion
    expected_prediction3 = new Prediction();
    expected_prediction3->addSuggestion(*sugg123Ptr);
    expected_prediction3->addSuggestion(*sugg4Ptr);

    prediction4 = new Prediction();
    prediction4->addSuggestion(*sugg3Ptr);
    prediction4->addSuggestion(*sugg4Ptr);
    prediction4->addSuggestion(*sugg5Ptr);
    prediction4->addSuggestion(*sugg6Ptr);

    // only one bar suggestion, sum of probs and one foo suggestion
    expected_prediction4 = new Prediction();
    expected_prediction4->addSuggestion(*sugg3Ptr);
    expected_prediction4->addSuggestion(*sugg456Ptr);

    prediction5 = new Prediction();
    prediction5->addSuggestion(*sugg1Ptr);
    prediction5->addSuggestion(*sugg2Ptr);
    prediction5->addSuggestion(*sugg3Ptr);
    prediction5->addSuggestion(*sugg4Ptr);
    prediction5->addSuggestion(*sugg5Ptr);
    prediction5->addSuggestion(*sugg6Ptr);

    // one foo suggestion and one bar suggestion, both with sum of
    // probs
    expected_prediction5 = new Prediction();
    expected_prediction5->addSuggestion(*sugg123Ptr);
    expected_prediction5->addSuggestion(*sugg456Ptr);
}


void CombinerTest::tearDown()
{
    delete sugg1Ptr;
    delete sugg2Ptr;
    delete sugg3Ptr;
    delete sugg4Ptr;
    delete sugg5Ptr;
    delete sugg6Ptr;
    delete sugg123Ptr;
    delete sugg456Ptr;

    delete prediction1;
    delete prediction2;
    delete prediction3;
    delete prediction4;
    delete prediction5;
    delete expected_prediction1;
    delete expected_prediction2;
    delete expected_prediction3;
    delete expected_prediction4;
    delete expected_prediction5;
}

void CombinerTest::testFilter()
{
    ConcreteCombiner combiner;

    CPPUNIT_ASSERT_EQUAL(*expected_prediction1, combiner.filter(*prediction1));
    CPPUNIT_ASSERT_EQUAL(*expected_prediction2, combiner.filter(*prediction2));
    CPPUNIT_ASSERT_EQUAL(*expected_prediction3, combiner.filter(*prediction3));
    CPPUNIT_ASSERT_EQUAL(*expected_prediction4, combiner.filter(*prediction4));
    CPPUNIT_ASSERT_EQUAL(*expected_prediction5, combiner.filter(*prediction5));
}
