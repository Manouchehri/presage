
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


#include "core/profile.h"

#include <iostream>

Profile::Profile(TiXmlDocument* profileDoc)
{
    profile = profileDoc;
}

Profile::~Profile()
{
}

Configuration* Profile::get_configuration()
{
    Configuration* config = new Configuration();

    init_configuration(config, profile);

    return config;
}

void Profile::init_configuration(Configuration* config, TiXmlDocument* root)
{
    Variable variable;

    visit_node(config, root, variable);
}

void Profile::visit_node(Configuration* configuration,
			 TiXmlNode* node,
			 Variable variable)
{
    if (node) {
	// visit the node only if it is one

	// first visit our siblings
	visit_node(configuration, node->NextSibling(), variable);

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
		//(*configuration)[variable] = text;
		configuration->set(variable, text);

		//std::cout << variable.string() << " = " << text << std::endl;
	    }
	}

	// then descend down the tree
	visit_node(configuration, node->FirstChild(), variable);
    }
}

// Value Profile::getConfig(const Variable& variable)
// {
//     try {
// 	return configuration->get(variable);
//     } catch (Configuration::ConfigurationException& ex) {
// 	std::cerr << "caught ConfigurationException" << std::endl;
// 	
// 	// uncomment next throw statement to fix tests,
// 	// but more needs to be done to get everything ready for 
// 	// committing...
// 	//
// 	throw Profile::ProfileException(ex.what());
//     } catch (...) {
// 	std::cerr << "caught exception" << std::endl;
//     }
// }
