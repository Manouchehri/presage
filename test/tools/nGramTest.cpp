
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


#include "nGramTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( NgramTest );

void NgramTest::setUp()
{
    unigramFoo = new Ngram(1);
    unigramFoo->setNgram(0, "foo");
    unigramBar = new Ngram(1);
    unigramBar->setNgram(0, "bar");
    
    bigramFooBar = new Ngram(2);
    bigramFooBar->setNgram(0, "foo");
    bigramFooBar->setNgram(1, "bar");
    bigramBarFoo = new Ngram(2);
    bigramBarFoo->setNgram(0, "bar");
    bigramBarFoo->setNgram(1, "foo");
    
    trigramFooBarFoobar = new Ngram(3);
    trigramFooBarFoobar->setNgram(0, "foo");
    trigramFooBarFoobar->setNgram(1, "bar");
    trigramFooBarFoobar->setNgram(2, "foobar");
    trigramBarFooBarfoo = new Ngram(3);
    trigramBarFooBarfoo->setNgram(0, "bar");
    trigramBarFooBarfoo->setNgram(1, "foo");
    trigramBarFooBarfoo->setNgram(2, "barfoo");
}

void NgramTest::tearDown()
{
    delete unigramFoo;
    delete unigramBar;

    delete bigramFooBar;
    delete bigramBarFoo;

    delete trigramFooBarFoobar;
    delete trigramBarFooBarfoo;
}

void NgramTest::testConstructor()
{}

void NgramTest::testLessOperator()
{
    CPPUNIT_ASSERT( *unigramBar < *unigramFoo );
    CPPUNIT_ASSERT( *bigramBarFoo < *bigramFooBar );
    CPPUNIT_ASSERT( *trigramBarFooBarfoo < *trigramFooBarFoobar );
}

void NgramTest::testToString()
{
    CPPUNIT_ASSERT( "<foo> " == unigramFoo->toString() );
    CPPUNIT_ASSERT( "<bar> " == unigramBar->toString() );
    CPPUNIT_ASSERT( "<foo> <bar> " == bigramFooBar->toString() );
    CPPUNIT_ASSERT( "<bar> <foo> " == bigramBarFoo->toString() );
    CPPUNIT_ASSERT( "<foo> <bar> <foobar> " == trigramFooBarFoobar->toString() );
    CPPUNIT_ASSERT( "<bar> <foo> <barfoo> " == trigramBarFooBarfoo->toString() );
}
