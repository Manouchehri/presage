
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


#include "contextChangeDetectorTest.h"

#include "core/charsets.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ContextChangeDetectorTest );

void ContextChangeDetectorTest::setUp()
{
    detector = new ContextChangeDetector(DEFAULT_WORD_CHARS,
					 DEFAULT_SEPARATOR_CHARS,
					 DEFAULT_BLANKSPACE_CHARS,
					 DEFAULT_CONTROL_CHARS );
}

void ContextChangeDetectorTest::tearDown()
{
    delete detector;
}

void ContextChangeDetectorTest::testCharChanges()
{
    std::cerr << "ContextChangeDetectorTest::testCharChanges()" << std::endl;

    detector->set_sliding_window_size("10");

    const std::string line   = "foo bar foobar, foo   bar! Foobar foo bar... foobar. ";
    const std::string change = "10010001000000100001000001000000010001000100000000010";

    for (size_t i = 0; i < line.size(); i++) {

	bool expected = (change[i] == '0' ? false : true);
	
	std::string stream = line.substr(0, i+1);
	std::cerr << "Stream: " << stream << std::endl;

	std::stringstream ss;
	ss << "Error detected at: " << stream << '|';
	CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), expected, detector->context_change(stream));
	detector->update_sliding_window(stream);
    }
}

void ContextChangeDetectorTest::testBlockChanges()
{
    std::cerr << "ContextChangeDetectorTest::testBlockChanges()" << std::endl;

    std::string str = "The quick brown fox jumped over the lazy dog";

    detector->set_sliding_window_size("10");

    // sliding window is now empty, so new string will trigger a context change
    CPPUNIT_ASSERT_EQUAL(true,  detector->context_change("The quick"));

    // update detector
    detector->update_sliding_window("The quick");

    // there was no change since last time, should be false
    CPPUNIT_ASSERT_EQUAL(false, detector->context_change("The quick"));
    // no changes, should be false
    CPPUNIT_ASSERT_EQUAL(false, detector->context_change("The quick"));
    // still no changes, should still be false
    CPPUNIT_ASSERT_EQUAL(false, detector->context_change("The quick"));
}
