
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



#include "option.h"

#include <iostream>
#include <ctype.h>


/** Option
 *
 * \param t option type
 * \param n option name
 * \param d option description
 * \param def default option value
 * 
 */
Option::Option( const std::string dv,   // default
		const OptionType t,          // tipo
		const std::string n,         // nome
		const std::string d )        // descrizione		
    : name( n ), description( d ), type( t )
{
    setDefault( dv );
    setValue( dv );
}


/** Option
 *
 * \param t option type
 * \param n option name
 * \param d option description
 * \param def default option value
 * \param v option value
 * 
 */
Option::Option( const std::string v,       // valore
		const std::string dv, // default
		const OptionType t,        // tipo
		const std::string n,       // nome
		const std::string d )      // descrizione
    : name( n ), description( d ), type( t )
{
    setDefault( dv );
    setValue( v );
}


/** Option destructor
 *
 */
Option::~Option()
{}


/** Get option type
 *
 * Returns option type.
 */
OptionType Option::getType() const
{
    return type;
}


/** Get option name
 *
 * The option name corresponds to the XML container name.
 * 
 * Returns the option name.
 *
 */
std::string Option::getName() const
{
    return name;
}


/** Get option description
 * 
 * Returns the option description
 *
 */
std::string Option::getDescription() const
{
    return description;
}


/** Get option default value
 *
 * Returns option's default value.
 *
 */
std::string Option::getDefault() const
{
    return defaultValue;
}


/** Get option value
 *
 * Returns option value.
 *
 */
std::string Option::getValue() const
{
    return value;
}


/** Set option value
 *
 * \param s value to set (string)
 *
 * Returns true if option is successfully set, false otherwise.
 *
 */
bool Option::setValue(std::string s)
{
    if( !s.empty() && validate( s ) ) {
	value = s;
	return true;
    } else {
	return false;
    }
}


/** Set option default value
 *
 * \param s value to set (string)
 * 
 * Returns true if option is successfully set, false otherwise.
 *
 */
void Option::setDefault(const std::string s)
{
    if( !s.empty() && validate( s ) ) {
	defaultValue = s;
    } else {
	std::cerr << "Attempted assignment of illegal default value\a"
		  << std::endl << "Bailing out..." << std::endl;
	abort();
    }
}


/** Validate value string based on option type
 *
 */
bool Option::validate(std::string s)
{
    // validation that string is not empty should be done here...
    // but it's done in setValue...

    switch( type ) {
    case INT:
	if( iterate( s, isdigit ) ) {
	    value = s;
	    return true;
	} else {
	    std::cerr << "[Option] Value cannot be converted to integer. Fatal error"
		      << std::endl;
	    return false;
	}
	break;
    case BOOL:
	if( s == "yes" || s == "no" ) {
	    value = s;
	    return true;
	} else {
	    std::cerr << "[Option] Value cannot be converted to boolean. Fatal error"
		      << std::endl;
	    return false;
	}			
	break;
    case DOUBLE:
	// FIXME

	// TODO <- validate here that it can be translated to float
	// ********************************************************
	return true;
	break;
    case STRING:
	// FIXME
//		if( iterate( s, isalpha ) ) {
	if( 1 ) {            // ATTENTION <<<=========== !!!!!!!!!!!
	    value = s;
	    return true;
	} else {
	    std::cerr << "[Option] Value cannot be converted to string. Fatal error"
		      << std::endl;
	    return false;
	}			
	break;
    case INT_RANGE:

	// TODO <==
		
	return false;
	break;
    case INT_LIST:

	// TODO <==

	return false;
	break;
    default:
	std::cerr << "[Option] Validation attempted on unknown option type.\aBailing out..." << std::endl;
	abort();
	break;
    }

}


/** Helper function for validate
 *
 * \param s string to iterate through
 * \param f pointer to function to execute for each character
 *
 * For each character in string s, invokes validating function f passing the current character as argument.
 *
 */
bool Option::iterate( std::string s, int (*f)(int) throw () )
{
    std::string::const_iterator i;
    for( i=s.begin(); i!=s.end(); i++ ) {
	if( !f( *i ) ) {
	    return false;
	}
    }

    return true;
}


/** Draw option interface
 *
 */
void Option::draw() const
{
#ifdef NCURSES

#elif WXWIDGETS

#else
    std::cout << "Name       : " << name << std::endl
	      << "Type       : " << type << std::endl
	      << "Description: " << description << std::endl
	      << "Default    : " << defaultValue << std::endl
	      << "Value      : " << value << std::endl;
#endif
}
