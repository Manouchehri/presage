
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


#ifndef PRESAGE_PROFILEMANAGER
#define PRESAGE_PROFILEMANAGER

#include "core/profile.h"
#include "tinyxml/tinyxml.h"
#include "core/combiner.h"
#include "core/logger.h"

#include <string>
#include <list>
#include <sstream> // for std::ostringstream
#include <stdio.h> // for int remove( const char* ) to remove files/dirs


const char DEFAULT_PROFILE_FILENAME[] = "presage.xml";

const std::string  DEFAULT_LOGGER_LEVEL                 = "DEBUG";
const int          DEFAULT_PREDICT_TIME                = 1000;
const int          DEFAULT_MAX_PARTIAL_PREDICTION_SIZE = 100;
const std::string  DEFAULT_COMBINATION_POLICY          = "Meritocracy";
const std::string  DEFAULT_PREDICTIVE_PLUGINS          = "";
const int          DEFAULT_SLIDING_WINDOW_SIZE         = 80;
const size_t       DEFAULT_SUGGESTIONS                 = 6;
const bool         DEFAULT_REPEAT_SUGGESTION           = false;
const unsigned int DEFAULT_GREEDY_SUGGESTION_THRESHOLD = 0;
const std::string  DEFAULT_PLUGINS                     = "";

/** Juggles configuration files and presage system initialization.
 *
 * The idea is that ProfileManager loads up an xml file containing
 * configuration data and initializes system components according to
 * the config file.
 * 
 * If no profile is supplied, load the default profile in the
 * following order:
 *  - ~/.presage.xml
 *  - sysconfdir/presage.xml
 *
 */
class ProfileManager {
public:
    ProfileManager(const std::string = "");
    ~ProfileManager();

    bool loadDefaultProfile();
    bool loadProfile(const std::string);
    void buildProfile(const std::string = DEFAULT_PROFILE_FILENAME);
    bool saveProfile() const;
    Profile* getProfile();
    
private:
    std::string get_user_home_dir() const;
    
    /** Cache log message until logger level is read from configuration.
     */
    void cache_log_message(Logger<char>::Level level, const std::string& message);
    /** Flush cached log messages.
     */
    void flush_cached_log_messages();
    /** Refresh configuration variables
     */
    void refresh_config(Profile* profile);

    struct CachedLogMessage {
	// Level is commented out because it gets translated to
	// numeric value of enum, instead of acting as templatized
	// function type switching the logger level as intended.  This
	// is not an issue however since all cached log messages need
	// to be logged at same level, so we can hardcode level in
	// flush method.
	//
	//Logger<char>::Level level;
	std::string         message;
    };

    std::list<CachedLogMessage> cached_log_messages;

    TiXmlDocument*  xmlProfileDoc;
    std::string     profileFile;
    Logger<char>    logger;
};


#endif // PRESAGE_PROFILEMANAGER
