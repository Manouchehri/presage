
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


#include "prediction.h"
#include <assert.h>

Prediction::Prediction()
{}

Prediction::~Prediction()
{}

const Prediction &Prediction::operator=( const Prediction &right )
{
    if( &right != this ) {
	suggestions = right.suggestions;

	//assert( ( suggestions == right.suggestions ) );
    }

    return *this;
}

bool Prediction::operator== (const Prediction& right) const
{
    // same instance is obviously equal to itself
    if (&right == this) {
	return true;
    } else {
	if (size() != right.size()) {
	    return false;
	} else {
	    // need to compare each suggestion
	    bool result = true;
	    size_t i = 0;
	    while (i < size() && result) {
		if (getSuggestion(i) != right.getSuggestion(i)) {
		    result = false;
		}
		i++;
	    }
	    return result;
	}
    }
}

size_t Prediction::size() const
{
    return suggestions.size();
}

Suggestion Prediction::getSuggestion(int i) const
{
    assert( i >= 0 && static_cast<unsigned int>(i) < suggestions.size() );

    return suggestions[i];
}

Suggestion Prediction::getSuggestion(std::string token) const
{
    for (size_t i = 0; i < suggestions.size(); i++) {
	if (suggestions[i].getWord() == token) {
	    return suggestions[i];
	}
    }
    return Suggestion();
}

void Prediction::addSuggestion(Suggestion s)
{
    // insert s so that suggestions vector is sorted

    // handle empty vector first
    if( suggestions.empty() ) {
	suggestions.push_back( s );
    } else {
	std::vector< Suggestion >::iterator i = suggestions.begin();
	while( i != suggestions.end() && s < *i ) {
	    ++i;
	}
	suggestions.insert( i, s );
    }
}

std::string Prediction::toString() const
{
    std::string str;
    std::vector<Suggestion>::const_iterator i;
    for( i=suggestions.begin(); i!=suggestions.end(); ++i ) {
	str += i->toString();
    }
    return str;
}

std::ostream &operator<<( std::ostream &output, const Prediction &p )
{
    std::vector<Suggestion>::const_iterator i;
    for( i=p.suggestions.begin(); i!=p.suggestions.end(); ++i ) {
	output << *i << std::endl;
    }

    return output;
}
