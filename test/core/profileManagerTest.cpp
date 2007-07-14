
/******************************************************************************\
 *                                                                            *
 * Soothsayer, an extensible predictive text entry system                     *
 * ------------------------------------------------------                     *
 *                                                                            *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>             *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*
 *                                                                            *
\******************************************************************************/        

#include <profileManagerTest.h>

CPPUNIT_TEST_SUITE_REGISTRATION( ProfileManagerTest );


void ProfileManagerTest::setUp()
{
    profileManager = new ProfileManager();
    profile        = 0;
    
    historyTracker = 0;
    selector       = 0;
    predictor      = 0;
}

void ProfileManagerTest::tearDown()
{
    delete predictor;
    delete selector;
    delete historyTracker;

    delete profile;
    delete profileManager;
}

void ProfileManagerTest::testDefaultProfile()
{

    std::cout << "ProfileManagerTest::testDefaultProfile()" << std::endl;
    profile = profileManager->getProfile();

    historyTracker = new HistoryTracker(profile);
    selector = new Selector(profile, historyTracker);
    predictor = new Predictor(profile, historyTracker);

    std::cout << "ProfileManagerTest: before testProfile()" << std::endl;
    testProfile();
    std::cout << "Exiting ProfileManagerTest::testDefaultProfile( ...)" << std::endl;

}

void ProfileManagerTest::testCustomProfile()
{

    // REVISIT: this test is disabled because it currently fails!
    // TODO: implement support for loading separate profiles from 
    //       different locations

//      ProfileManager profileManager(*historyTracker,
//  				  *predictor,
//  				  *selector);
//      std::string custom_profile("custom.xml");
//      profileManager.buildProfile(custom_profile);
//      profileManager.saveProfile();
//      profileManager.loadProfile(custom_profile);

//      testProfile();
}

void ProfileManagerTest::testNonExistantProfile()
{
/*
    std::cout << "ProfileManagerTest::testNonExistantProfile()" << std::endl;

    // hopefully a file with the following name will not exists
    // in the directories we look for...
    const std::string wacky_profile("this_IS_a_wAckY_profileName.xml");

    profileManager->loadProfile(wacky_profile);
    profile = profileManager->getProfile();

    historyTracker = new HistoryTracker(profile);
    selector = new Selector(profile, historyTracker);
    predictor = new Predictor(profile, historyTracker);

    testProfile();
*/
}

void ProfileManagerTest::testProfile()
{
    // test init historyTracker
    CPPUNIT_ASSERT_EQUAL(DEFAULT_MAX_BUFFER_SIZE,
			 historyTracker->getMaxBufferSize());

    // test init predictor
    CPPUNIT_ASSERT_EQUAL(DEFAULT_PREDICT_TIME,
			 predictor->getPredictTime());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_COMBINATION_METHOD,
			 predictor->getCombinationMethod());

    // test init selector
    CPPUNIT_ASSERT_EQUAL(DEFAULT_SUGGESTIONS,
			 selector->getSuggestions());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_REPEAT_SUGGESTION,
			 selector->getRepeatSuggestions());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_GREEDY_SUGGESTION_THRESHOLD,
			 selector->getGreedySuggestionThreshold());
}
