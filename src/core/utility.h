
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>           *
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

#ifndef SOOTH_UTILITY
#define SOOTH_UTILITY

#include <string>   // overload strtolower function

char*        strtolower( char*        );
std::string& strtolower( std::string& );
std::string  strtolower( const std::string& );

bool isTrueFalse( const char* );
bool isTrueFalse( const std::string& );

bool isTrue( const char* );
bool isTrue( const std::string& );

bool isFalse( const char* );
bool isFalse( const std::string& );

bool isYesNo( const char* );
bool isYesNo( const std::string& );

bool isYes( const char* );
bool isYes( const std::string& );

bool isNo( const char* );
bool isNo( const std::string& );

double toDouble(const std::string);
int    toInt   (const std::string);

#endif // SOOTH_UTILITY
