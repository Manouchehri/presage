
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

Profile::Profile(TiXmlDocument* profileDoc)
    : profile(profileDoc)
{}

Profile::~Profile()
{}

Value Profile::getConfig(Variable variable)
{
    const char SEPARATOR = '.';
    std::string stringifiedVariable;
    TiXmlNode* node = profile;
    for (int i = 0; (i < variable.size() && node); i++) {
	node = node->FirstChild(variable[i].c_str());
	stringifiedVariable += variable[i] + SEPARATOR;
	if (!node) {
	    throw ProfileException("[ProfileException] Cannot find variable "
				   + stringifiedVariable);
	}
    }
    TiXmlElement* element = node->ToElement();
    if (!element) {
	throw ProfileException("[ProfileException] Cannot convert node to element");
    }
    TiXmlText* text = element->FirstChild()->ToText();
    if (!text) {
	throw ProfileException("[ProfileException] Cannot convert element to text");
    }
    
    // found desired configuration variable, return value
    return text->Value();
}
