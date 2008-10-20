
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


#include "predictionTest.h"
#include "core/prediction.h"

#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION( PredictionTest );

void PredictionTest::setUp()
{
    sugg1Ptr = new Suggestion();
    sugg2Ptr = new Suggestion( "foo", 0.1 );
    sugg3Ptr = new Suggestion( "bar", 0.3 );

    empty_prediction = new Prediction();

    prediction1 = new Prediction();
    prediction1->addSuggestion(*sugg1Ptr);

    prediction2 = new Prediction();
    prediction2->addSuggestion(*sugg2Ptr);

    prediction3 = new Prediction();
    prediction3->addSuggestion(*sugg3Ptr);

    prediction23 = new Prediction();
    prediction23->addSuggestion(*sugg2Ptr);
    prediction23->addSuggestion(*sugg3Ptr);

    prediction13 = new Prediction();
    prediction13->addSuggestion(*sugg1Ptr);
    prediction13->addSuggestion(*sugg3Ptr);

    prediction12 = new Prediction();
    prediction12->addSuggestion(*sugg1Ptr);
    prediction12->addSuggestion(*sugg2Ptr);

    prediction123 = new Prediction();
    prediction123->addSuggestion(*sugg1Ptr);
    prediction123->addSuggestion(*sugg2Ptr);
    prediction123->addSuggestion(*sugg3Ptr);
}


void PredictionTest::tearDown()
{
    delete sugg1Ptr;
    delete sugg2Ptr;
    delete sugg3Ptr;

    delete empty_prediction;
    delete prediction1;
    delete prediction2;
    delete prediction3;
    delete prediction23;
    delete prediction13;
    delete prediction12;
    delete prediction123;
}

void PredictionTest::testAssignmentOperator()
{
    Prediction* prediction = new Prediction();
    const int PREDICTIONS_SIZE = 7;
    Prediction* predictions[PREDICTIONS_SIZE] = {
	prediction1,
	prediction2,
	prediction3,
	prediction12,
	prediction23,
	prediction13,
	prediction123 
    };
    
    for (int i = 0; i < PREDICTIONS_SIZE; i++) {
	*prediction = *(predictions[i]);
	//std::cout << prediction->toString() << ' ' << (predictions[i])->toString() << std::endl;
	CPPUNIT_ASSERT_EQUAL( prediction->toString(), (predictions[i])->toString() );
    }

}

void PredictionTest::testGetSize()
{
    CPPUNIT_ASSERT( empty_prediction->size() == 0 );
    CPPUNIT_ASSERT( prediction1->size() == 1 );
    CPPUNIT_ASSERT( prediction2->size() == 1 );
    CPPUNIT_ASSERT( prediction3->size() == 1 );
    CPPUNIT_ASSERT( prediction23->size() == 2 );
    CPPUNIT_ASSERT( prediction13->size() == 2 );
    CPPUNIT_ASSERT( prediction12->size() == 2 );
    CPPUNIT_ASSERT( prediction123->size() == 3 );

    Prediction* pred = new Prediction();
    const size_t total = 100;
    for (size_t i = 0; i < total; i++) {
        pred->addSuggestion(*sugg3Ptr);
    }
    CPPUNIT_ASSERT( pred->size() == total );
    delete pred;
}

void PredictionTest::testGetSuggestion()
{
    const int SUGGESTIONS_SIZE = 3;
    // be sure that suggestions array contains 
    // suggestions in order of probability or 
    // this test will fail.
    Suggestion* suggestions[SUGGESTIONS_SIZE] = {
	sugg3Ptr,
	sugg2Ptr,
	sugg1Ptr 
    };
    for (int i = 0; i < SUGGESTIONS_SIZE; i++) {
	//std::cout << prediction123->getSuggestion(i) << ' ' << *(suggestions[i]) << std::endl;
	CPPUNIT_ASSERT_EQUAL( prediction123->getSuggestion(i),
			      *(suggestions[i]) );
    }
}

void PredictionTest::testAddSuggestion()
{
    // test that added suggestions are ordered correctly
    Prediction* pred = new Prediction();
    pred->addSuggestion(*sugg3Ptr);
    pred->addSuggestion(*sugg2Ptr);
    pred->addSuggestion(*sugg1Ptr);

    CPPUNIT_ASSERT( pred->size() == 3 );
    CPPUNIT_ASSERT( pred->getSuggestion(0) == *sugg3Ptr );
    CPPUNIT_ASSERT( pred->getSuggestion(1) == *sugg2Ptr );
    CPPUNIT_ASSERT( pred->getSuggestion(2) == *sugg1Ptr );

    delete pred;
}
