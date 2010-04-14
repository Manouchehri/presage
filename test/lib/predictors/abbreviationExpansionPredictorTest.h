
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


#ifndef PRESAGE_ABBREVIATIONEXPANSIONPREDICTORTEST
#define PRESAGE_ABBREVIATIONEXPANSIONPREDICTORTEST

#include <cppunit/extensions/HelperMacros.h>

#include <predictors/abbreviationExpansionPredictor.h>

#include "predictorsTestMockObjects.h"
#include "predictorsTestFixture.h"


/** Test abbreviationExpansionPredictor.
 * 
 * The idea is to provide mock HistoryTracker and mock Profile classes
 * to supply controlled values to the predictor and test that the
 * returned predictions are as expected.
 *
 */
//class AbbreviationExpansionPredictorTest : public CppUnit::TestFixture {

class AbbreviationExpansionPredictorTest : public PredictorsTestFixture {
  public: 
    AbbreviationExpansionPredictorTest();

    void setUp();
    void tearDown();
    
    void testSimpleExpansion();
    void testLongExpansion();
    void testUnknownAbbreviation();

  private:
    /** Converts array of configuration values config to a configuration map.
     */
    Configuration* prepareConfiguration(const char* config[]) const;
    
    Predictor* createPredictor(Configuration* configuration, ContextTracker* ct) const;

    std::string STANDARD_ABBREVIATION_FILE;

    CPPUNIT_TEST_SUITE( AbbreviationExpansionPredictorTest );
    CPPUNIT_TEST( testSimpleExpansion                      );
    CPPUNIT_TEST( testLongExpansion                        );
    CPPUNIT_TEST( testUnknownAbbreviation                  );
    CPPUNIT_TEST_SUITE_END();
};


#endif // PRESAGE_ABBREVIATIONEXPANSIONPREDICTORTEST
