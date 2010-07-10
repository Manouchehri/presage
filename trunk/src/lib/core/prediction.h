
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


#ifndef PRESAGE_PREDICTION
#define PRESAGE_PREDICTION

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <vector>

#include "suggestion.h"


/** Prediction
 *
 * A prediction contains a set of Suggestion objects.  More precisely,
 * a Prediction is a list of Suggestion object, ordered by decreasing
 * probability.
 *
 * A Prediction object is returned by the predictors and by a combiner
 * object.
 * 
 */
class Prediction {
    friend std::ostream &operator<<( std::ostream &, const Prediction & );

public:
    Prediction();
    ~Prediction();

    const Prediction &operator=( const Prediction & );

    /** Predictions are equal iff suggestions are equal and in same order.
     */
    bool operator== (const Prediction&) const;

    /** Returns number of suggestions in prediction.
     */
    size_t size() const;                      

    /** Returns nth most probable suggestion.
     */
    Suggestion getSuggestion(int=0) const;

    /** Returns suggestion with given token.
     */
    Suggestion getSuggestion(std::string token) const;

    /** Inserts a new suggestion, preserves the ordering.
     * 
     * The suggestion object to be inserted is compared against the
     * suggestions already contained in the prediction and inserted in an
     * ordered fashion.
     *
     * Comparison between suggestion objects uses the overloaded operator<
     *
     */
    void addSuggestion( Suggestion );
    
    /** Returns a string representation of the prediction.
     */
    std::string toString() const;
private:
    std::vector<Suggestion> suggestions;

};


#endif // PRESAGE_PREDICTION
