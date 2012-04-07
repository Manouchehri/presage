
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


#include "recencyPredictor.h"

#include <math.h>  // for exp()

RecencyPredictor::RecencyPredictor(Configuration* config, ContextTracker* ct, const char* name)
    : Predictor(config,
		ct,
		name,
		"RecencyPredictor, a statistical recency promotion predictor",
		"RecencyPredictor, based on a recency promotion principle, generates predictions by assigning exponentially decaying probability values to previously encountered tokens. Tokens are assigned a probability value that decays exponentially with their distance from the current token, thereby promoting context recency." ),
      dispatcher (this)
{
    // RecencyPredictor config variables
    LOGGER           = PREDICTORS + name + ".LOGGER";
    LAMBDA           = PREDICTORS + name + ".LAMBDA";
    N_0              = PREDICTORS + name + ".N_0";
    CUTOFF_THRESHOLD = PREDICTORS + name + ".CUTOFF_THRESHOLD";

    // init default values
    lambda = 1;
    n_0 = 1;
    cutoff_threshold = 20;

    dispatcher.map(config->find (LOGGER),            &RecencyPredictor::set_logger);
    dispatcher.map(config->find (LAMBDA),            &RecencyPredictor::set_lambda);
    dispatcher.map(config->find (N_0),               &RecencyPredictor::set_n_0);
    dispatcher.map(config->find (CUTOFF_THRESHOLD),  &RecencyPredictor::set_cutoff_threshold);
}

RecencyPredictor::~RecencyPredictor()
{
    // complete
}

void RecencyPredictor::set_lambda (const std::string& value)
{
    lambda = Utility::toDouble(value);
    logger << INFO << "LAMBDA: " << value << endl;
}

void RecencyPredictor::set_n_0 (const std::string& value)
{
    n_0 = Utility::toDouble (value);
    logger << INFO << "N_0: " << value << endl;
}


void RecencyPredictor::set_cutoff_threshold (const std::string& value)
{
    cutoff_threshold = Utility::toInt (value);
    logger << INFO << "CUTOFF_THRESHOLD: " << value << endl;
}


Prediction RecencyPredictor::predict (const size_t max, const char** filter) const
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

		if (token_satisfies_filter (token, prefix, filter)) {
		    // compute probability according to exponential decay
		    // formula
		    //
		    prob = n_0 * exp(-(lambda * (index - 1)));
		    logger << INFO << "probability: " << prob << endl;
		    suggestion.setWord(token);
		    suggestion.setProbability(prob);
		    result.addSuggestion(suggestion);
		}

            }

            index++;
            token = contextTracker->getToken(index);
        }
    }

    return result;
}

void RecencyPredictor::learn(const std::vector<std::string>& change)
{}

void RecencyPredictor::update (const Observable* var)
{
    logger << DEBUG << "About to invoke dispatcher: " << var->get_name () << " - " << var->get_value() << endl;
    dispatcher.dispatch (var);
}
