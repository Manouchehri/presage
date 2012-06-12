
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
					 DEFAULT_CONTROL_CHARS,
					 false);
}

void ContextChangeDetectorTest::tearDown()
{
    delete detector;
}

void ContextChangeDetectorTest::testCharChanges()
{
    detector->set_sliding_window_size("10");

    const std::string line   = "foo bar foobar, foo   bar! Foobar foo bar... foobar. ";
    const std::string change = "10010001000000100001000001000000010001000100000000010";

    for (size_t i = 0; i < line.size(); i++) {

	bool expected = (change[i] == '0' ? false : true);
	
	std::string stream = line.substr(0, i+1);

	std::stringstream ss;
	ss << "Error detected at: " << stream << '|';
	CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), expected, detector->context_change(stream));
	detector->update_sliding_window(stream);
    }
}

void ContextChangeDetectorTest::testBlockChanges()
{
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

void ContextChangeDetectorTest::testGetChange()
{
    std::string str = "The quick brown fox jumped over the lazy dog";

    detector->set_sliding_window_size("10");

    // when sliding window is empty, change is equal to stream
    CPPUNIT_ASSERT_EQUAL(detector->change(str), str);

    // when sliding window contains part of stream, change is equal to text that is in stream and not in sliding window
    detector->update_sliding_window ("The quick bro");
    CPPUNIT_ASSERT_EQUAL(detector->get_sliding_window(), std::string(" quick bro"));
    CPPUNIT_ASSERT_EQUAL(detector->change(str), std::string("wn fox jumped over the lazy dog"));

    detector->update_sliding_window ("The quick brow");
    CPPUNIT_ASSERT_EQUAL(detector->get_sliding_window(), std::string("quick brow"));
    CPPUNIT_ASSERT_EQUAL(detector->change(str), std::string("n fox jumped over the lazy dog"));

    detector->update_sliding_window ("The quick brown");
    CPPUNIT_ASSERT_EQUAL(detector->get_sliding_window(), std::string("uick brown"));
    CPPUNIT_ASSERT_EQUAL(detector->change(str), std::string(" fox jumped over the lazy dog"));

    detector->update_sliding_window ("The quick brown ");
    CPPUNIT_ASSERT_EQUAL(detector->get_sliding_window(), std::string("ick brown "));
    CPPUNIT_ASSERT_EQUAL(detector->change(str), std::string("fox jumped over the lazy dog"));

    // remove last char from stream
    detector->update_sliding_window ("The quick brown");
    CPPUNIT_ASSERT_EQUAL(detector->get_sliding_window(), std::string("uick brown"));
    CPPUNIT_ASSERT_EQUAL(detector->change(str), std::string(" fox jumped over the lazy dog"));

    // remove last char from stream again
    detector->update_sliding_window ("The quick brow");
    CPPUNIT_ASSERT_EQUAL(detector->get_sliding_window(), std::string("quick brow"));
    CPPUNIT_ASSERT_EQUAL(detector->change(str), std::string("n fox jumped over the lazy dog"));
}
