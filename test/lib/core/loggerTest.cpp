
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


#include "loggerTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LoggerTest );

void LoggerTest::setUp()
{
}

void LoggerTest::tearDown()
{
}

void LoggerTest::testStandardErrorOutput()
{
    Logger<char>* logger = new Logger<char>("LoggerTest", std::cerr);
    
    *logger << "[LoggerTest] testStandardErrorOutput" << endl;
}

void LoggerTest::testFileOutput()
{

}

void LoggerTest::testSetLevelManipulator()
{
    Logger<char> logger("LoggerTest", std::cerr);

    logger << setlevel("EMERG") << "[LoggerTest] testSetLevelManipulator: EMERG\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::EMERG, logger.getLevel());
    logger << setlevel("FATAL") << "[LoggerTest] testSetLevelManipulator: FATAL\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::FATAL, logger.getLevel());
    logger << setlevel("ALERT") << "[LoggerTest] testSetLevelManipulator: ALERT\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::ALERT, logger.getLevel());
    logger << setlevel("CRIT")  << "[LoggerTest] testSetLevelManipulator: CRIT\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::CRIT, logger.getLevel());
    logger << setlevel("ERROR") << "[LoggerTest] testSetLevelManipulator: ERROR\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::ERROR, logger.getLevel());
    logger << setlevel("WARN")  << "[LoggerTest] testSetLevelManipulator: WARN\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::WARN, logger.getLevel());
    logger << setlevel("NOTICE")<< "[LoggerTest] testSetLevelManipulator: WARN\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::NOTICE, logger.getLevel());
    logger << setlevel("INFO")  << "[LoggerTest] testSetLevelManipulator: NOTICE\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::INFO, logger.getLevel());
    logger << setlevel("DEBUG") << "[LoggerTest] testSetLevelManipulator: DEBUG\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::DEBUG, logger.getLevel());
    logger << setlevel("ALL")   << "[LoggerTest] testSetLevelManipulator: ALL\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::ALL, logger.getLevel());
}

void LoggerTest::testCurrentLevelManipulator()
{
    Logger<char> logger("LoggerTest", std::cerr);

    logger << EMERG << "[LoggerTest] testCurrentLevelManipulator: EMERG\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::EMERG, logger.getCurrentLevel());
    logger << FATAL << "[LoggerTest] testCurrentLevelManipulator: FATAL\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::FATAL, logger.getCurrentLevel());
    logger << ALERT << "[LoggerTest] testCurrentLevelManipulator: ALERT\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::ALERT, logger.getCurrentLevel());
    logger << CRIT  << "[LoggerTest] testCurrentLevelManipulator: CRIT\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::CRIT, logger.getCurrentLevel());
    logger << ERROR << "[LoggerTest] testCurrentLevelManipulator: ERROR\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::ERROR, logger.getCurrentLevel());
    logger << WARN  << "[LoggerTest] testCurrentLevelManipulator: WARN\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::WARN, logger.getCurrentLevel());
    logger << NOTICE<< "[LoggerTest] testCurrentLevelManipulator: WARN\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::NOTICE, logger.getCurrentLevel());
    logger << INFO  << "[LoggerTest] testCurrentLevelManipulator: NOTICE\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::INFO, logger.getCurrentLevel());
    logger << DEBUG << "[LoggerTest] testCurrentLevelManipulator: DEBUG\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::DEBUG, logger.getCurrentLevel());
    logger << ALL   << "[LoggerTest] testCurrentLevelManipulator: ALL\n";
    CPPUNIT_ASSERT_EQUAL( Logger<char>::ALL, logger.getCurrentLevel());
}
