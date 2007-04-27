
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


#ifndef SOOTH_REVERSETOKENIZERTEST
#define SOOTH_REVERSETOKENIZERTEST

#include <cppunit/extensions/HelperMacros.h>

#include "core/tokenizer/reverseTokenizer.h"
#include "testStringSuite.h"

class ReverseTokenizerTest : public CppUnit::TestFixture { 
public:
    void setUp();
    void tearDown();

    void testConstructor();
    void testNextToken();
    void testCountTokens();
    void testLowercaseMode();
    void testProgress();
    void testRepeatability();

private:
    std::string blankspaces;
    std::string separators;

    TestStringSuite* stringSuite;

    CPPUNIT_TEST_SUITE( ReverseTokenizerTest );
    CPPUNIT_TEST( testConstructor            );
    CPPUNIT_TEST( testNextToken              );
    CPPUNIT_TEST( testCountTokens            );
    CPPUNIT_TEST( testLowercaseMode          );
    CPPUNIT_TEST( testProgress               );
    CPPUNIT_TEST( testRepeatability          );
    CPPUNIT_TEST_SUITE_END();
};

#endif // SOOTH_REVERSETOKENIZERTEST
