
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>            *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either version 2            *
 * of the License, or (at your option) any later version.                    *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software               *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.*
 *                                                                           *
\*****************************************************************************/

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

int Prediction::getSize() const
{
    return suggestions.size();
}

Suggestion Prediction::getSuggestion(int i) const
{
    assert( i >= 0 && static_cast<unsigned int>(i) < suggestions.size() );

    return suggestions[i];
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
	    i++;
	}
	suggestions.insert( i, s );
    }
}

std::string Prediction::toString() const
{
    std::string str;
    std::vector<Suggestion>::const_iterator i;
    for( i=suggestions.begin(); i!=suggestions.end(); i++ ) {
	str += i->toString();
    }
    return str;
}

std::ostream &operator<<( std::ostream &output, const Prediction &p )
{
    std::vector<Suggestion>::const_iterator i;
    for( i=p.suggestions.begin(); i!=p.suggestions.end(); i++ ) {
	output << *i << std::endl;
    }

    return output;
}
