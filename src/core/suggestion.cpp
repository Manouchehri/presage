
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>            *
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


#include "suggestion.h"


Suggestion::Suggestion(std::string s, double p)
{
    setWord(s);
    setProbability(p);
}

Suggestion::~Suggestion() {}


bool Suggestion::operator==( const Suggestion& right ) const
{
    if( word == right.word && probability == right.probability )
	return true;
    else
	return false;
}


bool Suggestion::operator<( const Suggestion& right ) const
{
    if( probability < right.probability ) {
	return true;
    } else if( probability == right.probability ) {
	return ( word < right.word );
    } else {
	return false;
    }
}


std::string Suggestion::getWord() const
{
    return word;
}

double Suggestion::getProbability() const
{
    return probability;
}

void Suggestion::setWord(std::string s)
{
    word = s;
}

void Suggestion::setProbability(double p)
{
    // TODO: validate correct probability value
    // correct value depends on whether I decide to implement
    // a probability or a logprobability
    probability = p;
}

std::string Suggestion::toString() const
{
    std::stringstream ss;
    ss << "Word: " << word << " Probability: " << probability << std::endl;
    return ss.str();
}


std::ostream &operator<<( std::ostream &output, const Suggestion &s)
{
    output << s.word << ' ' << s.probability;
    return output;
}
