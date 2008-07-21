
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

#include "plugins/dejavuPlugin.h"

const Variable DejavuPlugin::LOGGER = "Presage.Plugins.DejavuPlugin.LOGGER";
const Variable DejavuPlugin::MEMORY = "Presage.Plugins.DejavuPlugin.MEMORY";

DejavuPlugin::DejavuPlugin(Configuration* config, ContextTracker* ct)
    : Plugin(config,
	     ct,
	     "DejavuPlugin",
	     "DejavuPlugin, a parrot plugin",
	     "DejavuPlugin is a parrot plugin.\n"
	     "It always returns what it has heard before.\n")
{
    // read values from config
    try {
	Value value = config->get(LOGGER);
	logger << setlevel(value);
	logger << INFO << "LOGGER: " << value << endl;
	
	value = config->get(MEMORY);
	memory = value;
	logger << INFO << "MEMORY: " << value << endl;
	
    } catch (Configuration::ConfigurationException ex) {
	logger << ERROR << "Caught ConfigurationException: " << ex.what() << endl;
    }
}

DejavuPlugin::~DejavuPlugin()
{}

Prediction DejavuPlugin::predict(const size_t max_partial_predictions_size) const
{
    // A real plugin would query its resources to retrieve the most 
    // probable completion of the prefix based on the current history,
    // but this is just a dummy plugin that returns random suggestions.
    //
    Prediction result;

    result.addSuggestion (Suggestion("polly", 0.99));
    result.addSuggestion (Suggestion("wants", 0.98));
    result.addSuggestion (Suggestion("a", 0.97));
    result.addSuggestion (Suggestion("cracker", 0.96));
    result.addSuggestion (Suggestion("croak", 0.95));
    result.addSuggestion (Suggestion("croak", 0.94));

    return result;
}

void DejavuPlugin::learn()
{
    logger << DEBUG << "DejavuPlugin::learn() method called" << endl;

    if (contextTracker->contextChange()) {
	logger << ALL << "You learn something new every day!" << endl;	
    }

    logger << DEBUG << "DejavuPlugin::learn() method exited" << endl;
}

void DejavuPlugin::extract()
{
    logger << DEBUG << "DejavuPlugin::extract() method called" << endl;
    logger << DEBUG << "DejavuPlugin::extract() method exited" << endl;
}

void DejavuPlugin::train()
{
    logger << DEBUG << "DejavuPlugin::train() method called" << endl;
    logger << DEBUG << "DejavuPlugin::train() method exited" << endl;
}
