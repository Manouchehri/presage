
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


#include "core/profileManager.h"
#include "core/utility.h"        // isYes isNo isTrue isFalse utility function
#include "dirs.h"                // sysconfdir macro define
#include "core/configuration.h"

#ifdef HAVE_PWD_H
# include <pwd.h>
#endif

#if HAVE_UNISTD_H
# include <sys/types.h>
# include <unistd.h>
#endif

#if HAVE_STDLIB_H
# include <stdlib.h>
#endif

const char*        ProfileManager::LOGGER = "Presage.ProfileManager.LOGGER";

const char*        ProfileManager::DEFAULT_PROFILE_FILENAME            = "presage.xml";
const std::string  ProfileManager::DEFAULT_LOGGER_LEVEL                = "DEBUG";
const int          ProfileManager::DEFAULT_PREDICT_TIME                = 1000;
const int          ProfileManager::DEFAULT_MAX_PARTIAL_PREDICTION_SIZE = 100;
const std::string  ProfileManager::DEFAULT_COMBINATION_POLICY          = "Meritocracy";
const std::string  ProfileManager::DEFAULT_PREDICTIVE_PLUGINS          = "";
const int          ProfileManager::DEFAULT_SLIDING_WINDOW_SIZE         = 80;
const size_t       ProfileManager::DEFAULT_SUGGESTIONS                 = 6;
const bool         ProfileManager::DEFAULT_REPEAT_SUGGESTION           = false;
const size_t       ProfileManager::DEFAULT_GREEDY_SUGGESTION_THRESHOLD = 0;
const std::string  ProfileManager::DEFAULT_PLUGINS                     = "";

/** Constructor.
 *
 * Initialises other modules.
 *
 */
ProfileManager::ProfileManager(const std::string profilename)
    : logger("ProfileManager", std::cerr)
{
    init_profiles (profilename);

    configuration = new Configuration();

    read_profiles_into_configuration ();
}


/** Destructor.
 *
 */
ProfileManager::~ProfileManager()
{
    flush_cached_log_messages();

    delete configuration;
}


void ProfileManager::read_profiles_into_configuration ()
{
    for (std::list<std::string>::const_iterator it = profiles.begin();
	 it != profiles.end();
	 it++ ) {
        Profile* profile = new Profile (*it);
	profile->read_into_configuration (configuration);
	delete profile;
    }
}


void ProfileManager::init_profiles (const std::string& profilename)
{
    // system etc directory
    profiles.push_back (static_cast<std::string>("/etc") + '/' + DEFAULT_PROFILE_FILENAME);
    // installation config directory
    profiles.push_back (static_cast<std::string>(sysconfdir) + '/' + DEFAULT_PROFILE_FILENAME);
    // home dir dotfile
    profiles.push_back (get_user_home_dir() + '/' + '.' + DEFAULT_PROFILE_FILENAME);
    // user specified profile (if any)
    if (! profilename.empty()) {
        profiles.push_back (profilename);
    }
}


std::string ProfileManager::get_user_home_dir() const
{
    std::string result;
  
#ifdef HAVE_PWD_H
    uid_t me;
    struct passwd *my_passwd;
    
    me = getuid ();
    my_passwd = getpwuid (me);
    if (!my_passwd) {
        // got passwd for user
        // read home dir from passwd struct
	result = my_passwd->pw_dir;
    } else 
        // unable to get passwd struct,
        // read $HOME env variable
#endif // HAVE_PWD_H
    {
        const char* HOME = "HOME";
        char* value = getenv(HOME);
        // check if HOME env var exists,
        // assigning a null pointer to a string is
        // not such a good idea...
        if (value) {
            result = value;
        }
    }

    return result;
}


/** Write profile to disk.
 *
 * Returns true is file was saved successfully, false otherwise.
 *
 */
void ProfileManager::saveProfile() const
{
  // TODO
  //bool saveOk = xmlProfileDoc->SaveFile();
  //return saveOk;
}


/** Create new profile with default values.
 *
 */
TiXmlDocument* ProfileManager::buildProfile(const std::string p)
{
    TiXmlNode* root;
    TiXmlNode* node;
    TiXmlNode* module;
    TiXmlNode* element;
	

    // Create document
    TiXmlDocument* xmlProfileDoc = new TiXmlDocument(p.c_str());

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

        // handled by PluginManager object
        //PLUMP		pluginManager.createProfile( module );


    }

    // print out doc for debug purposes
    // result.Print();

    return xmlProfileDoc;
}


Configuration* ProfileManager::get_configuration()
{
    // since a Profile is being returned, we know we have a valid
    // configuration object. Here, we obtain a temporary Configuration
    // object to read the this ProfileManager configuration.  We could
    // not do this during profile manager construction because no
    // profile was available at that time.
    //
    refresh_config();
    return configuration;
}

void ProfileManager::cache_log_message(Logger<char>::Level level, const std::string& message)
{
    static CachedLogMessage clm;
    //clm.level = level;
    clm.message = message;
    //std::cerr << "Caching message: " << message << std::endl;
    cached_log_messages.push_back(clm);
}

void ProfileManager::flush_cached_log_messages()
{
    std::list<CachedLogMessage>::const_iterator it = cached_log_messages.begin();
    while (it != cached_log_messages.end()) {
	//std::cerr << "Flushing message: " << it->message << std::endl;
	logger << NOTICE << it->message << endl;
	it++;
    }
    cached_log_messages.clear();
}

void ProfileManager::refresh_config()
{
    try {
        logger << setlevel(configuration->find (LOGGER)->get_value());
    } catch (Configuration::ConfigurationException& ex) {
	// if no config is available, turn on full logging for profile
	// manager
	logger << setlevel("ALL");
    }

    flush_cached_log_messages();
}
