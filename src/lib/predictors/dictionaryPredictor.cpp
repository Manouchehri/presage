
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


#include "dictionaryPredictor.h"

#include <assert.h>


DictionaryPredictor::DictionaryPredictor (Configuration* config, ContextTracker* ht, const char* name)
    : Predictor(config,
	     ht,
	     name,
	     "DictionaryPredictor, dictionary lookup",
	     "DictionaryPredictor, a dictionary based predictor that generates a prediction by extracting tokens that start with the current prefix from a given dictionary"
	     ),
      dispatcher (this)
{
    LOGGER      = PREDICTORS + name + ".LOGGER";
    DICTIONARY  = PREDICTORS + name + ".DICTIONARY";
    PROBABILITY = PREDICTORS + name + ".PROBABILITY";

    // build notification dispatch map
    dispatcher.map (config->find (LOGGER), & DictionaryPredictor::set_logger);
    dispatcher.map (config->find (DICTIONARY), & DictionaryPredictor::set_dictionary);
    dispatcher.map (config->find (PROBABILITY), & DictionaryPredictor::set_probability);
}

DictionaryPredictor::~DictionaryPredictor()
{
    // intentionally empty
}

void DictionaryPredictor::set_dictionary (const std::string& value)
{
    dictionary_path = value;
    logger << INFO << "DICTIONARY: " << value << endl;
}


void DictionaryPredictor::set_probability (const std::string& value)
{
    probability = Utility::toDouble (value);
    logger << INFO << "PROBABILITY: " << value << endl;
}

Prediction DictionaryPredictor::predict(const size_t max_partial_predictions_size, const char** filter) const
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
	if(candidate.find(prefix) == 0) {  // candidate starts with prefix
	    logger << NOTICE << "Found valid token: " << candidate << endl;
	    if (token_satisfies_filter (candidate, prefix, filter)) {
		logger << NOTICE << "Filter check satisfied by token: " << candidate << endl;
		result.addSuggestion(Suggestion(candidate,probability));
		count++;
	    } else {
		logger << NOTICE << "Filter check failed, discarding token: " << candidate << endl;
	    }
	} else {
	    logger << INFO << "Discarding invalid token: " << candidate << endl;
	}
    }

    dictionary_file.close();

    return result;
}

void DictionaryPredictor::learn(const std::vector<std::string>& change)
{
    std::cout << "DictionaryPredictor::learn() method called" << std::endl;
    std::cout << "DictionaryPredictor::learn() method exited" << std::endl;
}

void DictionaryPredictor::update (const Observable* var)
{
    logger << DEBUG << "About to invoke dispatcher: " << var->get_name () << " - " << var->get_value() << endl;
    dispatcher.dispatch (var);
}
