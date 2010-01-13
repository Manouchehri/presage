
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


#include "plugins/dictionaryPlugin.h"

#include <assert.h>

const Variable DictionaryPlugin::DICTIONARY  = "Presage.Plugins.DictionaryPlugin.DICTIONARY";
const Variable DictionaryPlugin::PROBABILITY = "Presage.Plugins.DictionaryPlugin.PROBABILITY";

DictionaryPlugin::DictionaryPlugin(Configuration* config, ContextTracker* ht)
    : Plugin(config,
	     ht,
	     "DictionaryPlugin",
	     "DictionaryPlugin, dictionary lookup",
	     "DictionaryPlugin, a dictionary based plugin that generates a prediction by extracting tokens that start with the current prefix from a given dictionary")
{
    // might throw ConfigurationException
    dictionary_path = config->get(DICTIONARY);
    probability     = toDouble(config->get(PROBABILITY));
}

DictionaryPlugin::~DictionaryPlugin()
{
    // intentionally empty
}

Prediction DictionaryPlugin::predict(const size_t max_partial_predictions_size, const char** filter) const
{
    Prediction result;

    std::string candidate;
    std::string prefix = contextTracker->getPrefix();

    std::ifstream dictionary_file;
    dictionary_file.open(dictionary_path.c_str());
    if(!dictionary_file)
        logger << ERROR << "Error opening dictionary: " << dictionary_path << endl;
    assert(dictionary_file); // REVISIT: handle with exceptions

    // scan file entries until we get enough suggestions
    unsigned int count = 0;
    while(dictionary_file >> candidate && count < max_partial_predictions_size) {
	if(candidate.find(prefix) == 0) {
	    result.addSuggestion(Suggestion(candidate,probability));
	    count++;
	    logger << NOTICE << "Found valid token: " << candidate << endl;
	} else {
	    logger << INFO << "Discarding invalid token: " << candidate << endl;
	}
    }

    dictionary_file.close();

    return result;
}

void DictionaryPlugin::learn(const std::vector<std::string>& change)
{
    std::cout << "DictionaryPlugin::learn() method called" << std::endl;
    std::cout << "DictionaryPlugin::learn() method exited" << std::endl;
}

void DictionaryPlugin::extract()
{
    std::cout << "DictionaryPlugin::extract() method called" << std::endl;
    std::cout << "DictionaryPlugin::extract() method exited" << std::endl;
}

void DictionaryPlugin::train()
{
    std::cout << "DictionaryPlugin::train() method called" << std::endl;
    std::cout << "DictionaryPlugin::train() method exited" << std::endl;
}
