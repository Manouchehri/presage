
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

#ifndef SOOTH_PROFILEMANAGER
#define SOOTH_PROFILEMANAGER

#include "core/profile.h"
#include "tinyxml/tinyxml.h"
#include "core/combiner.h"
#include "core/logger.h"

#include <string>
#include <sstream> // for std::ostringstream
#include <stdio.h> // for int remove( const char* ) to remove files/dirs


const char DEFAULT_PROFILE_FILENAME[] = "soothsayer.xml";

const std::string  DEFAULT_LOGGER_LEVEL                 = "DEBUG";
const int          DEFAULT_PREDICT_TIME                = 1000;
const int          DEFAULT_MAX_PARTIAL_PREDICTION_SIZE = 100;
const std::string  DEFAULT_COMBINATION_POLICY          = "Meritocracy";
const std::string  DEFAULT_PREDICTIVE_PLUGINS          = "";
const int          DEFAULT_MAX_BUFFER_SIZE             = 1024;  // TODO: <==== not implemented yet
const int          DEFAULT_SUGGESTIONS                 = 6;
const bool         DEFAULT_REPEAT_SUGGESTION           = false;
const unsigned int DEFAULT_GREEDY_SUGGESTION_THRESHOLD = 0;


/** Juggles configuration files and soothsayer system initialization.
 *
 * The idea is that ProfileManager loads up an xml file containing
 * configuration data and initializes system components according to
 * the config file.
 * 
 * If no profile is supplied, load the default profile in the
 * following order:
 *  - ~/.soothsayer.xml
 *  - sysconfdir/soothsayer.xml
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
    Profile* getProfile() const;

private:
    std::string get_user_home_dir() const;

    TiXmlDocument*  xmlProfileDoc;
    std::string     profileFile;
    Logger<char>    logger;
};


#endif // SOOTH_PROFILEMANAGER
