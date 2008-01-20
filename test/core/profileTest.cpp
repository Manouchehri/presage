
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

#define DECLARE_CSTRING(STRING) char* STRING = #STRING
    
    DECLARE_CSTRING(Soothsayer);
    DECLARE_CSTRING(ContextTracker);
    DECLARE_CSTRING(MAX_BUFFER_SIZE);
    DECLARE_CSTRING(Selector);
    DECLARE_CSTRING(SUGGESTIONS);
    DECLARE_CSTRING(REPEAT_SUGGESTIONS);
    DECLARE_CSTRING(GREEDY_SUGGESTION_THRESHOLD);
    DECLARE_CSTRING(Plugins);
    DECLARE_CSTRING(SmoothedNgramPlugin);
    DECLARE_CSTRING(DBFILENAME);
    DECLARE_CSTRING(MAX_PARTIAL_PREDICTION_SIZE);

    Variable* var;
    
    var = new Variable;
    var->push_back(Soothsayer);
    var->push_back(ContextTracker);
    var->push_back(MAX_BUFFER_SIZE);
    CPPUNIT_ASSERT(profile->getConfig(*var) == "1024");
    delete var;

    var = new Variable;
    var->push_back(Soothsayer);
    var->push_back(Selector);
    var->push_back(SUGGESTIONS);
    CPPUNIT_ASSERT(profile->getConfig(*var) == "6");
    var->pop_back();
    var->push_back(REPEAT_SUGGESTIONS);
    CPPUNIT_ASSERT(profile->getConfig(*var) == "no");
    var->pop_back();
    var->push_back(GREEDY_SUGGESTION_THRESHOLD);
    CPPUNIT_ASSERT(profile->getConfig(*var) == "0");
    var->pop_back();
    var->push_back(REPEAT_SUGGESTIONS);
    delete var;
    
    var = new Variable;
    var->push_back(Soothsayer);
    var->push_back(Plugins);
    var->push_back(SmoothedNgramPlugin);
    var->push_back(DBFILENAME);
    CPPUNIT_ASSERT(profile->getConfig(*var) == "database_en.db");
    var->pop_back();
    var->push_back(MAX_PARTIAL_PREDICTION_SIZE);
    CPPUNIT_ASSERT(profile->getConfig(*var) == "40");
    delete var;
}
