
/*********************************************************
 *  Soothsayer, an extensible predictive text entry system
 *  ------------------------------------------------------
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

#include "profileTest.h"

#ifdef STDC_HEADERS
# include <stdlib.h> // for getenv()
#endif

CPPUNIT_TEST_SUITE_REGISTRATION( ProfileTest );

void ProfileTest::setUp()
{
    profileXmlDoc = new TiXmlDocument;
    CPPUNIT_ASSERT(profileXmlDoc);
    
    bool readOk = false;
    std::string profileFile = 
	static_cast<std::string>(getenv("srcdir"))
	+ '/' + "profileTest.xml";
    readOk = profileXmlDoc->LoadFile(profileFile.c_str());
    CPPUNIT_ASSERT(readOk);

    profile = new Profile(profileXmlDoc);
}

void ProfileTest::tearDown()
{
    delete profile;
    delete profileXmlDoc;
}

void ProfileTest::testGetConfig()
{
    Variable* var;
    
    var = new Variable("Soothsayer.ContextTracker.MAX_BUFFER_SIZE");
    CPPUNIT_ASSERT(profile->getConfig(*var) == "1024");
    delete var;

    var = new Variable("Soothsayer.Selector.SUGGESTIONS");
    CPPUNIT_ASSERT(profile->getConfig(*var) == "6");
    delete var;

    var = new Variable("Soothsayer.Selector.GREEDY_SUGGESTION_THRESHOLD");
    CPPUNIT_ASSERT(profile->getConfig(*var) == "0");
    delete var;


    var = new Variable("Soothsayer.Selector.REPEAT_SUGGESTIONS");
    CPPUNIT_ASSERT(profile->getConfig(*var) == "no");
    delete var;
    
    var = new Variable("Soothsayer.Plugins.SmoothedNgramPlugin.DBFILENAME");
    CPPUNIT_ASSERT(profile->getConfig(*var) == "database_en.db");
    delete var;

    var = new Variable("Soothsayer.Plugins.SmoothedNgramPlugin.MAX_PARTIAL_PREDICTION_SIZE");
    CPPUNIT_ASSERT(profile->getConfig(*var) == "40");
    delete var;
}

void ProfileTest::testGetNonExistantConfig()
{
    std::cout << "void ProfileTest::testGetNonExistantConfig()" << std::endl;

    Variable* var = new Variable();
    CPPUNIT_ASSERT_THROW(profile->getConfig(*var), Profile::ProfileException);
    delete var;

    var = new Variable("foo");
    CPPUNIT_ASSERT_THROW(profile->getConfig(*var), Profile::ProfileException);
    delete var;

    var = new Variable("bar");
    CPPUNIT_ASSERT_THROW(profile->getConfig(*var), Profile::ProfileException);
    delete var;
}
