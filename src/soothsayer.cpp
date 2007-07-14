
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>           *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either version 2            *
 * of the License, or (at your option) any later version.                    *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software               *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.*
 *                                                                           *
\*****************************************************************************/


#include "soothsayer.h"

Soothsayer::Soothsayer()
{
    profileManager = new ProfileManager();
    profile = profileManager->getProfile();

    historyTracker = new HistoryTracker(profile);
    predictor = new Predictor(profile, historyTracker);
    selector = new Selector(profile, historyTracker);

    //plump::Logger::getLogger()->setLevel(plump::Logger::DEBUG);
    //plump.appendPath("./plugins");
    //plump.registerCallback(callback_helloworld, 0);
    //plump.discoverPlugins();
}


Soothsayer::~Soothsayer()
{
    delete selector;
    delete predictor;
    delete historyTracker;

    delete profile;
    delete profileManager;
}


std::vector<std::string> Soothsayer::predict( std::string s )
{
    historyTracker->update (s);
    return selector->select (predictor->predict());
}

std::vector<std::string> Soothsayer::predict( char c )
{
    char str[2];
    str[0] = c;
    str[1] = '\0';

    return predict (str);
}


void Soothsayer::update( std::string s )
{
    historyTracker->update (s);
}

void Soothsayer::update( char c )
{
    char str[2];
    str[0] = c;
    str[1] = '\0';
    historyTracker->update (str);
}

void Soothsayer::complete(const std::string completion)
{
    std::string prefix = historyTracker->getPrefix();
 
    // ensure that current prefix is a substring of completion token
    int index = 0;
    while (prefix[index] == completion[index]) {
        index++;
    }
    if (prefix.size() == index) {
        std::string remainder = completion.substr(index);
        update(remainder);
    } else {
        // REVISIT: throw exception
        std::cerr << "Completion does not match prefix!" << std::endl;
    }
}

std::string Soothsayer::history() const
{
    return historyTracker->getPastStream();
}
