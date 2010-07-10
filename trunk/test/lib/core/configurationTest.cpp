
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


#include "configurationTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigurationTest );

void ConfigurationTest::setUp()
{
    configuration = new Configuration();
}

void ConfigurationTest::tearDown()
{
    delete configuration;
}

void ConfigurationTest::test_get_set()
{
    const unsigned int SIZE = 3;
    const char* test_variables[SIZE] = {
	"foo",
	"foo.bar",
	"foo.bar.foobar"
    };

    for (int loop = 0; loop < 3; loop++) {
        for (unsigned int i = 0; i < SIZE; i++) {
            configuration->insert (test_variables[i], test_variables[i]);
            CPPUNIT_ASSERT(configuration->find (test_variables[i])->get_value ().compare(test_variables[i]) == 0);
        }
    }
}
