
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


#include "core/profileManager.h"
#include "core/utility.h"        // isYes isNo isTrue isFalse utility function
#include "dirs.h"                // sysconfdir macro define

#include <pwd.h>
#include <stdlib.h>

/** Constructor.
 *
 * Initialises other modules.
 *
 */
ProfileManager::ProfileManager(const std::string profilename)
    : logger("ProfileManager", std::cerr)
{
    xmlProfileDoc = 0;
    if (profilename.empty()) {
        loadDefaultProfile();
    } else {
        loadProfile(profilename);
    }
}


/** Destructor.
 *
 */
ProfileManager::~ProfileManager()
{
    delete xmlProfileDoc;
}


std::string ProfileManager::get_user_home_dir() const
{
    std::string result;
  
    uid_t me;
    struct passwd *my_passwd;
    
    me = getuid ();
    my_passwd = getpwuid (me);
    if (!my_passwd) {
        // got passwd for user
        // read home dir from passwd struct
	result = my_passwd->pw_dir;
    } else {
        // unable to get passwd struct,
        // read $HOME env variable
        result = getenv("HOME");
    }

    return result;
}

/** Load and parse the default user/system profile.
 *
 * Load default profile in the following order:
 * - ~/.soothsayer.xml
 * - sysconfdir/soothsayer.xml
 *
 * If no profile is found, a default profile is built and used.
 *
 * @return true if profile is found and successfully loaded, false
 * otherwise.
 *
 */
bool ProfileManager::loadDefaultProfile()
{
    const int PROFILE_SEARCH_PATH_SIZE = 2;
    std::string profile_search_path[PROFILE_SEARCH_PATH_SIZE] = {
        // home dir dotfile
        get_user_home_dir() + '/' + '.' + DEFAULT_PROFILE_FILENAME,
        // installation config directory
	static_cast<std::string>(sysconfdir) + '/' + DEFAULT_PROFILE_FILENAME
    };

    bool readOk = false;

    // try looking for profilename in profile dirs
    int i = 0;
    while(!readOk && i < PROFILE_SEARCH_PATH_SIZE) {
        readOk = loadProfile(profile_search_path[i]);
        i++;
    }

    if (!readOk) {
        // handle failure to load profile
        logger << ERROR << "No profiles were found. Using default parameters." << endl;
        buildProfile();
    }

    return readOk;
}


/** Load and parse the specified profile.
 *
 * @param profile_file filename of the profile to load
 * @return true if profile is found and successfully loaded, false
 * otherwise.
 *
 */
bool ProfileManager::loadProfile(const std::string profile_file)
{
    // destroy xml profile document to prevent memory leaks.
    delete xmlProfileDoc;

    xmlProfileDoc = new TiXmlDocument;
    assert( xmlProfileDoc );

    bool readOk = xmlProfileDoc->LoadFile (profile_file.c_str());
    
    if (readOk) {
      logger << ERROR << "Using profile '" << profile_file << "'..." << endl;
    } else {
      logger << ERROR << "Opening profile '" << profile_file << "' attempt failed." << endl;
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
    bool saveOk = xmlProfileDoc->SaveFile();
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
    delete xmlProfileDoc;
    xmlProfileDoc = new TiXmlDocument(p.c_str());

    // Insert initial mandatory declaration
    node = xmlProfileDoc->InsertEndChild( TiXmlDeclaration( "1.0", "UTF-8", "no" ) );
    assert( node );

    // Insert root element
    root = xmlProfileDoc->InsertEndChild( TiXmlElement( "Soothsayer" ) );
    assert( root );

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

    // Predictor module
    module = root->InsertEndChild( TiXmlElement( "Predictor" ) );
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

        element = module->InsertEndChild( TiXmlElement( "PLUGINS" ) );
        assert( element );
        if( element ) {
            std::stringstream ss;
            ss << DEFAULT_PREDICTIVE_PLUGINS;
            node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
            assert( node );
        }
    }

    // ProfileManager module
    module = root->InsertEndChild( TiXmlElement( "ProfileManager" ) );
    assert( module );
    if( module ) {
        //element = module->InsertEndChild(TiXmlElement("LOGGER"));
        //assert( element );
        //if( element ) {
        //    std::ostringstream ss;
        //    ss << DEFAULT_LOGGER_LEVEL;
        //    node = element->InsertEndChild( TiXmlText( ss.str().c_str() ) );
        //    assert( node );
        //}
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
    // result.Print();

}


Profile* ProfileManager::getProfile() const
{
    return new Profile(xmlProfileDoc);
}
