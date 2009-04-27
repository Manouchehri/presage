
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
#include "core/pluginRegistry.h"
#include "core/context_tracker/contextTracker.h"
#include "core/selector.h"
#include "core/predictorActivator.h"

Presage::Presage()
{
    profileManager = new ProfileManager();
    profile = profileManager->getProfile();
    configuration = profile->get_configuration();

    pluginRegistry = new PluginRegistry(configuration);
    contextTracker = new ContextTracker(configuration, pluginRegistry);
    predictorActivator = new PredictorActivator(configuration, pluginRegistry, contextTracker);
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

    pluginRegistry = new PluginRegistry(configuration);
    contextTracker = new ContextTracker(configuration, pluginRegistry);
    predictorActivator = new PredictorActivator(configuration, pluginRegistry, contextTracker);
    selector = new Selector(configuration, contextTracker);
}

Presage::~Presage()
{
    delete selector;
    delete predictorActivator;
    delete contextTracker;
    delete pluginRegistry;

    delete configuration;
    delete profile;
    delete profileManager;
}

std::vector<std::string> Presage::predict(std::string s)
{
    std::vector<std::string> result;

    contextTracker->update (s);

    unsigned int multiplier = 1;
    Prediction prediction = predictorActivator->predict(multiplier++, 0);
    result = selector->select(prediction);

    Prediction previous_prediction = prediction;
    while ((result.size() < (selector->suggestions()))
	   && (prediction = predictorActivator->predict(multiplier++, 0)).size() > previous_prediction.size()) {
	// while the number of predicted tokens is lower than desired,
	// search harder (i.e. higher multiplier) for a prediction of
	// sufficient size (i.e. that satisfies selector), as long as
	// the result of current prediction is greater than the
	// previous prediction (i.e. we are finding new tokens).
	result = selector->select(prediction);
	previous_prediction = prediction;
    }

    return result;
}


std::map<double, std::string> Presage::predict(std::vector<std::string> filter)
{
    std::map<double, std::string> result;

    std::vector<std::string> selection;

    // convert filter to internal representation - currently a null
    // terminated const char**
    const char** internal_filter = new const char*[filter.size() + 1];
    for (std::vector<std::string>::size_type i = 0; i < filter.size(); i++) {
      internal_filter[i] = filter[i].c_str();
    }
    internal_filter[filter.size()] = 0;

    unsigned int multiplier = 1;
    Prediction prediction = predictorActivator->predict(multiplier++, internal_filter);
    selection = selector->select(prediction);

    Prediction previous_prediction = prediction;
    while ((selection.size() < (selector->suggestions()))
	   && (prediction = predictorActivator->predict(multiplier++, internal_filter)).size() > previous_prediction.size()) {
	// while the number of predicted tokens is lower than desired,
	// search harder (i.e. higher multiplier) for a prediction of
	// sufficient size (i.e. that satisfies selector), as long as
	// the selection of current prediction is greater than the
	// previous prediction (i.e. we are finding new tokens).
	selection = selector->select(prediction);
	previous_prediction = prediction;
    }

    delete[] internal_filter;

    for (std::vector<std::string>::const_iterator it = selection.begin();
	 it != selection.end();
	 it++) {
	result[prediction.getSuggestion(*it).getProbability()] = (*it);
    }

    return result;
}

void Presage::update(std::string s)
{
    contextTracker->update (s);
    selector->update ();
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
        if (contextTracker->isCompletionValid(completion)) {
            std::string prefix = contextTracker->getPrefix();
            update(completion.substr(prefix.size()));

        } else {
            std::string message = "[Presage] Error: completion '";
	    message += completion;
	    message += "' does not match prefix: ";
	    message += contextTracker->getPrefix();
	    throw PresageException(message);
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

bool Presage::context_change() const
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
