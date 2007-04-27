
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


// REVISIT: this file (utility.cpp) and the file that contains the
// declaration of the functions here implemented is a lampant example
// of bad programming practice since it causes CODE DUPLICATION. The
// strtolower functions are defined and implemented in
// src/core/utility.{h,cpp}.

// I replicated the code here to allow the VPATH mechanism to work,
// enable the make distcheck target to compile cleanly and
// successfully, and avoid linking to a library in a parent directory.

// REVISIT: this code duplication and circular library dependency must
// be solved!


#include "utility.h"


/** Convert string to lower case
 *
 */
char* strtolower( char* str )
{

    for( int i = 0; str[i] != '\0'; i++ )
	str[ i ] = tolower( str[ i ] );

    return str;
}


/** Convert string to lower case
 *
 */
std::string& strtolower( std::string& str )
{
	
    for( std::string::iterator i = str.begin();
	 i != str.end();
	 i++ )
	*i = tolower( *i );

    return str;
}


/** Returns a lower case copy of a string
 *
 */
std::string strtolower( const std::string& str )
{

    std::string lowstr = str;
    for( std::string::iterator i = lowstr.begin();
	 i != lowstr.end();
	 i++ )
	*i = tolower( *i );

    return lowstr;
}


