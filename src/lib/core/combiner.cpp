
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


#include "combiner.h"
#include "profile.h"

#include <set>

Combiner::Combiner()
{
    // intentionally empty
}

Combiner::~Combiner()
{
    // intentionally empty
}

Prediction Combiner::filter(const Prediction& prediction) const
{
    Prediction result;

    std::set<std::string> seen_tokens;

    int size = prediction.size();
    Suggestion suggestion;
    std::string token;
    for (int i = 0; i < size; i++) {
        suggestion = prediction.getSuggestion(i);
        token = suggestion.getWord();
        //std::cerr << "[filter] token: " << token << std::endl;
        if (seen_tokens.find(token) == seen_tokens.end()) {
            // if token has not been seen before, then look for
            // potential duplicates and add the interpolated combined
            // probability and remember that this token has now been
            // processed
            //
            //std::cerr << "[filter] searching for possible duplicates" << std::endl;
            for (int j = i + 1; j < size; j++) {
                if (suggestion.getWord() == prediction.getSuggestion(j).getWord()) {
                    double new_prob = suggestion.getProbability()
                        + prediction.getSuggestion(j).getProbability();
                    suggestion.setProbability((new_prob > Suggestion::MAX_PROBABILITY ? 
                                               Suggestion::MAX_PROBABILITY : new_prob));
                    //std::cerr << "[filter] duplicate found, adjusting probability" << std::endl;
                }
            }
            seen_tokens.insert(suggestion.getWord());
            result.addSuggestion(suggestion);
            //std::cerr << "[filter] added token " << token << std::endl;
        }
    }

    return result;
}
