
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


#ifndef PRESAGE_TESTSTRINGSUITE
#define PRESAGE_TESTSTRINGSUITE

#include <string>
#include <vector>

/** Contains a set of <string, tokens> pairs used in tokenizer tests.
 *
 * Provides accessor methods and pseudo iterators to reduce the amount
 * of code needed to use all the <string, tokens> pairs in the
 * tozenizer unit tests.
 *
 * This a typical usage:
 *
 *   while (stringSuite->hasMoreTestStrings()) {
 *       std::stringstream ss;
 *       assert(stringSuite->currentTestString() != 0);
 *       ss << stringSuite->currentTestString()->getstr();
 *       ForwardTokenizer tok(ss, blankspaces, separators);
 *       std::string str;
 *       for (i = 0; tok.hasMoreTokens(); i++) {
 *           str = stringSuite->currentTestString()->token(i);
 *           CPPUNIT_ASSERT_EQUAL( str, tok.nextToken() );
 *       }
 *       CPPUNIT_ASSERT_EQUAL( stringSuite->currentTestString()->tokencount(),
 *       		      i );
 *
 *       stringSuite->nextTestString();
 *   }
 *
 */
class TestStringSuite {
  public:

    class TestString {
      public:
	TestString(std::string,
		   std::vector<std::string>);
	~TestString();

	const std::string getstr              () const;
	const int         tokencount          () const;
	const std::string token(const size_t index) const;

      private:
	std::string              m_string;
	std::vector<std::string> m_tokens;

    };

    TestStringSuite();
    ~TestStringSuite();

    bool        hasMoreTestStrings() const;
    TestString* currentTestString();
    TestString* nextTestString();

  private:
    std::vector<TestString*>                 testStrings;
    std::vector<TestString*>::const_iterator iter;

};

#endif // PRESAGE_TESTSTRINGSUITE
