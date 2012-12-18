
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


#include "profileManager.h"
#include "utility.h"        // isYes isNo isTrue isFalse utility function
#include "dirs.h"                // sysconfdir macro define
#include "configuration.h"

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

#if defined(_WIN32)
# include <windows.h>
#endif

const char* ProfileManager::LOGGER = "Presage.ProfileManager.LOGGER";
const char* ProfileManager::AUTOPERSIST = "Presage.ProfileManager.AUTOPERSIST";

/** Constructor.
 *
 * Initialises other modules.
 *
 */
ProfileManager::ProfileManager(const std::string profilename)
    : logger("ProfileManager", std::cerr)
{
    config = new Configuration();
    loaded_at_least_one_profile = false;
    rw_profile = 0;
    autopersist_config = false;

    init_profiles (profilename);
}


/** Destructor.
 *
 */
ProfileManager::~ProfileManager()
{
    flush_cached_log_messages ();

    set_autopersist (config->find (AUTOPERSIST)->get_value ());
    if (autopersist_config) {
	save_profile ();
    }

    delete config;
    delete rw_profile;
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

    {
        // load default profile values
        DefaultProfile default_profile ("");
	default_profile.read_into_configuration (config);
    }


    // system etc directory
    profiles.push_back (get_system_etc_dir() + '/' + DefaultProfile::DEFAULT_PROFILE_FILENAME);
    // installation config directory
    profiles.push_back (static_cast<std::string>(sysconfdir) + '/' + DefaultProfile::DEFAULT_PROFILE_FILENAME);
    // home dir dotfile
    profiles.push_back (get_user_home_dir() + "/.presage/" + DefaultProfile::DEFAULT_PROFILE_FILENAME);
    // user specified profile (if any)
    if (! profilename.empty()) {
        profiles.push_back(profilename);
    }


    // read data from each profile and write it to configuration
    Profile* profile = 0;
    for (std::list<std::string>::const_iterator it = profiles.begin();
	 it != profiles.end();
	 it++ ) {
	delete profile;
        profile = create_profile_from_xml (*it);
	loaded_at_least_one_profile = loaded_at_least_one_profile || profile->file_read_ok ();
	profile->read_into_configuration (config);
    }

    // remember last profile as writable profile
    rw_profile = profile;
}


std::string ProfileManager::get_system_etc_dir() const
{
     std::string result;

#if defined(_WIN32)
     DWORD size;
     DWORD type;
     LONG res;
     HKEY reg_key = NULL;
     char *dst = NULL;

     res = RegOpenKeyExA (HKEY_CURRENT_USER, "Software\\Presage", 0,
			  KEY_READ, &reg_key);

     if (res == ERROR_SUCCESS) 
     {
	 size = 64;
	 dst = (char*) malloc (size);
	 
	 res = RegQueryValueExA (reg_key, "", 0, &type,
				 (LPBYTE) dst, &size);
	 if (res == ERROR_MORE_DATA && type == REG_SZ) {
	     char* tmp_dst = (char*) realloc (dst, size);
             if (tmp_dst != NULL) {
                 dst = tmp_dst;
             } else {
		 // realloc failed, try to free and malloc
		 free (dst);
		 dst = (char*) malloc (size);
		 if (dst == NULL) {
		     // malloc failed, cannot recover, just return empty result
		     return result;
		 }
	     }
	     res = RegQueryValueExA (reg_key, "", 0,
				     &type, (LPBYTE) dst,
				     &size);
	 }
	 
	 if (type != REG_SZ || res != ERROR_SUCCESS) 
	 {
	     result = ".";
	 }
	 else
	 {
	     result = dst;
	     result += "\\etc";
	 }

	 free (dst);
	 dst = 0;
	 RegCloseKey (reg_key);
     }

#else
     result = "/etc";
#endif

     return result;
}


std::string ProfileManager::get_user_home_dir() const
{
    std::string result;

#ifdef _WIN32
    const char* USERPROFILE = "USERPROFILE";
    char* value = getenv(USERPROFILE);
    // check if USERPROFILE env var exists...
    if (value) {
        result = value;
    }
#else
# ifdef HAVE_PWD_H
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
# endif // HAVE_PWD_H
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
#endif

    return result;
}


/** Write profile to disk.
 *
 * Returns true is file was saved successfully, false otherwise.
 *
 */
void ProfileManager::save_profile() const
{
    rw_profile->read_from_configuration (config);
    bool saveOk = rw_profile->write_to_file ();
    if (! saveOk) {
        logger << ERROR << "Failed to save configuration to profile " << endl;
    }
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
    return config;
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
        logger << setlevel(config->find (LOGGER)->get_value());
    } catch (Configuration::ConfigurationException& ex) {
	// if no config is available, turn on full logging for profile
	// manager
	logger << setlevel("ALL");
    }

    flush_cached_log_messages();
}

void ProfileManager::set_autopersist (const std::string& value)
{
    autopersist_config = Utility::isTrue (value);
    logger << INFO << "AUTOPERSIST: " << autopersist_config << endl;
}
