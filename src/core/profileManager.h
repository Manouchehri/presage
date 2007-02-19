
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

#ifndef SOOTH_PROFILEMANAGER
#define SOOTH_PROFILEMANAGER

#include "core/historyTracker.h"
#include "core/predictor.h"
#include "core/selector.h"
//PLUMP #include "core/pluginManager.h"
#include "plugins/plugin.h"

#include "tinyxml/tinyxml.h"

#include <string>
#include <sstream> // for std::ostringstream
#include <stdio.h> // for int remove( const char* ) to remove files/dirs


const char DEFAULT_PROFILE_FILENAME[] = "soothsayer.xml";

/** Juggles configuration files and soothsayer system initialization.
 *
 * The idea is that ProfileManager loads up an xml file containing
 * configuration data and initializes system components according to
 * the config file.
 * 
 * Configuration files:
 *   sysconfdir/soothsayer.xml
 *   ~/.soothsayer/soothsayer.xml
 *   /etc/soothsayer/soothsayer.xml
 *
 */
class ProfileManager {
public:
    //PLUMP ProfileManager(HistoryTracker&, Predictor&, Selector&, PluginManager&);
    ProfileManager(HistoryTracker&, Predictor&, Selector&);
    ~ProfileManager();

    void initHistoryTracker();
    void initPredictor();
    void initSelector();
    // Init predictive plugins
    //PLUMP void initPluginManager();

    bool loadProfile(const std::string = DEFAULT_PROFILE_FILENAME);
    void buildProfile(const std::string = DEFAULT_PROFILE_FILENAME);
    bool saveProfile() const;

private:
    HistoryTracker& historyTracker;
    Predictor&      predictor;
    Selector&       selector;
    //PLUMP PluginManager&  pluginManager;

    TiXmlDocument*  profileDoc;
    std::string     profileFile;

};


#endif // SOOTH_PROFILEMANAGER
