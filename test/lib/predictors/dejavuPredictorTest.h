
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


#ifndef PRESAGE_DEJAVUPREDICTORTEST
#define PRESAGE_DEJAVUPREDICTORTEST

#include <cppunit/extensions/HelperMacros.h>

#include <predictors/dejavuPredictor.h>

/** Test DejavuPredictor.
 * 
 */
class DejavuPredictorTest : public CppUnit::TestFixture {
public: 
    void setUp();
    void tearDown();
    
    void testPredict();

private:
    Configuration*  config;
    std::stringstream* stream;
    PresageCallback* callback;
    ContextTracker* ct;
    PredictorRegistry* predictorRegistry;

    static const int SIZE;
    static const char* NAME;
    static const char* PREDICTOR;
    static const char* LOGGER;
    static const char* TRIGGER;
    static const char* MEMORY;
    static const char* MEMORY_FILENAME;

    CPPUNIT_TEST_SUITE( DejavuPredictorTest );
    CPPUNIT_TEST( testPredict );
    CPPUNIT_TEST_SUITE_END();
};


#endif // PRESAGE_DEJAVUPREDICTORTEST
