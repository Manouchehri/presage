
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


#include "configuration.h"

#include <iostream>

Configuration::Configuration()
{
    configuration = new std::map<std::string, Variable*>();
}

Configuration::~Configuration()
{
    for (std::map<std::string, Variable*>::iterator it = configuration->begin();
	 it != configuration->end();
	 it++) {
        //std::cerr << "[Configuration] Deleting variable: " << it->first << '\t' << it->second << std::endl;
        delete it->second;
        //std::cerr << "[Configuration] Deleted  variable: " << it->first << std::endl;
    }
    delete configuration;
}
    
Variable* Configuration::find(const std::string& variable) const
{
    std::map<std::string, Variable*>::const_iterator it = configuration->find (variable);
    if (it == configuration->end()) {
        // variable not found, create exception message
        std::string message = "[Configuration] Cannot find variable " + variable;
        
	// if we get here, variable was not found in the configuration,
	// hence we have a right to complain
	throw ConfigurationException(PRESAGE_CONFIG_VARIABLE_ERROR, message);
    }

    return it->second;
}

Variable* Configuration::operator[](const std::string& variable) const
{
    return find(variable);
}

void Configuration::insert(const std::string& variable, 
			   const std::string& value)
{
    std::map<std::string, Variable*>::const_iterator it = configuration->find (variable);
    if (it != configuration->end ()) {
	it->second->set_value (value);
	//std::cerr << "[Configuration] Modifying existing variable: " << variable << std::endl;

    } else {
	Variable* var = new Variable (variable);
	var->set_value (value);
	configuration->insert (std::pair<std::string, Variable*> (variable, var));
      
	//std::cerr << "[Configuration] Adding new variable: " << variable << '\t' << var << std::endl;
    }

    //std::cerr << "[Configuration] Inserted variable: " << variable << std::endl;
}

void Configuration::remove(const std::string& variable)
{
    std::map<std::string, Variable*>::iterator it = configuration->find (variable);
    if (it != configuration->end()) {
        delete it->second;
	configuration->erase (it);
    }
}

void Configuration::print() const
{
    // iterate map
    for (std::map<std::string, Variable*>::const_iterator map_it = configuration->begin ();
	 map_it != configuration->end ();
	 map_it++) {

	// variable
	std::cout << map_it->first;

	// value
	std::cout << " = " << map_it->second->get_value () << std::endl;
    }
}

std::map<std::string, Variable*>::const_iterator Configuration::begin () const
{
    return configuration->begin();
}

std::map<std::string, Variable*>::const_iterator Configuration::end () const
{
    return configuration->end();
}
