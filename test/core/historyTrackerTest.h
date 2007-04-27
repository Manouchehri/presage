
/******************************************************************************\
 *                                                                            *
 * Soothsayer, an extensible predictive text entry system                     *
 * ------------------------------------------------------                     *
 *                                                                            *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>             *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*
 *                                                                            *
\******************************************************************************/        


#ifndef SOOTH_HISTORYTRACKERTEST
#define SOOTH_HISTORYTRACKERTEST

#include <cppunit/extensions/HelperMacros.h>

#include <core/historyTracker.h>
#include <tokenizer/testStringSuite.h>

class HistoryTrackerTest : public CppUnit::TestFixture { 
public:
    void setUp();
    void tearDown();
    
    void testConstructor();
    void testUpdate();
    void testGetPrefix();
    void testGetToken();

    void testGetFutureStream();
    void testGetPastStream();

    void testToString();

    void testGetMaxBufferSize();
    void testSetMaxBufferSize();

private:
    HistoryTracker*  historyTracker;
    TestStringSuite* testStringSuite;
    
    CPPUNIT_TEST_SUITE( HistoryTrackerTest );
    CPPUNIT_TEST( testConstructor          );
    CPPUNIT_TEST( testUpdate               );
    CPPUNIT_TEST( testGetPrefix            );
    CPPUNIT_TEST( testGetToken             );
    CPPUNIT_TEST( testGetFutureStream      );
    CPPUNIT_TEST( testGetPastStream        );
    CPPUNIT_TEST( testToString             );
    CPPUNIT_TEST( testGetMaxBufferSize     );
    CPPUNIT_TEST( testSetMaxBufferSize     );
    CPPUNIT_TEST_SUITE_END();
};

#endif // SOOTH_HISTORYTRACKERTEST
