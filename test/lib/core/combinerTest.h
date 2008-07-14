
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


#ifndef PRESAGE_COMBINERTEST
#define PRESAGE_COMBINERTEST

#include <cppunit/extensions/HelperMacros.h>

#include <core/combiner.h>

class CombinerTest : public CppUnit::TestFixture { 
public:
    void setUp();
    void tearDown();
    
    class ConcreteCombiner : public Combiner {
      public:
        // provide dummy implementation of pure virtual method
        Prediction combine(const std::vector<Prediction>&) {
            Prediction result; return result;
        }
        // wrap around base class protected method filter
        Prediction filter(const Prediction& prediction) const {
            return Combiner::filter(prediction);
        }
    };
    
    void testFilter();

private:
    Suggestion* sugg1Ptr;
    Suggestion* sugg2Ptr;
    Suggestion* sugg3Ptr;
    Suggestion* sugg4Ptr;
    Suggestion* sugg5Ptr;
    Suggestion* sugg6Ptr;
    Suggestion* sugg123Ptr;
    Suggestion* sugg456Ptr;

    Prediction* prediction1;
    Prediction* prediction2;
    Prediction* prediction3;
    Prediction* prediction4;
    Prediction* prediction5;
    Prediction* expected_prediction1;
    Prediction* expected_prediction2;
    Prediction* expected_prediction3;
    Prediction* expected_prediction4;
    Prediction* expected_prediction5;
    
    CPPUNIT_TEST_SUITE( CombinerTest );
    CPPUNIT_TEST( testFilter );
    CPPUNIT_TEST_SUITE_END();
};

#endif // PRESAGE_COMBINERTEST
