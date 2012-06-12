
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

#include "core/predictorRegistry.h"
#include "core/context_tracker/contextTracker.h"
#include "core/predictorActivator.h"
#include "core/selector.h"

#include <fstream>

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( ProfileManagerTest );

void ProfileManagerTest::setUp()
{
    profileManager = new ProfileManager("non-existant-profile.xml");
    configuration  = profileManager->get_configuration ();
}

void ProfileManagerTest::tearDown()
{
    delete profileManager;
}

void ProfileManagerTest::testProfile()
{
    // test that some default variables are there in retrieved config
    CPPUNIT_ASSERT(configuration->find (ProfileManager::LOGGER) != 0);
    CPPUNIT_ASSERT(configuration->find (PredictorRegistry::PREDICTORS) != 0);
    CPPUNIT_ASSERT(configuration->find (ContextTracker::LOGGER) != 0);
    CPPUNIT_ASSERT(configuration->find (Selector::LOGGER) != 0);
    CPPUNIT_ASSERT(configuration->find (PredictorActivator::LOGGER) != 0);
}

void ProfileManagerTest::testCustomProfile()
{
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
