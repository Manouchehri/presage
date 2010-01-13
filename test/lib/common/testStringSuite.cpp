
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


#include "testStringSuite.h"

#include <iostream>
#include <assert.h>

////
// TestString
TestStringSuite::TestString::TestString(
    std::string str,
    std::vector<std::string> tokens     )
    : m_string    (str),
      m_tokens    (tokens)
{}

TestStringSuite::TestString::~TestString()
{}

const std::string TestStringSuite::TestString::getstr() const
{
    // DEBUG
    //std::cerr << "TestString::str() called" << std::endl;
    //std::cerr << "Returning string: " << m_string << std::endl;
    return m_string;
}

const int TestStringSuite::TestString::tokencount() const
{
    return m_tokens.size();
}

const std::string TestStringSuite::TestString::token(const size_t index) const
{
    assert(index < m_tokens.size());
    return m_tokens[index];
}

////
// TestStringSuite
TestStringSuite::TestStringSuite()
{
    std::string              str;
    std::vector<std::string> tokens;

    str = "foo bar. Foo, bar! Foo bar... Foobar";
    tokens.push_back("foo");
    tokens.push_back("bar");
    tokens.push_back("Foo");
    tokens.push_back("bar");
    tokens.push_back("Foo");
    tokens.push_back("bar");
    tokens.push_back("Foobar");
    testStrings.push_back(new TestString(str, tokens));

    tokens.clear();
    str = "foo bar. Foo!!";
    tokens.push_back("foo");
    tokens.push_back("bar");
    tokens.push_back("Foo");
    tokens.push_back("");
    testStrings.push_back(new TestString(str, tokens));

    tokens.clear();
    str = "FoO.\n \nBaR. fOoBaR\n.";
    tokens.push_back("FoO");
    tokens.push_back("BaR");
    tokens.push_back("fOoBaR");
    tokens.push_back("");
    testStrings.push_back(new TestString(str, tokens));

    tokens.clear();
    str = "foo bar foobar ";
    tokens.push_back("foo");
    tokens.push_back("bar");
    tokens.push_back("foobar");
    tokens.push_back("");
    testStrings.push_back(new TestString(str, tokens));

    tokens.clear();
    str = "frobnik ";
    tokens.push_back("frobnik");
    tokens.push_back("");
    testStrings.push_back(new TestString(str, tokens));

//// REVISIT:
// These two following strings will cause tests to fail due to the
// blankspace at the beginning whitespace.
// Need to fix ReverseTokenizer!
//
//    tokens.clear();
//    str = " foo bar foobar";
//    tokens.push_back("foo");
//    tokens.push_back("bar");
//    tokens.push_back("foobar");
//    testStrings.push_back(new TestString(str, tokens));
//
//    tokens.clear();
//    str = " foo bar foobar ";
//    tokens.push_back("foo");
//    tokens.push_back("bar");
//    tokens.push_back("foobar");
//    tokens.push_back("");
//    testStrings.push_back(new TestString(str, tokens));
////

    // DEBUG
    //std::cerr << "TestStringSuite::TestStringSuite()" << std::endl;
    //for (std::vector<TestString*>::iterator it = testStrings.begin();
    //	 it != testStrings.end();
    //	 it++) {
    //	std::cerr << (*it)->getstr() << std::endl;
    //}
    
    iter = testStrings.begin();
}


TestStringSuite::~TestStringSuite()
{
    for (iter = testStrings.begin();
	 iter != testStrings.end();
	 iter++) {
	delete *iter;
    }
}

bool TestStringSuite::hasMoreTestStrings() const
{
    return ( iter == testStrings.end() ? false : true );
}

TestStringSuite::TestString* TestStringSuite::nextTestString()
{
    return *(iter++);
}

TestStringSuite::TestString* TestStringSuite::currentTestString()
{
    // DEBUG
    //std::cerr << "Entered currentTestString()" << std::endl;
    //std::cerr << "Returning pointer: " << *iter << std::endl;
    return *iter;
}
