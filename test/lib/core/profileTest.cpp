
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
    configuration = profile->get_configuration();
}

void ProfileTest::tearDown()
{
    delete configuration;
    delete profile;
    delete profileXmlDoc;
}

void ProfileTest::testGetConfig()
{
    Variable* var;
    
    var = configuration->find ("Presage.ContextTracker.MAX_BUFFER_SIZE");
    CPPUNIT_ASSERT(var->get_value() == "1024");
    delete var;

    var = configuration->find ("Presage.Selector.SUGGESTIONS");
    CPPUNIT_ASSERT(var->get_value() == "6");
    delete var;

    var = configuration->find ("Presage.Selector.GREEDY_SUGGESTION_THRESHOLD");
    CPPUNIT_ASSERT(var->get_value() == "0");
    delete var;


    var = configuration->find ("Presage.Selector.REPEAT_SUGGESTIONS");
    CPPUNIT_ASSERT(var->get_value() == "no");
    delete var;
    
    var = configuration->find ("Presage.Plugins.SmoothedNgramPlugin.DBFILENAME");
    CPPUNIT_ASSERT(var->get_value() == "database_en.db");
    delete var;

    var = configuration->find ("Presage.Plugins.SmoothedNgramPlugin.MAX_PARTIAL_PREDICTION_SIZE");
    CPPUNIT_ASSERT(var->get_value() == "40");
    delete var;
}

void ProfileTest::testGetNonExistantConfig()
{
    std::cout << "void ProfileTest::testGetNonExistantConfig()" << std::endl;

    CPPUNIT_ASSERT_THROW(configuration->find (""), Configuration::ConfigurationException);

    CPPUNIT_ASSERT_THROW(configuration->find ("foo"), Configuration::ConfigurationException);

    CPPUNIT_ASSERT_THROW(configuration->find ("bar"), Configuration::ConfigurationException);
}
