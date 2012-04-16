
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

/** Uniquify duplicate tokens and accumulate their probability
 *
 *  input prediction  ->  output combined prediction
 *  -                     -
 *  foo, 0.2              foo, MAX_PREDICTION
 *  bar, 0.1              bar, 0.3
 *  foobar, 0.1           foobar, 0.1
 *  foo, 0.8              foz, 0.1
 *  bar, 0.2
 *  foo, 0.3
 *  foz, 0.1
 *
 * FIXME : current implementation is O(n^2) ... it doesn't have to be.
 *
 */
Prediction Combiner::filter(const Prediction& prediction) const
{
    Prediction result;

    std::set<std::string> seen_tokens;

    size_t size = prediction.size();
    Suggestion suggestion;
    std::string token;
    for (size_t i = 0; i < size; i++)
    {
        suggestion = prediction.getSuggestion(i);
        token = suggestion.getWord();
        //std::cerr << "[filter] token: " << token << std::endl;
        if (seen_tokens.find(token) == seen_tokens.end())
	{
            // if token has not been seen before, then look for
            // potential duplicates and incrementally add the
            // interpolated combined probability and remember that
            // this token has now been processed
            //

            //std::cerr << "[filter] searching for possible duplicates" << std::endl;
            for (int j = i + 1; j < size; j++)
	    {
                if (token == prediction.getSuggestion(j).getWord())
		{
		    // duplicate of token found, increment probability, up to MAX_PROBABILITY
		    suggestion.setProbability(
			(  (suggestion.getProbability() + prediction.getSuggestion(j).getProbability()) > Suggestion::MAX_PROBABILITY
			 ? Suggestion::MAX_PROBABILITY 
			 : (suggestion.getProbability() + prediction.getSuggestion(j).getProbability()))
			);
                    //std::cerr << "[filter] duplicate found, adjusting probability" << std::endl;
                }
            }
            seen_tokens.insert(token);
            result.addSuggestion(suggestion);
            //std::cerr << "[filter] added token " << token << std::endl;
        }
    }

    return result;
}
