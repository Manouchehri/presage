
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


#include "suggestionTest.h"
#include "core/suggestion.h"

#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION( SuggestionTest );

void SuggestionTest::setUp()
{
    sugg1Ptr = new Suggestion();
    sugg2Ptr = new Suggestion( "test" );
    sugg3Ptr = new Suggestion( "test", 0.5 );
    sugg4Ptr = new Suggestion( "case", 0.7 );
    sugg5Ptr = new Suggestion( "foo", 0.1 );
    sugg6Ptr = new Suggestion( "bar", 0.3 );
    sugg7Ptr = new Suggestion( "base", 0.7 );
    sugg8Ptr = new Suggestion( "base", 0.7 );
}

void SuggestionTest::tearDown()
{
    delete sugg1Ptr;
    delete sugg2Ptr;
    delete sugg3Ptr;
    delete sugg4Ptr;
    delete sugg5Ptr;
    delete sugg6Ptr;
    delete sugg7Ptr;
    delete sugg8Ptr;
}

void SuggestionTest::testConstructor()
{
    CPPUNIT_ASSERT( sugg1Ptr->getWord() == "" );
    CPPUNIT_ASSERT( sugg1Ptr->getProbability() == 0.0 );
    CPPUNIT_ASSERT( sugg2Ptr->getWord() == "test" );
    CPPUNIT_ASSERT( sugg2Ptr->getProbability() == 0.0 );
    CPPUNIT_ASSERT( sugg3Ptr->getWord() == "test" );
    CPPUNIT_ASSERT( sugg3Ptr->getProbability() == 0.5 );
    CPPUNIT_ASSERT( sugg4Ptr->getWord() == "case" );
    CPPUNIT_ASSERT( sugg4Ptr->getProbability() == 0.7 );
}

void SuggestionTest::testOperatorEqual()
{
    //CPPUNIT_ASSERT( *sugg2Ptr == *sugg3Ptr );
    CPPUNIT_ASSERT( !( *sugg3Ptr == *sugg4Ptr ) );
    CPPUNIT_ASSERT_EQUAL(*sugg7Ptr, *sugg8Ptr);
    CPPUNIT_ASSERT_EQUAL(*sugg5Ptr, *sugg5Ptr);
}

void SuggestionTest::testOperatorNotEqual()
{
    CPPUNIT_ASSERT(*sugg2Ptr != *sugg3Ptr);
    CPPUNIT_ASSERT(*sugg3Ptr != *sugg4Ptr);
    CPPUNIT_ASSERT(*sugg4Ptr != *sugg5Ptr);
    CPPUNIT_ASSERT(*sugg5Ptr != *sugg6Ptr);
    CPPUNIT_ASSERT(!(*sugg7Ptr != *sugg8Ptr));
    CPPUNIT_ASSERT(!(*sugg5Ptr != *sugg5Ptr));
}

void SuggestionTest::testOperatorMinus()
{
    CPPUNIT_ASSERT( *sugg1Ptr < *sugg2Ptr );
    CPPUNIT_ASSERT( !( *sugg1Ptr < *sugg1Ptr ) );
    CPPUNIT_ASSERT( *sugg2Ptr < *sugg3Ptr );
    CPPUNIT_ASSERT( *sugg7Ptr < *sugg4Ptr );
    CPPUNIT_ASSERT( !( *sugg4Ptr < *sugg3Ptr ) );
}

void SuggestionTest::testGetSetMethods()
{
    CPPUNIT_ASSERT( sugg5Ptr->getWord() == "foo" );
    CPPUNIT_ASSERT( sugg6Ptr->getWord() == "bar" );
    CPPUNIT_ASSERT( sugg5Ptr->getProbability() == 0.1 );
    CPPUNIT_ASSERT( sugg6Ptr->getProbability() == 0.3 );
	
    sugg5Ptr->setWord( sugg6Ptr->getWord() );
    CPPUNIT_ASSERT( sugg5Ptr->getWord() == sugg6Ptr->getWord() );

    sugg5Ptr->setProbability( sugg6Ptr->getProbability() );
    CPPUNIT_ASSERT( sugg5Ptr->getProbability() == sugg6Ptr->getProbability() );
}

