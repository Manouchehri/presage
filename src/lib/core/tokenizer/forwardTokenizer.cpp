
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


#include "forwardTokenizer.h"

ForwardTokenizer::ForwardTokenizer(std::istream& stream,
				   const std::string blankspaces,
				   const std::string separators)
    : Tokenizer(stream, blankspaces, separators)
{
    //std::cerr << "ForwardTokenizer::ForwardTokenizer()" << std::endl;
    offset = offbeg;
}

ForwardTokenizer::~ForwardTokenizer()
{}

int ForwardTokenizer::countTokens()
{
    StreamGuard guard(stream, offset);

    // store current seek pointer position
    std::streamoff curroff = offset;

    // position get pointer at beginning of stream
    offset = offbeg;

    int count = 0;
    while (hasMoreTokens()) {
	count++;
	nextToken();
    }

    // reposition seek get pointer to original position
    offset = curroff;
    
    return count;
}

bool ForwardTokenizer::hasMoreTokens() const
{
    //StreamGuard guard(stream, offset);

    if (offset >= offend) {
	return false;
    } else {
	return true;
    }
}
    
std::string ForwardTokenizer::nextToken()
{
    StreamGuard guard(stream, offset);

    int current;
    std::string str;

    if (stream.good()) { // good() if bad,fail and eof bit are not set
	current = stream.peek();
	if (offset < offend) {
	    
	    while (isBlankspace(current)
		   || isSeparator(current)) {
		offset++;
		stream.seekg(offset);
		current = stream.peek();
	    }

	    while (!isBlankspace(current)
		   && !isSeparator(current)
		   && offset < offend) {

		//std::cerr << "[DEBUG] read: "
		//	  << static_cast<char>(current)
		//	  << std::endl;
		
		if( lowercaseMode() ) {
		    current = tolower( current );
		}

		str.push_back(current);
		
		//std::cerr << "[DEBUG] pushed: "
		//          << static_cast<char>(current)
		//          << std::endl;

		offset++;
		stream.seekg(offset);
		current = stream.peek();
	    }
	}
		
//	do {
//	    do {
//		current = stream.peek();
//		offset++;
//		stream.seekg(offset);
//
//		//std::cerr << "[DEBUG] read: "
//		//	  << static_cast<char>(current)
//		//	  << std::endl;
//		
//		if (   !isBlankspace(current)
//		    && !isSeparator(current)
//		    && offset <= offend) {
//					
//		    if( lowercaseMode() ) {
//		        current = tolower( current );
//		    }
//
//		    str.push_back(current);
//
//		    //std::cerr << "[DEBUG] pushed: "
//		    //          << static_cast<char>(current)
//		    //          << std::endl;
//		}
//	    } while (   !isBlankspace(current)
//			&& !isSeparator(current)
//			&& offset < offend);
//	} while (str.empty() && (offset < offend));
    } else {
	std::cerr << "stream is NOT good!" << std::endl;
    }

    //std::cerr << "[DEBUG] token: " << str << std::endl;
    
    return str;
}

double ForwardTokenizer::progress() const
{
    return static_cast<double>(offset) / offend;
}

