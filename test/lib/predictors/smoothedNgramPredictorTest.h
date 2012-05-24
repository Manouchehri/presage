
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


#ifndef PRESAGE_SMOOTHEDNGRAMPREDICTORTEST
#define PRESAGE_SMOOTHEDNGRAMPREDICTORTEST

#include <cppunit/extensions/HelperMacros.h>

#include <predictors/smoothedNgramPredictor.h>

#include "predictorsTestFixture.h"

/** Test SmoothedNgramPredictor.
 * 
 * The idea is to provide mock HistoryTracker and mock Profile classes
 * to supply controlled values to the predictor and test that the
 * returned predictions are as expected.
 *
 * This test also creates a sqlite database with controlled values
 * necessary to generate predictions.
 *
 */
class SmoothedNgramPredictorTest : public PredictorsTestFixture {
public: 
    void setUp();
    void tearDown();
    
    void testUnigramWeight();
    void testBigramWeight();
    void testTrigramWeight();
    void testUnigramBigramWeight();
    void testUnigramTrigramWeight();
    void testBigramTrigramWeight();
    void testUnigramBigramTrigramWeight();

    void testMaxPartialPredictionSize();


private:
    Configuration* prepareConfiguration(const char* config[]) const;
    Predictor* createPredictor(Configuration*, ContextTracker*) const;

    /** Asserts prediction is as expected.
     */
    void assertCorrectPrediction(const char** config,
				 const char** history,
				 const size_t expected_prediction_size,
				 const std::string* expected_prediction_words) const;


    static const std::string DATABASE;
    static const size_t      CARDINALITY;
    static const bool        READ_WRITE_MODE;

    static const std::string FOO;
    static const std::string BAR;
    static const std::string FOOBAR;


    CPPUNIT_TEST_SUITE( SmoothedNgramPredictorTest );
    CPPUNIT_TEST( testUnigramWeight             );
    CPPUNIT_TEST( testBigramWeight              );
    CPPUNIT_TEST( testTrigramWeight             );
    CPPUNIT_TEST( testUnigramBigramWeight       );
    CPPUNIT_TEST( testUnigramTrigramWeight      );
    CPPUNIT_TEST( testBigramTrigramWeight       );
    CPPUNIT_TEST( testUnigramBigramTrigramWeight);
    CPPUNIT_TEST( testMaxPartialPredictionSize  );
    CPPUNIT_TEST_SUITE_END();
};


#endif // PRESAGE_SMOOTHEDNGRAMPREDICTORTEST
