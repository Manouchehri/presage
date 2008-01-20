
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

#include "core/profile.h"

#include <iostream>

Profile::Profile(TiXmlDocument* profileDoc)
{
    configuration = new Configuration;

    initConfiguration(profileDoc);
}

Profile::~Profile()
{
    delete configuration;
}

void Profile::initConfiguration(TiXmlDocument* root)
{
    Variable variable;

    visitNode(root, variable);
}

void Profile::visitNode(TiXmlNode* node, Variable variable)
{
    if (node) {
	// if we have a node, visit it.
	// else, do nothing.

	// first visit our siblings
	visitNode(node->NextSibling(), variable);

	// then check this element contains a
	// configuration variable
	TiXmlElement* element = node->ToElement();
	if (element) {
	    // append element name to variable to
	    // build fully qualified variable name
	    // before visit children
	    variable.push_back(element->Value());

	    // if element contains text, we have a value for our
	    // config variable, so add it to our configuration
	    const char* text = element->GetText();
	    if (text) {
		(*configuration)[variable] = text;

		printVariable(variable);
		std::cerr << " = " << text << std::endl;
	    }
	}

	visitNode(node->FirstChild(), variable);
    }
}

std::string Profile::stringifyVariable(const Variable& variable) const
{
    std::string result;
    size_t i = 0;
    for ( ; i < variable.size() - 1; i++) {
	result += variable[i] + '.';
    }
    if (i < variable.size()) {
	result += variable[i];
    }
    return result;
}

void Profile::printVariable(const Variable& variable) const
{
    std::cout << stringifyVariable(variable);
}

void Profile::printConfiguration() const
{
    // iterate map
    for (Configuration::const_iterator map_it = configuration->begin();
	 map_it != configuration->end();
	 map_it++) {

	// iterate vector
	for (Variable::const_iterator vec_it = map_it->first.begin();
	     vec_it != map_it->first.end();
	     vec_it++) {
	    std::cout << *vec_it;
	    if (vec_it != map_it->first.end()) {
		std::cout << '.';
	    }
	}

	// value
	std::cout << " = " << map_it->second << std::endl;
    }
}

Value Profile::getConfig(const Variable& variable)
{
    Configuration::const_iterator it = configuration->find(variable);
    if (it != configuration->end()) {
	return it->second;
    } else {
	throw ProfileException("[ProfileException] Cannot find variable "
                               + stringifyVariable(variable));
    }
}
