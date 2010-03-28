
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

const char* ProfileManager::LOGGER = "Presage.ProfileManager.LOGGER";

/** Constructor.
 *
 * Initialises other modules.
 *
 */
ProfileManager::ProfileManager(const std::string profilename)
    : logger("ProfileManager", std::cerr)
{
    configuration = new Configuration();
    loaded_at_least_one_profile = false;
    rw_profile = 0;

    init_profiles (profilename);
}


/** Destructor.
 *
 */
ProfileManager::~ProfileManager()
{
    flush_cached_log_messages ();

    save_profile ();

    delete configuration;
}


Profile* ProfileManager::create_profile_from_xml (const std::string& filename)
{
        Profile* profile = new Profile (filename);

	std::string message;
	if (profile->file_read_ok ()) {
            // logger has not been init'd with configuration, because no
	    // profile is known yet, hence caching this logging item,
            // which will be flushed when configuration is finally read in
	    //
	    message = "Loaded profile: " + filename;
	  
	    cache_log_message(logger.NOTICE, message);
	  	  
	} else {
	    // logger has not been init'd with configuration, because no
	    // profile is known yet, hence caching this logging item,
	    // which will be flushed when configuration is finally read in
	    //
	    message = "Failed to load profile: " + filename;
	  
	    cache_log_message(logger.NOTICE, message);

	}
	
	return profile;
}


void ProfileManager::init_profiles (const std::string& profilename)
{
    std::list<std::string> profiles;

    // system etc directory
    profiles.push_back (static_cast<std::string>("/etc") + '/' + DefaultProfile::DEFAULT_PROFILE_FILENAME);
    // installation config directory
    profiles.push_back (static_cast<std::string>(sysconfdir) + '/' + DefaultProfile::DEFAULT_PROFILE_FILENAME);
    // home dir dotfile
    profiles.push_back (get_user_home_dir() + '/' + '.' + DefaultProfile::DEFAULT_PROFILE_FILENAME);
    // user specified profile (if any)
    if (! profilename.empty()) {
        profiles.push_back(profilename);
    }


    // read data from each standard profile and write it to configuration
    Profile* profile = 0;
    for (std::list<std::string>::const_iterator it = profiles.begin();
	 it != profiles.end();
	 it++ ) {
	delete profile;
        profile = create_profile_from_xml (*it);
	loaded_at_least_one_profile = loaded_at_least_one_profile || profile->file_read_ok ();
	profile->read_into_configuration (configuration);
    }

    // remember last profile as writable profile
    rw_profile = profile;

    if (! loaded_at_least_one_profile) {
        // load default profile values
        DefaultProfile default_profile (profiles.back());
	default_profile.read_into_configuration (configuration);
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
void ProfileManager::save_profile() const
{
    bool saveOk = rw_profile->write_to_file ();
    if (! saveOk) {
        logger << ERROR << "Failed to save configuration to profile " << endl;
    }
}


/** Create new profile with default values.
 *
 */
//TiXmlDocument* ProfileManager::buildProfile(const std::string p)
//{
//}


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
