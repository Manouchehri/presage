
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


#ifndef PRESAGE_NEWSMOOTHEDNGRAMPREDICTORTEST
#define PRESAGE_NEWSMOOTHEDNGRAMPREDICTORTEST

#include <cppunit/extensions/HelperMacros.h>

#include <predictors/smoothedNgramPredictor.h>

/** Test SmoothedNgramPredictor.
 *
 * this test doesn't follow the pattern of test
 * SmoothedNgramPredictorTest: i.e. it doesn't reuse the infrastructure
 * provided by the predictor test fixture, because that infrastructure
 * was not designed to accomodate predictors that are able to learn.
 * 
 */
class NewSmoothedNgramPredictorTest : public CppUnit::TestFixture {
public: 
    void setUp();
    void tearDown();
    
    void testOnlineLearning();
    void testOfflineLearning();
    void testFilter();

private:
    Configuration*  config;
    std::stringstream* stream;
    PresageCallback* callback;
    ContextTracker* ct;
    PredictorRegistry* predictorRegistry;
    Predictor*         predictor;

    static const char*  DATABASE;
    static const size_t CARDINALITY;
    static const bool   READ_WRITE_MODE;

    static const int SIZE;

    CPPUNIT_TEST_SUITE( NewSmoothedNgramPredictorTest );
    CPPUNIT_TEST( testOnlineLearning );
    CPPUNIT_TEST( testOfflineLearning );
    CPPUNIT_TEST( testFilter );
    CPPUNIT_TEST_SUITE_END();
};


#endif // PRESAGE_NEWSMOOTHEDNGRAMPREDICTORTEST
