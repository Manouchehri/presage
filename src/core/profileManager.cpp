
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


#include "core/profileManager.h"
#include "core/utility.h"        // isYes isNo isTrue isFalse utility function
#include "dirs.h"                // sysconfdir macro define

/** Constructor.
 *
 * Initialises other modules.
 *
 */
ProfileManager::ProfileManager( HistoryTracker& ht,
				Predictor& p,
				Selector& s //PLUMP,
                                //PLUMP				PluginManager& pm )
    )
    : historyTracker( ht ),
      predictor( p ),
      selector( s ) //PLUMP,
    //PLUMP	  pluginManager( pm )
{
    profileDoc = 0;

    loadProfile();

    initHistoryTracker();
    initPredictor();
    initSelector();
}


/** Destructor.
 *
 */
ProfileManager::~ProfileManager()
{}


/** Initialises HistoryTracker.
 *
 */
void ProfileManager::initHistoryTracker()
{
	
    ///////////////////////////////////////////////////////
    // Check this code: I wrote it very late at night after
    // a long abstinence from coding!!! Be wary!!!
    // TODO TODO TODO TODO
    ///////////////////////////////////////////////////////


    // Get handle to profile document root
    TiXmlHandle doc( profileDoc );

    // Get to HistoryTracker node
    TiXmlElement* module = doc.FirstChild( "Soothsayer" ).FirstChild( "HistoryTracker" ).Element();
    assert( module ); // TODO handle with exceptions
    if( NULL != module ) {
        // we found HistoryTracker option block,
        // let's handle each option

        TiXmlNode* option;
        TiXmlText* text;

        // option MAX_BUFFER_SIZE
        option = module->FirstChild( "MAX_BUFFER_SIZE" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option MAX_BUFFER_SIZE: " << text->Value() << std::endl;
                historyTracker.setMaxBufferSize( atoi( text->Value() ) );

            }
        }
    }
}


/** Initializes Predictor.
 */
void ProfileManager::initPredictor()
{
    // new profile pulling approach
    //
    predictor.setProfile(getProfile());

    // old profile pushing approach
    //
    // Get handle to profile document root
    TiXmlHandle doc( profileDoc );

    // Get to Predictor node
    TiXmlElement* module = doc.FirstChild( "Soothsayer" ).FirstChild( "Predictor" ).Element();
    assert( module ); // TODO handle with exceptions
    if( NULL != module ) {
        // we found Predictor option block,
        // let's handle each option

        TiXmlNode* option;
        TiXmlText* text;

        // Option PREDICT_TIME
        option = module->FirstChild( "PREDICT_TIME" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option PREDICT_TIME: " << text->Value() << std::endl;
                predictor.setPredictTime( atoi( text->Value() ) );
            }
        }
		
        // Option COMBINATION_METHOD
        option = module->FirstChild( "COMBINATION_METHOD" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option COMBINATION_METHOD: " << text->Value() << std::endl;
                predictor.setCombinationMethod( static_cast<CombinationMethod>( atoi( text->Value() ) ) );
            }
        }
    }
}


/** Initializes Selector.
 */
void ProfileManager::initSelector()
{

    // TODO: implement set methods in Selector object. Handle yes/no option value.

    // Get handle to profile document root
    TiXmlHandle doc( profileDoc );

    // Get to Selector node
    TiXmlElement* module = doc.FirstChild( "Soothsayer" ).FirstChild( "Selector" ).Element();
    assert( module ); // TODO: handle with exceptions instead of assertions
    if( NULL != module ) {
        // we found Selector option block,
        // let's handle each option

        TiXmlNode* option;
        TiXmlText* text;

        // Option SUGGESTIONS
        option = module->FirstChild( "SUGGESTIONS" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option SUGGESTIONS: " << text->Value() << std::endl;
                selector.setSuggestions( atoi( text->Value() ) );
            }
        }
		
        // Option REPEAT_SUGGESTIONS
        option = module->FirstChild( "REPEAT_SUGGESTIONS" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option REPEAT_SUGGESTIONS: " << text->Value() << std::endl;

                if( isYes( text->Value() ) || isTrue( text->Value() ) )
                    selector.setRepeatSuggestion( true );
                else if( isNo( text->Value() ) || isFalse( text->Value() ) )
                    selector.setRepeatSuggestion( false );
            }
        }

        // Option GREEDY_SUGGESTION_THRESHOLD
        option = module->FirstChild( "GREEDY_SUGGESTION_THRESHOLD" );
        assert( option );
        if( NULL!= option ) {
            text = option->FirstChild()->ToText();
            assert( text );
            if( NULL != text ) {
                std::cout << "[ProfileManager] Option GREEDY_SUGGESTION_THRESHOLD: " << text->Value() << std::endl;
                selector.setGreedySuggestionThreshold( atoi( text->Value() ) );
            }
        }
    }

}


/** Initializes PluginManager and managed Plugin objects.
 */
//PLUMP void ProfileManager::initPluginManager()
//PLUMP {
//PLUMP 	
//PLUMP }


///** Allows user to select which profile to use.
// *
// * User can:
// * - select a profile
// * - load a profile
// * - edit a profile
// * - save a profile
// * - remove a profile
// * - create a new profile
// */
//void ProfileManager::selectProfile()
//{}


/** Load and parse a profile.
 *
 * Load profile profilename.  The profile is searched in various
 * directories.  The first directory containing a file whose name
 * corresponds to the specified profile is loaded.
 *
 * The directories searched and their search order is:
 * - sysconfdir/soothsayer.xml
 * - ~/.soothsayer/soothsayer.xml (TBD)
 * - /etc/soothsayer/soothsayer.xml
 *
 * If no profile is found, a default profile is built and used.
 *
 * Return true if profile is found and successfully loaded, false
 * otherwise.
 *
 */
bool ProfileManager::loadProfile(const std::string profilename)
{
    profileDoc = new TiXmlDocument;
    assert( profileDoc );
    
    const int PROFILE_DIRS_SIZE = 2;
    std::string profile_dirs[PROFILE_DIRS_SIZE] = {
	sysconfdir,
	"/etc/soothsayer"
    };

    bool readOk = false;
    int i = 0;
    while(!readOk && i < PROFILE_DIRS_SIZE) {
	profileFile = profile_dirs[i] + '/' + profilename;
	readOk = profileDoc->LoadFile (profileFile.c_str());

        if (!readOk) {
            std::cerr << "[ProfileManager] Opening profile: '"
                      << profileFile << "' attempt failed." << std::endl;
        }

	i++;
    }

    if (readOk) {
	std::cout << "[ProfileManager] Using profile '" << profileFile
		  << "'" << std::endl;
    } else {
	// Handle failure to load profile
	std::cerr << "[ProfileManager] No profiles were found. Using default parameters."
		  << std::endl;
	buildProfile();
    }

    return readOk;
}


/** Write profile to disk.
 *
 * Returns true is file was saved successfully, false otherwise.
 *
 */
bool ProfileManager::saveProfile() const
{
    bool saveOk = profileDoc->SaveFile();

    return saveOk;
}


/** Create new profile with default values.
 *
 */
void ProfileManager::buildProfile(const std::string p)
{
    TiXmlNode* root;
    TiXmlNode* node;
    TiXmlNode* module;
    TiXmlNode* element;
	

    // Create document
    profileDoc = new TiXmlDocument(p.c_str());

    // Insert initial mandatory declaration
    node = profileDoc->InsertEndChild( TiXmlDeclaration( "1.0", "UTF-8", "no" ) );
    assert( node );

    // Insert root element
    root = profileDoc->InsertEndChild( TiXmlElement( "Soothsayer" ) );
    assert( root );

    // HistoryTracker module
    module = root->InsertEndChild( TiXmlElement( "HistoryTracker" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild( TiXmlElement( "MAX_BUFFER_SIZE" ) );
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_MAX_BUFFER_SIZE;
            node = element->InsertEndChild(TiXmlText( ss.str().c_str() ));
            assert( node );
        }
    }
	
    // Selector module
    module = root->InsertEndChild( TiXmlElement( "Selector" ) );
    assert( module );
    if( module ) {
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

    // Predictor module
    module = root->InsertEndChild( TiXmlElement( "Predictor" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild(TiXmlElement("PREDICT_TIME"));
        assert( element );
        if( element ) {
            std::ostringstream ss;
            ss << DEFAULT_PREDICT_TIME;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
		
        element = module->InsertEndChild( TiXmlElement( "COMBINATION_METHOD" ) );
        assert( element );
        if( element ) {
            std::stringstream ss;
            ss << DEFAULT_COMBINATION_METHOD;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
    }

    // ProfileManager module
    module = root->InsertEndChild( TiXmlElement( "ProfileManager" ) );
    assert( module );
    if( module ) {
        element = module->InsertEndChild(TiXmlElement("DUMMY_OPTION"));
        assert( element );
        if( element ) {
            node = element->InsertEndChild( TiXmlText( "very dummy" ) );
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

        // handled by PluginManager object
        //PLUMP		pluginManager.createProfile( module );


    }

    // print out doc for debug purposes
    // profileDoc.Print();

}

Profile* ProfileManager::getProfile() const
{
    //Profile profile(profileDoc);
    //return profile;

    return new Profile(profileDoc);
}
