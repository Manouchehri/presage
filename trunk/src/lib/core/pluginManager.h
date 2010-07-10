
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


#ifndef PRESAGE_PLUGINMANAGER
#define PRESAGE_PLUGINMANAGER


#include "core/predictor.h"
#include "plugins/plugin.h"

#include <vector>
#include <string>

#include <stdio.h>           // needed for directory and files manipulation
#include <sys/types.h>       // 

#if HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namlen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

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

#endif // PRESAGE_PLUGINMANAGER
