
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


#ifndef PRESAGE_PREDICTIONTEST
#define PRESAGE_PREDICTIONTEST

#include <cppunit/extensions/HelperMacros.h>

#include <core/prediction.h>

class PredictionTest : public CppUnit::TestFixture { 
public:
    void setUp();
    void tearDown();
    
    void testAssignmentOperator();
    void testGetSize();
    void testGetSuggestion();
    void testAddSuggestion();

private:
    Suggestion* sugg1Ptr;
    Suggestion* sugg2Ptr;
    Suggestion* sugg3Ptr;

    Prediction* empty_prediction;
    Prediction* prediction1;
    Prediction* prediction2;
    Prediction* prediction3;
    Prediction* prediction23;
    Prediction* prediction13;
    Prediction* prediction12;
    Prediction* prediction123;
    
    CPPUNIT_TEST_SUITE( PredictionTest );
    CPPUNIT_TEST( testAssignmentOperator );
    CPPUNIT_TEST( testGetSize );
    CPPUNIT_TEST( testGetSuggestion );
    CPPUNIT_TEST( testAddSuggestion );
    CPPUNIT_TEST_SUITE_END();
};

#endif // PRESAGE_PREDICTIONTEST
