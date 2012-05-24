
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


#ifndef PRESAGE_UTILITY
#define PRESAGE_UTILITY

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif 

#include <string>   // overload strtolower function

class Utility {
    
public:    
    static char*        strtolower( char*        );
    static std::string& strtolower( std::string& );
    static std::string  strtolower( const std::string& );

    static std::string  strtoupper( const std::string& );

    static bool isTrueFalse( const char* );
    static bool isTrueFalse( const std::string& );

    static bool isTrue( const char* );
    static bool isTrue( const std::string& );

    static bool isFalse( const char* );
    static bool isFalse( const std::string& );

    static bool isYesNo( const char* );
    static bool isYesNo( const std::string& );

    static bool isYes( const char* );
    static bool isYes( const std::string& );

    static bool isNo( const char* );
    static bool isNo( const std::string& );

    static double toDouble(const std::string);
    static int    toInt   (const std::string);

    static std::string dirname (const std::string&);
    static std::string filename (const std::string&);

    static bool is_directory_usable (const std::string& dir);
    static void create_directory (const std::string& dir);

};

#endif // PRESAGE_UTILITY
