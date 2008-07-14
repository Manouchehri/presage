
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

    result.addSuggestion (Suggestion("croak", 0.99));

    return result;
}

void DejavuPlugin::learn()
{
    std::cout << "DejavuPlugin::learn() method called" << std::endl;

    if (contextTracker->contextChange()) {
	
    }

    std::cout << "DejavuPlugin::learn() method exited" << std::endl;
}

void DejavuPlugin::extract()
{
    std::cout << "DejavuPlugin::extract() method called" << std::endl;
    std::cout << "DejavuPlugin::extract() method exited" << std::endl;
}

void DejavuPlugin::train()
{
    std::cout << "DejavuPlugin::train() method called" << std::endl;
    std::cout << "DejavuPlugin::train() method exited" << std::endl;
}
