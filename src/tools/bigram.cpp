
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

#include "bigram.h"


Bigram& Bigram::operator=( const Bigram& b )
{
    if( &b != this ) {
	w_1 = b.w_1;
	w = b.w;
    }

    return *this;
}


bool Bigram::operator<( const Bigram& right ) const
{
    if( w_1 < right.w_1 )
	return true;
    else if( w_1 == right.w_1 )
	if( w < right.w )
	    return true;

    return false;
}





std::ostream& operator<<( std::ostream& output, const Bigram& b )
{
    output << b.w_1 << '\t' << b.w;

    return output;
}


