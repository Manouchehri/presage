
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


#ifndef PRESAGE_PLUGINREGISTRY
#define PRESAGE_PLUGINREGISTRY


#include "plugins/plugin.h"

class ContextTracker;

/** Manages instantiation and iteration through predictive plugins and aids in generating predictions and learning.
 *
 * PluginRegitry class holds the active plugins and provides the
 * interface required to obtain an iterator to the predictive plugins.
 * 
 * The standard use case is: Predictor obtains an iterator from
 * PluginRegistry and invokes the predict() or learn() method on each
 * Plugin pointed to by the iterator.
 * 
 * Plugin registry should eventually just be a simple wrapper around
 * plump.
 * 
 */
class PluginRegistry {
  public:
    PluginRegistry(Configuration* config);
    ~PluginRegistry();

    class Iterator {
    public:
	Iterator(std::vector<Plugin*>&);
	~Iterator();

	bool hasNext() const;
	Plugin* next();
	
    private:
	std::vector<Plugin*>::iterator iter_end;
	std::vector<Plugin*>::iterator iter_curr;

    };

    Iterator iterator();
    void setContextTracker(ContextTracker* ct);

    class PluginRegistryException : public PresageException {
    public:
	PluginRegistryException(const std::string& desc) throw() : PresageException(desc) { }
	virtual ~PluginRegistryException() throw() { }

    private:
	PluginRegistryException() throw() : PresageException("") { }

    };

  private:
    void setPlugins(const std::string& plugin_list);
    void addPlugin(const std::string& plugin_name);
    void removePlugins();

    Configuration*  config;
    ContextTracker* contextTracker;
    Logger<char>    logger;

    std::string          plugins_list;
    std::vector<Plugin*> plugins;        // active Plugins

    static const Variable LOGGER;
    static const Variable PLUGINS;
};

#endif // PRESAGE_PLUGINREGISTRY
