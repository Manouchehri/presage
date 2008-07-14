
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


#ifndef PRESAGE_REVERSETOKENIZER
#define PRESAGE_REVERSETOKENIZER

#include "tokenizer.h"

/** Tokenizes a stream from the end to the beginning.
 *  _ _ _ _ _ _ _
 * |_|_|_|_|_|_|_|
 *  |   |         |
 *  |   |          offend
 *  |    offset
 *   offbeg
 */
class ReverseTokenizer 
: public Tokenizer {
public:
    ReverseTokenizer(std::istream& stream,
		     const std::string   blanks,
		     const std::string   separs);
    virtual ~ReverseTokenizer();

    /** Returns the number of tokens left.
     */
    virtual int countTokens();

    /** Tests if there are more tokens.
     */
    virtual bool hasMoreTokens() const;
    
    /** Returns the next token.
     */
    virtual std::string nextToken();

    /** Returns progress percentage.
     */
    virtual double progress() const;

protected:

private:

};

#endif // PRESAGE_REVERSETOKENIZER


