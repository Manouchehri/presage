
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


#include "plugins/plugin.h"

/** Creates a plugin object.
 *
 * @param ht is a reference to HistoryTracker
 * @param pluginName is the plugin name
 * @param shortDesc is a short description of the plugin's functionality
 * @param longDesc is a long description of the plugin's functionality
 */
Plugin::Plugin( HistoryTracker& ht,
                const char* pluginName,
		const char* shortDesc,
                const char* longDesc )
	: name( pluginName ),
	  shortDescription( shortDesc ),
	  longDescription( longDesc ),
	  historyTracker( ht )
{
	options.push_back( Option( "yes",
				   BOOL,
				   "ACTIVE",
				   "Determines whether the plugin is active." ) );

}

/* Virtual plugin destructor.
 *
 */
Plugin::~Plugin()
{}

/* Get plugin name.
 *
 */
const std::string Plugin::getName() const
{
	return name;
}

/* Get plugin short description.
 *
 */
const std::string Plugin::getShortDescription() const
{
	return shortDescription;
}

/* Get plugin long description.
 *
 */
const std::string Plugin::getLongDescription() const
{
	return longDescription;
}

/*
void Plugin::setLibFilename(const std::string s)
{
	libFilename = s;
}

std::string Plugin::getLibFilename() const
{
	return libFilename;
}

void Plugin::setLibHandle( lt_dlhandle handle)
{
	libHandle = handle;
}

lt_dlhandle Plugin::getLibHandle() const
{
	return libHandle;
}
*/

/** Get option template.
 *
 * An option template is a structure that describes the available
 * option. An option template is a vector of strings. Each string in
 * the vector is an option name.
 *
 */
std::vector<std::string> Plugin::getOptionTemplate() const
{
	// create optionTemplate object (vector of strings)
	std::vector<std::string> optionTemplate;

	std::vector<Option>::const_iterator i;

	// build optionTemplate by walking the options
	for( i=options.begin(); i!=options.end(); i++ ) {
		optionTemplate.push_back( i->getName() );
	}

	return optionTemplate;
}


/** Get option value.
 *
 * Returns the value of the option having name passed as parameter.
 *
 */
std::string Plugin::getOptionValue( const std::string n ) const
{
	std::vector<Option>::const_iterator i;
	i = options.begin();

	while( i!=options.end() && i->getName() != n ) {
//		std::cout << "Option to find: " << n
//			  << " Current option: " << i->getName() << std::endl;
		i++;
	}

	return i->getValue();
}


/** Set option value.
 * 
 * @param n option name
 * @param v value to assign to option
 * @return true if option is set correctly, false otherwise
 *
 */
bool Plugin::setOptionValue( const std::string n, const std::string v )
{
	std::vector<Option>::iterator i;
	i = options.begin();

	while( i!=options.end() && i->getName() != n ) {
		i++;
	}

	if( i != options.end() ) {
		return i->setValue( v );
	} else
		return false;
}


/** Get default option value.
 *
 * @param n option name
 * @return option's default value
 *
 */
std::string Plugin::getOptionDefault( const std::string n ) const
{
	std::vector<Option>::const_iterator i;
	i = options.begin();

	while( i!=options.end() && i->getName() != n ) {
		i++;
	}

	return i->getDefault();
}
	

/** Get option description.
 * 
 * @param n option name
 * @return description of an option
 *
 */
std::string Plugin::getOptionDescription( const std::string n ) const
{
	std::vector<Option>::const_iterator i;
	i = options.begin();

	while( i!=options.end() && i->getName() != n ) {
		i++;
	}

	return i->getDescription();
}
