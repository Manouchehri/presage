
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

#include "soothsayer.h"

#include <string>
#include <vector>

void print( std::vector<std::string> );

int main()
{
    Soothsayer soothsayer;

    const int BUFFER_SIZE = 80;
    char buffer[ BUFFER_SIZE ];

    while(1) {
		
	std::cout << "> ";
	std::cin.getline( buffer, BUFFER_SIZE );

	print( soothsayer.predict(static_cast<std::string>(buffer)) );
		
    }
    return 0;
}


void print( std::vector<std::string> words )
{
    for( std::vector<std::string>::const_iterator i = words.begin();
	 i != words.end();
	 i++ )
	std::cout << *i << std::endl;
}
