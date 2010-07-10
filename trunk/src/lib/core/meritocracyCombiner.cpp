
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


#include "meritocracyCombiner.h"

MeritocracyCombiner::MeritocracyCombiner()
{}

MeritocracyCombiner::~MeritocracyCombiner()
{}

/** Combine predictions based on each suggestion's individual merit.
 *
 * The merit associated to each suggestion is its probability. The
 * higher the probability associated with a suggestion, the higher the
 * merit.
 *
 * Of course, this combination strategy might introduce some imbalance
 * between different predictive methods that calculate probabilities
 * substantially differently.
 *
 */
Prediction MeritocracyCombiner::combine(const std::vector< Prediction >& predictions)
{
    Prediction result;
    for (std::vector< Prediction >::const_iterator it = predictions.begin();
	 it != predictions.end();
	 it++) {
	for (size_t i = 0; i < it->size(); i++) {
	    result.addSuggestion(it->getSuggestion(i));
	}
    }
    return filter(result);
}
