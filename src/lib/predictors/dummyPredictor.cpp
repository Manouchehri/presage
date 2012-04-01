
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


#include "dummyPredictor.h"


DummyPredictor::DummyPredictor(Configuration* config, ContextTracker* ct, const char* name)
    : Predictor(config,
		ct,
		name,
		"DummyPredictor, a fake predictor",
		"DummyPredictor is a fake predictor.\n"
		"It does not perform any actual computation nor implement any prediction mechanism.\n"
		"It always returns the same sample prediction.\n")
{}

DummyPredictor::~DummyPredictor()
{}

Prediction DummyPredictor::predict(const size_t max_partial_predictions_size, const char** filter) const
{
    // A real predictor would query its resources to retrieve the most 
    // probable completion of the prefix based on the current history,
    // but this is just a dummy predictor that returns random suggestions.
    //
    Prediction result;

    result.addSuggestion (Suggestion("foo1", 0.99));
    result.addSuggestion (Suggestion("foo2", 0.98));
    result.addSuggestion (Suggestion("foo3", 0.97));
    result.addSuggestion (Suggestion("foo4", 0.96));
    result.addSuggestion (Suggestion("foo5", 0.95));
    result.addSuggestion (Suggestion("foo6", 0.94));

    result.addSuggestion (Suggestion("bar1", 0.89));
    result.addSuggestion (Suggestion("bar2", 0.88));
    result.addSuggestion (Suggestion("bar3", 0.87));
    result.addSuggestion (Suggestion("bar4", 0.86));
    result.addSuggestion (Suggestion("bar5", 0.85));
    result.addSuggestion (Suggestion("bar6", 0.84));

    result.addSuggestion (Suggestion("foobar1", 0.79));
    result.addSuggestion (Suggestion("foobar2", 0.78));
    result.addSuggestion (Suggestion("foobar3", 0.77));
    result.addSuggestion (Suggestion("foobar4", 0.76));
    result.addSuggestion (Suggestion("foobar5", 0.75));
    result.addSuggestion (Suggestion("foobar6", 0.74));

    return result;
}

void DummyPredictor::learn(const std::vector<std::string>& change)
{
    std::cout << "DummyPredictor::learn() method called" << std::endl;
    std::cout << "DummyPredictor::learn() method exited" << std::endl;
}
