
/*****************************************************************************\
 *                                                                           *
 * Soothsayer, an extensible predictive text entry system                    *
 * ------------------------------------------------------                    *
 *                                                                           *
 * Copyright (C) 2004  Matteo Vescovi <matteo.vescovi@tiscali.it>            *
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

/*
  Test program for TinyXML.
*/


#include <iostream>
#include <sstream>
using namespace std;

#include "tinyxml/tinyxml.h"


int main()
{

    TiXmlDocument doc( "default_config" );
	
    // Insert initial declaration
    doc.InsertEndChild( TiXmlDeclaration( "1.0", "UTF-8", "no" ) );
	
    // Insert root element
    doc.InsertEndChild( TiXmlElement( "Soothsayer" ) );

    TiXmlNode* root;
    TiXmlNode* node;

    root = doc.RootElement();

    // HistoryTracker module
    root->InsertEndChild( TiXmlElement( "HistoryTracker" ) );
    node = root->FirstChild( "HistoryTracker" );
    if( node ) {
	node->InsertEndChild( TiXmlElement( "MAX_BUFFER_SIZE" ) );
	node = node->FirstChild( "MAX_BUFFER_SIZE" );
	if( node ) {
	    node->InsertEndChild( TiXmlText( "1024" ) );
	    node = node->Parent();
	}
	node = node->Parent();
    }
	
    // Selector module
    root->InsertEndChild( TiXmlElement( "Selector" ) );
    node = root->FirstChild( "Selector" );
    if( node ) {
	node->InsertEndChild( TiXmlElement( "SUGGESTIONS" ) );
	node = node->FirstChild( "SUGGESTIONS" );
	if( node ) {
	    node->InsertEndChild( TiXmlText( "6" ) );
	    node = node->Parent();
	}
		
	node->InsertEndChild( TiXmlElement( "REPEAT_SUGGESTIONS" ) );
	node = node->FirstChild( "REPEAT_SUGGESTIONS" );
	if( node ) {
	    node->InsertEndChild( TiXmlText( "no" ) );
	    node = node->Parent();
	}
		
	node->InsertEndChild( TiXmlElement( "GREEDY_SUGGESTIONS_THRESHOLD" ) );
	node = node->FirstChild( "GREEDY_SUGGESTIONS_THRESHOLD" );
	if( node ) {
	    node->InsertEndChild( TiXmlText( "0" ) );
	    node = node->Parent();
	}
	node = node->Parent();
    }

    // Predictor module
    root->InsertEndChild( TiXmlElement( "Predictor" ) );
    node = root->FirstChild( "Predictor" );
    if( node ) {
	node->InsertEndChild( TiXmlElement( "PREDICT_TIME" ) );
	node = node->FirstChild( "PREDICT_TIME" );
	if( node ) {
	    node->InsertEndChild( TiXmlText( "1000" ) );
	    node = node->Parent();
	}
		
	node->InsertEndChild( TiXmlElement( "COMBINATION_METHOD" ) );
	node = node->FirstChild( "COMBINATION_METHOD" );
	if( node ) {
	    node->InsertEndChild( TiXmlText( "linear_interpolation" ) );
	    node = node->Parent();
	}
	node = node->Parent();
    }

    // ProfileManager module
    root->InsertEndChild( TiXmlElement( "ProfileManager" ) );
    node = root->FirstChild( "ProfileManager" );
    if( node ) {
	node->InsertEndChild( TiXmlElement( "DUMMY_OPTION" ) );
	node = node->FirstChild( "DUMMY_OPTION" );
	if( node ) {
	    node->InsertEndChild( TiXmlText( "very dummy" ) );
	    node = node->Parent();
	}
	node = node->Parent();
    }

    // Plugin modules
    root->InsertEndChild( TiXmlElement( "Plugins" ) );
    node = root->FirstChild( "Plugins" );
    if( node ) {
		
	// Dummy plugin
	node->InsertEndChild( TiXmlElement( "DummyPlugin" ) );
	node = node->FirstChild( "DummyPlugin" );
	if( node ) {
	    node->InsertEndChild( TiXmlElement( "ACTIVE" ) );
	    node = node->FirstChild( "ACTIVE" );
	    if( node ) {
		node->InsertEndChild( TiXmlText( "yes" ) );
		node = node->Parent();
	    }
	    node = node->Parent();
	}
		
	// Dictionary plugin
	node->InsertEndChild( TiXmlElement( "DictionaryPlugin" ) );
	node = node->FirstChild( "DictionaryPlugin" );
	if( node ) {
	    node->InsertEndChild( TiXmlElement( "ACTIVE" ) );
	    node = node->FirstChild( "ACTIVE" );
	    if( node ) {
		node->InsertEndChild( TiXmlText( "yes" ) );
		node = node->Parent();
	    }

	    node->InsertEndChild( TiXmlElement( "DICTIONARY_PATH" ) );
	    node = node->FirstChild( "DICTIONARY_PATH" );
	    if( node ) {
		node->InsertEndChild( TiXmlText( "/usr/share/dict/italian" ) );
		node = node->Parent();
	    }
	    node = node->Parent();
			
	}			
	node = node->Parent();
    }

       
    doc.Print();
    doc.SaveFile();


    return 0;
}

