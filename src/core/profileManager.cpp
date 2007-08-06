
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


#ifdef DEBUG
# define LOG(x) std::cout << x << std::endl
#else
# define LOG(x) /* x */
#endif


/** Constructor.
 *
 * Initialises other modules.
 *
 */
ProfileManager::ProfileManager()
{
    xmlProfileDoc = 0;
    loadProfile();
}


/** Destructor.
 *
 */
ProfileManager::~ProfileManager()
{
    delete xmlProfileDoc;
}


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
    // destroy xml profile document to prevent memory leaks.
    delete xmlProfileDoc;

    xmlProfileDoc = new TiXmlDocument;
    assert( xmlProfileDoc );
    
    const int PROFILE_DIRS_SIZE = 2;
    std::string profile_dirs[PROFILE_DIRS_SIZE] = {
	sysconfdir,
	"/etc/soothsayer"
    };

    bool readOk = false;
    int i = 0;
    while(!readOk && i < PROFILE_DIRS_SIZE) {
	profileFile = profile_dirs[i] + '/' + profilename;
	readOk = xmlProfileDoc->LoadFile (profileFile.c_str());

        if (!readOk) {
            LOG("[ProfileManager] Opening profile: '" << profileFile << "' attempt failed.");
        }

	i++;
    }

    if (readOk) {
	LOG("[ProfileManager] Using profile '" << profileFile);
    } else {
	// Handle failure to load profile
	LOG("[ProfileManager] No profiles were found. Using default parameters.");
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
    // result.Print();

}


Profile* ProfileManager::getProfile() const
{
    return new Profile(xmlProfileDoc);
}
