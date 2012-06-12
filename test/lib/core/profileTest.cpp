
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
    std::string profileFile = 
	static_cast<std::string>(getenv("srcdir"))
	+ '/' + "profileTest.xml";
    profile = new Profile(profileFile);
    configuration = new Configuration();
    profile->read_into_configuration(configuration);
}

void ProfileTest::tearDown()
{
    delete configuration;
    delete profile;
}

void ProfileTest::testGetConfig()
{
    Variable* var;
    
    var = configuration->find ("Presage.ContextTracker.MAX_BUFFER_SIZE");
    CPPUNIT_ASSERT(var->get_value() == "1024");

    var = configuration->find ("Presage.Selector.SUGGESTIONS");
    CPPUNIT_ASSERT(var->get_value() == "6");

    var = configuration->find ("Presage.Selector.GREEDY_SUGGESTION_THRESHOLD");
    CPPUNIT_ASSERT(var->get_value() == "0");

    var = configuration->find ("Presage.Selector.REPEAT_SUGGESTIONS");
    CPPUNIT_ASSERT(var->get_value() == "no");
    
    var = configuration->find ("Presage.Predictors.SmoothedNgramPredictor.DBFILENAME");
    CPPUNIT_ASSERT(var->get_value() == "database_en.db");

    var = configuration->find ("Presage.Predictors.SmoothedNgramPredictor.MAX_PARTIAL_PREDICTION_SIZE");
    CPPUNIT_ASSERT(var->get_value() == "40");
}

void ProfileTest::testGetNonExistantConfig()
{
    CPPUNIT_ASSERT_THROW(configuration->find (""), Configuration::ConfigurationException);

    CPPUNIT_ASSERT_THROW(configuration->find ("foo"), Configuration::ConfigurationException);

    CPPUNIT_ASSERT_THROW(configuration->find ("bar"), Configuration::ConfigurationException);
}

void ProfileTest::testNonExistantProfile()
{
    // hopefully a file with the following name will not exists
    const std::string wacky_profile("this_IS_a_wAckY_profileName.xml");

    Profile* local_profile = new Profile (wacky_profile);
    Configuration* local_config = new Configuration ();

    CPPUNIT_ASSERT_NO_THROW( local_profile->read_into_configuration (local_config) );

    delete local_config;
    delete local_profile;
}
