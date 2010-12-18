
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


#ifndef PRESAGE_SUGGESTION
#define PRESAGE_SUGGESTION

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <sstream>
#include <string>

#include "../presageException.h"

/** Suggestion
 * 
 * A suggestion is a prediction token, probability pair.
 *
 * A prediction token is a string (word or single character). The
 * probability describes how likely the predictor estimated that the
 * associated predicted token is the desired token.
 * 
 * Probability might have to be computed as logarithmic probability to
 * avoid buffer underflows.
 *
 */
class Suggestion {
    friend std::ostream &operator<<( std::ostream &, const Suggestion & );
public:
    Suggestion(std::string="",double=0.0);
    ~Suggestion();

    bool operator== (const Suggestion&) const;
    bool operator!= (const Suggestion&) const;
    bool operator<  (const Suggestion&) const;

    std::string getWord() const;
    double getProbability() const;
    void setWord(std::string);
    void setProbability(double);
    /** Returns a string representation of this suggestion.
     */
    std::string toString() const;

    static const double MIN_PROBABILITY;
    static const double MAX_PROBABILITY;

    class SuggestionException : public PresageException {
    public:
	SuggestionException(presage_error_code_t code, const std::string& msg) throw() : PresageException(code, msg) { }
	virtual ~SuggestionException() throw() { }

    private:
	SuggestionException() throw() : PresageException(PRESAGE_ERROR, "") {};

    };

private:
    std::string word;
    double probability;

};



#endif // PRESAGE_SUGGESTION
