
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
    configuration = new std::map<Variable, Value>();
}

Configuration::~Configuration()
{
    delete configuration;
}
    
Value Configuration::get(const Variable& variable) const
{
    std::string message;
    if (variable.size() > 0) {
        // non empty variable, search for it in the config
        std::map<Variable, Value>::const_iterator it = configuration->find(variable);
        if (it != configuration->end()) {
            return it->second;
        }

        // variable not found, create exception message
        message = "[Configuration] Cannot find variable "
            + variable.string();
        
    } else {
        message = "[Configuration] Empty variable";
    }
    
    // if we get here, variable was not found in the configuration,
    // hence we have a right to complain
    throw ConfigurationException(message);
}

Value Configuration::operator[](const Variable& variable) const
{
    return get(variable);
}

void Configuration::set(const Variable& variable, const Value& value)
{
    (*configuration)[variable] = value;
}

//void Configuration::operator[](const Variable& variable)
//{
//    //set(variable, value);
//}

void Configuration::print() const
{
    // iterate map
    for (std::map<Variable, Value>::const_iterator map_it = configuration->begin();
	 map_it != configuration->end();
	 map_it++) {

	// variable
	std::cout << map_it->first.string();

	// value
	std::cout << " = " << map_it->second << std::endl;
    }
}
