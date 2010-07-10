
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


#ifndef PRESAGE_SUGGESTIONTEST
#define PRESAGE_SUGGESTIONTEST

#include <cppunit/extensions/HelperMacros.h>

#include <core/suggestion.h>

class SuggestionTest : public CppUnit::TestFixture { 
public:
    void setUp();
    void tearDown();

    void testConstructor();
    void testOperatorEqual();
    void testOperatorNotEqual();
    void testOperatorMinus();
    void testGetSetMethods();

private:
    Suggestion* sugg1Ptr;
    Suggestion* sugg2Ptr;
    Suggestion* sugg3Ptr;
    Suggestion* sugg4Ptr;
    Suggestion* sugg5Ptr;
    Suggestion* sugg6Ptr;
    Suggestion* sugg7Ptr;
    Suggestion* sugg8Ptr;

    CPPUNIT_TEST_SUITE( SuggestionTest );
    CPPUNIT_TEST( testConstructor      );
    CPPUNIT_TEST( testOperatorEqual    );
    CPPUNIT_TEST( testOperatorNotEqual );
    CPPUNIT_TEST( testOperatorMinus    );
    CPPUNIT_TEST( testGetSetMethods    );
    CPPUNIT_TEST_SUITE_END();
};

#endif // PRESAGE_SUGGESTIONTEST
