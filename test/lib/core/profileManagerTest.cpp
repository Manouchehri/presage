
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


#include "profileManagerTest.h"
#include "../common/stringstreamPresageCallback.h"

#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION( ProfileManagerTest );

void ProfileManagerTest::setUp()
{
    profileManager = new ProfileManager("non-existant-profile.xml");
    profile        = 0;
    configuration  = 0;

    pluginRegistry = 0;
    stream         = 0;
    callback       = 0;
    contextTracker = 0;
    selector       = 0;
    predictorActivator = 0;
}

void ProfileManagerTest::tearDown()
{
    delete predictorActivator;
    delete selector;
    delete contextTracker;
    delete callback;
    delete stream;

    delete profile;
    delete profileManager;
}

void ProfileManagerTest::testDefaultProfile()
{

    std::cout << "ProfileManagerTest::testDefaultProfile()" << std::endl;
    configuration = profileManager->get_configuration();
    configuration->print ();
    pluginRegistry = new PluginRegistry(configuration);

    callback = new StringstreamPresageCallback(*stream);
    contextTracker = new ContextTracker(configuration, pluginRegistry, callback);
    selector = new Selector(configuration, contextTracker);
    predictorActivator = new PredictorActivator(configuration, pluginRegistry, contextTracker);

    std::cout << "ProfileManagerTest: before testProfile()" << std::endl;
    testProfile();
    std::cout << "Exiting ProfileManagerTest::testDefaultProfile( ...)" << std::endl;

}

void ProfileManagerTest::testProfile()
{
    // test init contextTracker


    // test init predictorActivator
    CPPUNIT_ASSERT_EQUAL(DefaultProfile::DEFAULT_PREDICT_TIME,
			 predictorActivator->getPredictTime());
    CPPUNIT_ASSERT_EQUAL(DefaultProfile::DEFAULT_COMBINATION_POLICY,
			 predictorActivator->getCombinationPolicy());

    // test init selector
    CPPUNIT_ASSERT_EQUAL(DefaultProfile::DEFAULT_SUGGESTIONS,
			 selector->get_suggestions());
    CPPUNIT_ASSERT_EQUAL(DefaultProfile::DEFAULT_REPEAT_SUGGESTION,
			 selector->get_repeat_suggestions());
    CPPUNIT_ASSERT_EQUAL(DefaultProfile::DEFAULT_GREEDY_SUGGESTION_THRESHOLD,
			 selector->get_greedy_suggestion_threshold());
}

void ProfileManagerTest::testCustomProfile()
{
    std::cout << "ProfileManagerTest::testCustomProfile()" << std::endl;

    // create custom profile
    const std::string custom_profile = "custom_profile.xml";
    std::ofstream profile_stream(custom_profile.c_str());
    profile_stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
                   << "<Presage>\n"
		   << "    <ProfileManager>\n"
		   << "         <LOGGER>ERROR</LOGGER>\n"
		   << "         <AUTOPERSIST>false</AUTOPERSIST>\n"
		   << "    </ProfileManager>\n"
                   << "    <Custom>CUSTOM</Custom>\n"
                   << "</Presage>\n";
    profile_stream.close();

    delete profileManager;
    profileManager = new ProfileManager (custom_profile);

    configuration = profileManager->get_configuration();

    Value value = configuration->find ("Presage.Custom")->get_value ();

    CPPUNIT_ASSERT_EQUAL(std::string("CUSTOM"), value);

#ifdef HAVE_UNISTD_H
    // remove file
    int result = unlink(custom_profile.c_str());
    assert(result == 0);
#else
    // fail test
    std::string message = "Unable to remove custom profile file " + custom_profile;
    CPPUNIT_FAIL(message.c_str());
#endif

}
