
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

    xml_filename = profile_file;

    xml_profile_read_ok = xmlProfileDoc->LoadFile (xml_filename.c_str());
}

Profile::~Profile()
{
    delete xmlProfileDoc;
}

bool Profile::file_read_ok () const
{
    return xml_profile_read_ok;
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
                configuration->insert (Variable::vector_to_string(variable), text);

		//std::cerr << "[Profile] Inserted variable: " << Variable::vector_to_string(variable) << " = " << text << std::endl;
	    }
	}

	// then descend down the tree
	visit_node(configuration, node->FirstChild(), variable);
    }
}

bool Profile::write_to_file () const
{
    //std::cerr << "Saving profile to file: " << xml_filename << std::endl;
    return xmlProfileDoc->SaveFile(xml_filename.c_str());
}

void Profile::read_from_configuration (Configuration* configuration)
{
    TiXmlNode* node = 0;

    // insert initial mandatory declaration if not present
    TiXmlNode* decl = 0;
    for (TiXmlNode* child = xmlProfileDoc->FirstChild();
	 child && !decl; child = child->NextSibling() ) {
	decl = child->ToDeclaration ();
    }
    if (! decl) {
	node = xmlProfileDoc->InsertEndChild( TiXmlDeclaration( "1.0", "UTF-8", "no" ) );
	assert (node);
    }

    // for each configuration variable in configuration
    for (std::map<std::string, Variable*>::const_iterator conf_it = configuration->begin();
	 conf_it != configuration->end();
	 conf_it ++) {

	// start from root of DOM
	node = xmlProfileDoc;

	// get the variable name and break it up in its component vector
	std::string variable_name = conf_it->second->get_name ();
	std::vector<std::string> variable_name_vector = Variable::string_to_vector (variable_name);

	// for each component in variable name vector
	for (size_t i = 0; i < variable_name_vector.size(); i++) {

	    // check if component element exists
	    TiXmlElement* existing = node->FirstChildElement (variable_name_vector[i].c_str());
	    if (existing) {
		// carry on with existing component
		node = existing;

	    } else {
		// create missing component element and carry on with new component
		node = node->InsertEndChild (TiXmlElement (variable_name_vector[i].c_str()));
		assert (node);
	    }
	}

	// check if a text node for element exists
	TiXmlText* text = 0;
	for(TiXmlNode* child = node->FirstChild(); child && !text; child = child->NextSibling() ) {
	    text = child->ToText ();
	}
	if (text) {
	    // text child already exists, so remove it to set new value
	    node->RemoveChild (text);
	}
	node = node->InsertEndChild (TiXmlText (conf_it->second->get_value ().c_str ()));
	assert (node);
    
    }
}
