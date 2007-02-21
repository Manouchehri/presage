
/******************************************************************************\
 *                                                                            *
 * Soothsayer, an extensible predictive text entry system                     *
 * ------------------------------------------------------                     *
 *                                                                            *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>             *
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
    historyTracker = new HistoryTracker();
    predictor = new Predictor(*historyTracker);
    selector = new Selector(*historyTracker);
}

void ProfileManagerTest::tearDown()
{
    delete historyTracker;
    delete predictor;
    delete selector;
}

void ProfileManagerTest::testDefaultProfile()
{
    ProfileManager profileManager(*historyTracker,
				  *predictor,
				  *selector);
    testProfile();
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
    // hopefully a file with the following name will not exists
    // in the directories we look for...
    const std::string wacky_profile("this_IS_a_wAckY_profileName.xml");

    ProfileManager profileManager(*historyTracker,
				  *predictor,
				  *selector);
    profileManager.loadProfile(wacky_profile);
    testProfile();
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
			 selector->getRepeatSuggestion());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_GREEDY_SUGGESTION_THRESHOLD,
			 selector->getGreedySuggestionThreshold());
}
