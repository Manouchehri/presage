
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


#include "reverseTokenizer.h"

ReverseTokenizer::ReverseTokenizer(std::istream& stream,
				   const std::string   blanks,
				   const std::string   separs)
    : Tokenizer(stream, blanks, separs)
{
    offset = offend;
    //assert( stream.good());
    //assert(!stream.fail());
    //assert(!stream.bad() );
    //assert(!stream.eof() );
    // stream clearing needed because offset is positioned at end
    stream.clear();

    //std::cerr << "ReverseTokenizer::ReverseTokenizer() offbeg: " << offbeg 
    //          << " offset: " << offset << " offend: " << offend << std::endl;
}

ReverseTokenizer::~ReverseTokenizer()
{}

int ReverseTokenizer::countTokens()
{
    StreamGuard guard(stream, offset);

    // store current seek pointer position
    std::streamoff curroff = offset;

    // position get pointer at end of stream
    offset = offend;

    int count = 0;
    while (hasMoreTokens()) {
	nextToken();
	count++;
    }

    // reposition seek get pointer to original position
    offset = curroff;

    return count;
}

bool ReverseTokenizer::hasMoreTokens() const
{
    //std::cerr << "ReverseTokenizer::hasMoreTokens() offbeg: " << offbeg 
    //          << " offset: " << offset << " offend: " << offend << std::endl;
    if (offbeg < offset) {
	return true;
    } else {
	return false;
    }
}
    
std::string ReverseTokenizer::nextToken()
{
    StreamGuard guard(stream, offset);

    int current;
    std::string str;

    if (stream.good()) {
	while (offbeg < offset
	       && str.empty()) {
	    stream.seekg(offset - 1);
	    current = stream.peek();

	    if (offset == offend &&
		(isSeparator(current) || isBlankspace(current))) {
		offset--;
		return str;
	    }

	    while ((isBlankspace(current) || isSeparator(current))
		   && offbeg < offset ) {
		offset--;
		stream.seekg(offset - 1);
		current = stream.peek();
	    }

	    while (!isBlankspace(current)
		   && !isSeparator(current)
		   && offbeg < offset) {
		
		if( lowercaseMode() ) {
		    current = tolower( current );
		}

		// since the token is read backwards, the string
		// needs to be reversed by inserting the char at
		// the front
		str.insert(str.begin(), current);

		offset--;
		stream.seekg(offset - 1);
		current = stream.peek();
	    }
	}
    }

//    if (stream.good()) {
//	do {
//	    do {
//		current = stream.peek();
//		offset--;
//		stream.seekg(offset);
//
//		// handle case where last character is a separator by
//		// returning an empty token
//                if (offset == offend - 2
//                    && isSeparator(current)) {
//                    return "";
//                }
//                
//		//std::cerr << "[DEBUG] read: "
//                //  << static_cast<char>(current)
//                //  << std::endl;
//
//		if (!isBlankspace(current)
//                    && !isSeparator(current)
//                    && offset >= offbeg - 1) {
//                    
//		    if( lowercaseMode() ) {
//		        current = tolower( current );
//		    }
//
//                    // since the token is read backwards, the string
//                    // needs to be reversed by inserting the char at
//                    // the front
//		    str.insert(str.begin(), current);
//
//		    //std::cerr << "[DEBUG] pushed: "
//                    //  << static_cast<char>(current)
//                    //  << std::endl;
//                    //std::cerr << "[DEBUG] partial string: "
//                    //          << str << std::endl;
//		}
//	    } while (!isBlankspace(current)
//                     && !isSeparator(current)
//                     && (offset >= offbeg));
//	} while (str.empty() && (offset >= offbeg));
//    }

    //std::cerr << "[DEBUG] token: " << str << std::endl;

    return str;
}

double ReverseTokenizer::progress() const
{
    return static_cast<double>(offend - offset) / (offend - offbeg);
}
