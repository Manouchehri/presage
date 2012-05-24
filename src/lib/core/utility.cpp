
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


#include "utility.h"
#ifdef HAVE_STRING_H
# include <string.h>
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_DIRENT_H
# include <dirent.h>
#endif
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

/** Convert string to lower case
 *
 */
char* Utility::strtolower(char* str)
{
    for(int i = 0; str[i] != '\0'; i++)
	str[i] = tolower (str[i]);
    
    return str;
}


/** Convert string to lower case
 *
 */
std::string& Utility::strtolower(std::string& str)
{
    for(std::string::iterator i = str.begin();
	i != str.end();
	i++ )
	*i = tolower( *i );
    
    return str;
}

/** Returns a lower case copy of a string
 *
 */
std::string Utility::strtolower(const std::string& str)
{
    std::string lowstr = str;
    for(std::string::iterator i = lowstr.begin();
	i != lowstr.end();
	i++ )
	*i = tolower( *i );
    
    return lowstr;
}


/** Convert string to upper case
 *
 */
std::string Utility::strtoupper(const std::string& str)
{
    std::string upstr = str;
    for(std::string::iterator i = upstr.begin();
	i != upstr.end();
	i++ )
	*i = toupper( *i );
    
    return upstr;
}


/** Is string either true or false
 *
 */
bool Utility::isTrueFalse(const char* str)
{
    // TODO: manage yes/no with gettext?

    return (isTrue (str) || isFalse (str));
}

/** Is string True
 *
 */
bool Utility::isTrue(const char* str)
{
    bool result = false;
    
    char* workingStr = new char[strlen (str) + 1];
    strcpy (workingStr, str);
    
    std::string lowstr = strtolower (workingStr);
    if( lowstr == "true" ||
	lowstr == "1" )
	result = true;

    delete[] workingStr;

    return result;
}


/** Is string False
 *
 */
bool Utility::isFalse(const char* str)
{
    bool result = false;

    char* workingStr = new char[strlen(str) + 1];
    strcpy (workingStr, str);

    std::string lowstr = strtolower (workingStr);
    if( lowstr == "false" ||
	lowstr == "0" )
	result = true;
    
    delete[] workingStr;

    return result;
}


/** Is string True
 *
 */
bool Utility::isTrue(const std::string& str)
{
    return isTrue( str.c_str() );
}


/** Is string False
 *
 */
bool Utility::isFalse(const std::string& str)
{
    return isFalse( str.c_str() );
}


/** Is string either true or false
 *
 */
bool Utility::isTrueFalse( const std::string& str )
{
    return isTrueFalse( str.c_str() );
}


/** Is string either yes or no
 *
 */
bool Utility::isYesNo(const char* str)
{
    return (isYes (str) || isNo (str));
}


/** Is string Yes
 *
 */
bool Utility::isYes( const char* str )
{
    bool result = false;

    char* workingStr = new char[strlen(str) + 1];
    strcpy (workingStr, str);
    
    std::string lowstr = strtolower (workingStr);
    if( lowstr == "yes" ||
	lowstr == "yeah" ||
	lowstr == "ye" ||
	lowstr == "true")
	result = true;
    
    delete[] workingStr;
    
    return result;
}


/** Is string No
 *
 */
bool Utility::isNo( const char* str )
{
    bool result = false;

    char* workingStr = new char[strlen(str) + 1];
    strcpy (workingStr, str);

    std::string lowstr = strtolower (workingStr);
    if( lowstr == "no" ||
	lowstr == "nope" ||
	lowstr == "nah" ||
	lowstr == "nay" ||
	lowstr == "false")
	result = true;

    delete[] workingStr;

    return result;
}


/** Is string either yes or no
 *
 */
bool Utility::isYesNo(const std::string& str)
{
    return isYesNo (str.c_str());
}


/** Is string Yes
 * 
 */
bool Utility::isYes( const std::string& str )
{
    return isYes (str.c_str());
}


/** Is string No
 *
 */
bool Utility::isNo(const std::string& str)
{
    return isNo (str.c_str());
}

/** Convert string to double.
 *
 */
double Utility::toDouble(const std::string str)
{
    return atof(str.c_str());
}

/** Convert string to int.
 *
 */
int Utility::toInt(const std::string str)
{
    return atoi(str.c_str());
}


/** Return directory name from filepath
 *
 */
std::string Utility::dirname (const std::string& filepath)
{
    std::string result;

    std::string::size_type pos = filepath.find_last_of ("\\/");
    if (std::string::npos != pos) {
	result = filepath.substr (0, pos);
    }

    return result;
}


/** Return file name from filepath
 *
 */
std::string Utility::filename (const std::string& filepath)
{
    std::string result;

    std::string::size_type pos = filepath.find_last_of ("\\/");
    if (std::string::npos != pos) {
	result = filepath.substr (pos + 1);
    }

    return result;
}


/** Attempt to open directory and return true if successful
 *
 */
bool Utility::is_directory_usable (const std::string& dir)
{
    DIR *dp;
#ifdef HAVE_DIRENT_H
    dp = opendir (dir.c_str());
    if (dp != NULL)
    {
	closedir (dp);
	return true;
    }
    else
    {
	return false;
    }
#else
    return true;
#endif
}


/** Create directory
 *
 */
void Utility::create_directory (const std::string& dir)
{
#ifdef HAVE_SYS_STAT_H
    mkdir (dir.c_str()
#ifndef _WIN32
	   , S_IRWXU
#endif
	   );
#endif
}
