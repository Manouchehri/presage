
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
#include <sstream>

Profile::Profile(const std::string& profile_file)
{
    xmlProfileDoc = new TiXmlDocument();
    assert( xmlProfileDoc );

    bool readOk = xmlProfileDoc->LoadFile (profile_file.c_str());
    
    std::stringstream message;
    if (readOk) {
	// logger has not been init'd with configuration, because no
	// profile is known yet, hence caching this logging item,
	// which will be flushed when configuration is finally read in
	//
	message << "Loaded profile: " << profile_file;

	//cache_log_message(logger.NOTICE, message.str());
	std::cerr << message << std::endl;

    } else {
	// logger has not been init'd with configuration, because no
	// profile is known yet, hence caching this logging item,
	// which will be flushed when configuration is finally read in
	//
	message << "Failed to load profile: " << profile_file;

	//cache_log_message(logger.NOTICE, message.str());
	std::cerr << message << std::endl;
    }
}

Profile::~Profile()
{
    delete xmlProfileDoc;
}

void Profile::read_into_configuration(Configuration* config)
{
    init_configuration(config, xmlProfileDoc);
}

void Profile::init_configuration(Configuration* config, TiXmlDocument* root)
{
    std::vector<std::string> variable;

    visit_node(config, root, variable);
}

void Profile::visit_node(Configuration* configuration,
			 TiXmlNode* node,
			 std::vector<std::string> variable)
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
	      configuration->insert (Variable::vector_to_string(variable), text);

		//std::cout << variable.string() << " = " << text << std::endl;
	    }
	}

	// then descend down the tree
	visit_node(configuration, node->FirstChild(), variable);
    }
}
