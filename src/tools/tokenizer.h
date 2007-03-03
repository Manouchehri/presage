
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


#include <iostream>
#include <fstream>
#include <string>

#include "iso8859_1.h"

#ifndef FILETOKENIZER_H
#define FILETOKENIZER_H

/** Tokenizer returns a token each time method tokenize is invoke.
 * 
 * Tokenizer reads a stream of characters from a ifstream input file stream.
 * 
 * Each character is classified in one (or more) of three classes:
 * - valid character: a valid character is part of a token
 * - separator character: a separator character marks the end of a token and the beginning of another
 * - invalid characters: they are characters that can be safely discarded
 * 
 * Tokenizer parses each character from the stream. It builds a token
 * as it reads chars from the stream. When the token is ready, it
 * returns the token in the first parameter. When the stream has
 * reached the end of the input file, it raises the done flag by
 * setting the second parameter to true. In that case, the token
 * returned is not valid.
 * 
 * Bool private data member 'lowercase' determines whether every token
 * is converted to lowercase before being returned.
 * 
 */
class FileTokenizer {
public:
    FileTokenizer( char* );
    ~FileTokenizer();
    void tokenize(std::string*, bool*);
    float getProgress();
    bool getLowercase() const;
    void setLowercase( bool );
    unsigned long int getStreamLength();
    unsigned long int getStreamPosition();

private:
    bool isValid( const int ) const;
    bool isSeparator( const int ) const;

    std::ifstream infile;
    std::string partialToken;

    unsigned long int streamLength;

    bool lowercase;

};



#endif // FILETOKENIZER_H
