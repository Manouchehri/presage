
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

#ifndef SOOTH_PLUGINMANAGER
#define SOOTH_PLUGINMANAGER


#include "core/predictor.h"
#include "plugins/plugin.h"

#include <vector>
#include <string>

#include <stdio.h>           // needed for directory and files manipulation
#include <sys/types.h>       // 
#include <dirent.h>          // 

//#include <dlfcn.h>           // needed for dynamic loading of shared libraries
#include <ltdl.h>

#include "tinyxml/tinyxml.h" // needed for creation of default option profile


const char DEFAULT_PLUGIN_PATH[] = "plugins/.libs/";
const char DEFAULT_PLUGIN_SUFFIX[] = ".so";

/** PluginManager manages Plugin objects.
 *
 */
class PluginManager {
  public:
    PluginManager( HistoryTracker&, Predictor& );
    ~PluginManager();


    void loadOptions(); // load options into plugin
    void createProfile( TiXmlNode* );

    // register active plugins with predictor
    void registerPlugins() const; 
    void unregisterPlugins() const;

  private:
    void init();

    bool checkSuffix( const char*, const char* ) const;
	
    Plugin* createPlugin( const std::string );
    void destroyPlugin( Plugin* );


    HistoryTracker& historyTracker;
    Predictor& predictor;

    std::vector< Plugin* > plugins;

    std::string PLUGIN_PATH;
    std::string PLUGIN_SUFFIX;

};

#endif // SOOTH_PLUGINMANAGER
