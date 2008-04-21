
/*********************************************************
 *  Soothsayer, an extensible predictive text entry system
 *  ------------------------------------------------------
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


RecencyPlugin::RecencyPlugin(Configuration* config, ContextTracker* ct)
    : Plugin(config,
	     ct,
             "RecencyPlugin",
             "RecencyPlugin, a recency statistical plugin",
             "RecencyPlugin, long description." )
{
    lambda = 1;
    n_0 = 1;
    cutoff_threshold = 20;
}

RecencyPlugin::~RecencyPlugin()
{

}

Prediction RecencyPlugin::predict(const size_t max) const
{
    Prediction result;

    std::string prefix = contextTracker->getPrefix();
    if (!prefix.empty()) {
        // Only build recency prediction if prefix is not empty.
        // 
        // When prefix is empty, all previosly seen tokens are
        // candidates for prediction. This means that recency
        // prediction reduces to repetion of max previous tokens,
        // which is not desirable.
        //
        Suggestion  suggestion;
        int         index = 1;
        std::string token = contextTracker->getToken(index);
        while (!token.empty() && index <= max) {
            if (token.find(prefix) == 0) { // if token starts with prefix
                suggestion.setWord(token);
                suggestion.setProbability(exp(-(lambda * (index - 1))));
                result.addSuggestion(suggestion);
            }
            index++;
            token = contextTracker->getToken(index);
        }
    }

    return result;
}

void RecencyPlugin::learn()
{}

void RecencyPlugin::extract()
{}

void RecencyPlugin::train()
{}
