
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


#include "defaultProfile.h"

#include <sstream>

const char*        DefaultProfile::DEFAULT_PROFILE_FILENAME            = "presage.xml";
const std::string  DefaultProfile::DEFAULT_LOGGER_LEVEL                = "DEBUG";
const int          DefaultProfile::DEFAULT_PREDICT_TIME                = 1000;
const int          DefaultProfile::DEFAULT_MAX_PARTIAL_PREDICTION_SIZE = 100;
const std::string  DefaultProfile::DEFAULT_COMBINATION_POLICY          = "Meritocracy";
const int          DefaultProfile::DEFAULT_SLIDING_WINDOW_SIZE         = 80;
const size_t       DefaultProfile::DEFAULT_SUGGESTIONS                 = 6;
const bool         DefaultProfile::DEFAULT_REPEAT_SUGGESTION           = false;
const size_t       DefaultProfile::DEFAULT_GREEDY_SUGGESTION_THRESHOLD = 0;
const std::string  DefaultProfile::DEFAULT_PLUGINS                     = "RecencyPlugin";

DefaultProfile::DefaultProfile (const std::string& filename)
  : Profile (filename)
{
    build_xml_document (filename);
}

DefaultProfile::~DefaultProfile()
{
    // complete
}

void DefaultProfile::build_xml_document (const std::string& filename)
{
    TiXmlNode* root;
    TiXmlNode* node;
    TiXmlNode* module;
    TiXmlNode* element;

    // Create document
    xmlProfileDoc = new TiXmlDocument(filename.c_str());

    // Insert initial mandatory declaration
    node = xmlProfileDoc->InsertEndChild( TiXmlDeclaration( "1.0", "UTF-8", "no" ) );
    assert( node );

    // Insert root element
    root = xmlProfileDoc->InsertEndChild( TiXmlElement( "Presage" ) );
    assert( root );

    // PluginRegistry module
    module = root->InsertEndChild( TiXmlElement( "PluginRegistry" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild(TiXmlElement("LOGGER"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_LOGGER_LEVEL;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }

        element = module->InsertEndChild( TiXmlElement( "PLUGINS" ) );
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_PLUGINS;
            node = element->InsertEndChild(TiXmlText( ss.str().c_str() ));
            assert( node );
        }
    }

    // ContextTracker module
    module = root->InsertEndChild( TiXmlElement( "ContextTracker" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild(TiXmlElement("LOGGER"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_LOGGER_LEVEL;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }

        element = module->InsertEndChild( TiXmlElement( "SLIDING_WINDOW_SIZE" ) );
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_SLIDING_WINDOW_SIZE;
            node = element->InsertEndChild(TiXmlText( ss.str().c_str() ));
            assert( node );
        }
    }
	
    // Selector module
    module = root->InsertEndChild( TiXmlElement( "Selector" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild(TiXmlElement("LOGGER"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_LOGGER_LEVEL;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }

        element = module->InsertEndChild(TiXmlElement("SUGGESTIONS"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_SUGGESTIONS;
            node = element->InsertEndChild(TiXmlText( ss.str().c_str() ));
            assert( node );
        }
		
        element = module->InsertEndChild( TiXmlElement( "REPEAT_SUGGESTIONS" ) );
        assert( element );
        if( element ) {
            node = element->InsertEndChild( TiXmlText( "no" ) );
            assert( node );
        }
		
        element = module->InsertEndChild( TiXmlElement( "GREEDY_SUGGESTION_THRESHOLD" ) );
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_GREEDY_SUGGESTION_THRESHOLD;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
    }

    // PredictorActivator module
    module = root->InsertEndChild( TiXmlElement( "PredictorActivator" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild(TiXmlElement("LOGGER"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_LOGGER_LEVEL;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }

        element = module->InsertEndChild(TiXmlElement("PREDICT_TIME"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_PREDICT_TIME;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }

        element = module->InsertEndChild(TiXmlElement("MAX_PARTIAL_PREDICTION_SIZE"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_MAX_PARTIAL_PREDICTION_SIZE;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }

        element = module->InsertEndChild( TiXmlElement( "COMBINATION_POLICY" ) );
        assert( element );
        if( element ) {
            std::stringstream ss;
            ss << DEFAULT_COMBINATION_POLICY;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
    }

    // ProfileManager module
    module = root->InsertEndChild( TiXmlElement( "ProfileManager" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild(TiXmlElement("LOGGER"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_LOGGER_LEVEL;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
    }

    //PLUMP
    /*
    // PluginManager module
    module = root->InsertEndChild( TiXmlElement( "PluginManager" ) );
    assert( module );
    if( module ) {
    element = module->InsertEndChild( TiXmlElement( "PLUGIN_PATH" ) );
    assert( element );
    if( element ) {
    std::stringstream ss;
    ss << DEFAULT_PLUGIN_PATH;
    node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
    assert( node );
    }

    element = module->InsertEndChild( TiXmlElement( "PLUGIN_SUFFIX" ) );
    assert( element );
    if( element ) {
    std::stringstream ss;
    ss << DEFAULT_PLUGIN_SUFFIX;
    node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
    assert( node );
    }
    }
    */

    // Plugin modules
    module = root->InsertEndChild( TiXmlElement( "Plugins" ) );
    assert( module );
    if( module ) {
      
      module = module->InsertEndChild( TiXmlElement( "RecencyPlugin" ) );
      assert( module );
      if( module ) {
        element = module->InsertEndChild(TiXmlElement("LOGGER"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_LOGGER_LEVEL;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }	
        element = module->InsertEndChild(TiXmlElement("LAMBDA"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << "1";
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
        element = module->InsertEndChild(TiXmlElement("N_0"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << "1";
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
        element = module->InsertEndChild(TiXmlElement("CUTOFF_THRESHOLD"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << "20";
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }

      }

    }

    // print out doc for debug purposes
    // result.Print();
}
