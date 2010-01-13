
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


#include "recencyPlugin.h"

#include <math.h>  // for exp()

// RecencyPlugin config variables
const Variable RecencyPlugin::LOGGER           = "Presage.Plugins.RecencyPlugin.LOGGER";
const Variable RecencyPlugin::LAMBDA           = "Presage.Plugins.RecencyPlugin.LAMBDA";
const Variable RecencyPlugin::N_0              = "Presage.Plugins.RecencyPlugin.N_0";
const Variable RecencyPlugin::CUTOFF_THRESHOLD = "Presage.Plugins.RecencyPlugin.CUTOFF_THRESHOLD";

RecencyPlugin::RecencyPlugin(Configuration* config, ContextTracker* ct)
    : Plugin(config,
	     ct,
             "RecencyPlugin",
             "RecencyPlugin, a statistical recency promotion plugin",
             "RecencyPlugin, based on a recency promotion principle, generates predictions by assigning exponentially decaying probability values to previously encountered tokens. Tokens are assigned a probability value that decays exponentially with their distance from the current token, thereby promoting context recency." )
{
    // init default values
    lambda = 1;
    n_0 = 1;
    cutoff_threshold = 20;

    // read values from config
    try {
	Value value = config->get(LOGGER);
	logger << setlevel(value);
	logger << INFO << "LOGGER: " << value << endl;
    } catch (Configuration::ConfigurationException ex) {
	logger << WARN << "Caught ConfigurationException: " << ex.what() << endl;
    }

    try {
	Value value = config->get(LAMBDA);
	lambda = toDouble(value);
	logger << INFO << "LAMBDA: " << value << endl;
	
	value = config->get(N_0);
	n_0 = toDouble(value);
	logger << INFO << "N_0: " << value << endl;
	
	value = config->get(CUTOFF_THRESHOLD);
	cutoff_threshold = toInt(value);
	logger << INFO << "CUTOFF_THRESHOLD: " << value << endl;

    } catch (Configuration::ConfigurationException ex) {
	logger << ERROR << "Caught fatal ConfigurationException: " << ex.what() << endl;
	throw PresageException("Unable to init " + name + " predictive plugin.");
    }
    

}

RecencyPlugin::~RecencyPlugin()
{

}

Prediction RecencyPlugin::predict(const size_t max, const char** filter) const
{
    Prediction result;

    std::string prefix = contextTracker->getPrefix();
    logger << INFO << "prefix: " << prefix << endl;
    if (!prefix.empty()) {
        // Only build recency prediction if prefix is not empty: when
        // prefix is empty, all previosly seen tokens are candidates
        // for prediction. This is not desirable, because it means
        // that recency prediction reduces to repetion of max previous
        // tokens (i.e. the prediction would contain the most recent
        // tokens in reverse order).
        //
        Suggestion  suggestion;
        size_t      index = 1;
        std::string token = contextTracker->getToken(index);
	double      prob = 0;
        while (!token.empty()                // context history exhausted
	       && result.size() < max        // need only max suggestions
	       && index <= cutoff_threshold  // look back only as far as cutoff
	    ) {
	    logger << INFO << "token: " << token << endl;

            if (token.find(prefix) == 0) { // if token starts with prefix
		// compute probability according to exponential decay
		// formula
		//
		prob = n_0 * exp(-(lambda * (index - 1)));
		logger << INFO << "probability: " << prob << endl;
                suggestion.setWord(token);
                suggestion.setProbability(prob);
                result.addSuggestion(suggestion);
            }

            index++;
            token = contextTracker->getToken(index);
        }
    }

    return result;
}

void RecencyPlugin::learn(const std::vector<std::string>& change)
{}

void RecencyPlugin::extract()
{}

void RecencyPlugin::train()
{}
