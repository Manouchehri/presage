
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


#include "presage.h"

#include "core/profileManager.h"
#include "core/contextTracker.h"
#include "core/selector.h"
#include "core/predictor.h"

Presage::Presage()
{
    profileManager = new ProfileManager();
    profile = profileManager->getProfile();
    configuration = profile->get_configuration();

    contextTracker = new ContextTracker(configuration);
    predictor = new Predictor(configuration, contextTracker);
    selector = new Selector(configuration, contextTracker);

    //plump::Logger::getLogger()->setLevel(plump::Logger::DEBUG);
    //plump.appendPath("./plugins");
    //plump.registerCallback(callback_helloworld, 0);
    //plump.discoverPlugins();
}

Presage::Presage(const std::string config_filename)
{
    profileManager = new ProfileManager(config_filename);
    profile = profileManager->getProfile();
    configuration = profile->get_configuration();

    contextTracker = new ContextTracker(configuration);
    predictor = new Predictor(configuration, contextTracker);
    selector = new Selector(configuration, contextTracker);
}

Presage::~Presage()
{
    delete selector;
    delete predictor;
    delete contextTracker;

    delete configuration;
    delete profile;
    delete profileManager;
}

std::vector<std::string> Presage::predict(std::string s)
{
    contextTracker->update (s);
    return selector->select (predictor->predict());
}

void Presage::update(std::string s)
{
    contextTracker->update (s);
}

void Presage::complete(const std::string completion)
{
    // There are two types of completions: normal and erasing.
    // normal_completion  = prefix + remainder
    // erasing_completion = eraser + prefix + remainder
    //
    // or, given that token = prefix + remainder
    // normal_completion  = token
    // erasing_completion = eraser + token
    // 
    // where eraser = ^H+ (one or more backspace characters)
    // 
    // offset to first non ^H character in completion (^H are inserted
    // by abbreviation expansion predictor to erase abbreviation from
    // stream)
    // 
    std::string::size_type offset = completion.find_first_not_of('\b');
    if (offset == 0) {
        // normal completion, 
        // ensure that current prefix is a substring of completion
        // token and update with remainder
        //
        std::string prefix = contextTracker->getPrefix();
        if (completion.find(prefix) == 0) {
            update(completion.substr(prefix.size()));

        } else {
            // REVISIT: throw exception
            std::cerr << "[Presage] Error: completion '" << completion 
                      << "' does not match prefix '" << prefix << "'" << std::endl;
            abort();
        }
    } else {
        // erasing completion,
        // pass it to tracker in its entirety
        //
        update(completion);
    }
}

std::string Presage::context() const
{
    return contextTracker->getPastStream();
}

bool Presage::contextChange() const
{
    return contextTracker->contextChange();
}

std::string Presage::prefix() const
{
    return contextTracker->getPrefix();
}

std::string Presage::config(const std::string variable) const
{
    return configuration->get(variable);
}

void Presage::config(const std::string variable, const std::string value)
{
    configuration->set(variable, value);
}
