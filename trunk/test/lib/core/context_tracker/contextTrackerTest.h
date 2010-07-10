
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


#ifndef PRESAGE_CONTEXTTRACKERTEST
#define PRESAGE_CONTEXTTRACKERTEST

#include <cppunit/extensions/HelperMacros.h>

#include "core/profileManager.h"
#include "core/context_tracker/contextTracker.h"
#include "core/predictorRegistry.h"

#include "../../common/testStringSuite.h"

class ContextTrackerTest : public CppUnit::TestFixture { 
public:
    void setUp();
    void tearDown();
    
    void testConstructor();
    void testGetPrefix();
    void testGetToken();

    void testGetFutureStream();
    void testGetPastStream();

    void testToString();

    void testContextChange();
    void testCumulativeContextChange();

private:
    TestStringSuite* testStringSuite;

    ContextTracker*  contextTracker;
    Configuration*   configuration;
    PredictorRegistry*  predictorRegistry;
    
    CPPUNIT_TEST_SUITE( ContextTrackerTest );
    CPPUNIT_TEST( testConstructor          );
    CPPUNIT_TEST( testGetPrefix            );
    CPPUNIT_TEST( testGetToken             );
    CPPUNIT_TEST( testGetFutureStream      );
    CPPUNIT_TEST( testGetPastStream        );
    CPPUNIT_TEST( testToString             );
    CPPUNIT_TEST( testContextChange        );
    CPPUNIT_TEST( testCumulativeContextChange );
    CPPUNIT_TEST_SUITE_END();

};

#endif // PRESAGE_CONTEXTTRACKERTEST
