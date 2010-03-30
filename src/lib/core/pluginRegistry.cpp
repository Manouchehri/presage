
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


#include "core/pluginRegistry.h"

// REVISIT: including plugins here because Plump is temporarily
// disabled. Plugin lifetime management is currently disabled.
//
// IMPORTANT: remove following includes when moving back to dynamically
// loaded plugins.
//
#include "plugins/smoothedNgramPlugin.h"
#include "plugins/ARPAPlugin.h"
#include "plugins/abbreviationExpansionPlugin.h"
#include "plugins/dummyPlugin.h"
#include "plugins/dictionaryPlugin.h"
#include "plugins/smoothedCountPlugin.h"
#include "plugins/recencyPlugin.h"
#include "plugins/dejavuPlugin.h"

const char* PluginRegistry::LOGGER  = "Presage.PluginRegistry.LOGGER";
const char* PluginRegistry::PLUGINS = "Presage.PluginRegistry.PLUGINS";

PluginRegistry::PluginRegistry(Configuration* configuration)
    : config(configuration),
      contextTracker(0),
      logger("PluginRegistry", std::cerr),
      dispatcher(this)
{
    // build notification dispatch map
    dispatcher.map (config->find (LOGGER), & PluginRegistry::setLogger);
    dispatcher.map (config->find (PLUGINS), & PluginRegistry::setPlugins);
}


PluginRegistry::~PluginRegistry()
{
    removePlugins();
}

void PluginRegistry::setLogger (const std::string& value)
{
    logger << setlevel (value);
    logger << INFO << "LOGGER: " << value << endl;
}


void PluginRegistry::setContextTracker(ContextTracker* ct) {
    if (contextTracker != ct) {
	contextTracker = ct;
	removePlugins ();
	setPlugins (plugins_list);
    }
}

void PluginRegistry::setPlugins(const std::string& pluginList)
{
    plugins_list = pluginList;
    logger << INFO << "PLUGINS: " << plugins_list << endl;

    if (contextTracker) {
	// plugins need tracker, only initialize them if available

        removePlugins();

        std::stringstream ss(plugins_list);
        std::string plugin;
	while (ss >> plugin) {
	    logger << INFO << "Initializing predictive plugin: " << plugin << endl;
	    addPlugin(plugin);
	}
    }
}

void PluginRegistry::addPlugin(const std::string& pluginName)
{
    Plugin* plugin = 0;
    try {
	// TODO: this will have to do for now, until a proper plugin
	// framework (i.e. plump) is integrated into presage. Until
	// then, all known plugins have to be listed here and explicitly
	// created based on their name.
	//
	if (pluginName == "SmoothedNgramPlugin") {
	    plugin = new SmoothedNgramPlugin(config, contextTracker);
	} else if (pluginName == "AbbreviationExpansionPlugin") {
	    plugin = new AbbreviationExpansionPlugin(config, contextTracker);
	} else if (pluginName == "DummyPlugin") {
	    plugin = new DummyPlugin(config, contextTracker);
	} else if (pluginName == "DictionaryPlugin" ) {
	    plugin = new DictionaryPlugin(config, contextTracker);
	} else if (pluginName == "SmoothedCountPlugin") {
	    plugin = new SmoothedCountPlugin(config, contextTracker);
	} else if (pluginName == "RecencyPlugin") {
	    plugin = new RecencyPlugin(config, contextTracker);
	} else if (pluginName == "DejavuPlugin") {
	    plugin = new DejavuPlugin(config, contextTracker);
	} else if (pluginName == "ARPAPlugin") {
	      plugin = new ARPAPlugin(config,contextTracker);
	}
    } catch (PresageException ex) {
	logger << ERROR << "Plugin " + pluginName + " failed to initialize." << endl;
    }

    if (plugin != 0) {
	plugins.push_back (plugin);
	logger << INFO << "Activated predictive plugin: " << pluginName << endl;
    } else {
	// TODO: raise exception
	logger << FATAL << "Unable to initialize plugin: " << pluginName << endl;
	throw PluginRegistryException("Unable to initialize plugin: " + pluginName);
    }
}

void PluginRegistry::removePlugins()
{
    for (size_t i = 0; i < plugins.size(); i++) {
	delete plugins[i];
	logger << DEBUG << "Removing plugin: " << plugins[i]->getName() << endl;
    }
    plugins.clear();
}

PluginRegistry::Iterator PluginRegistry::iterator()
{
    return Iterator(plugins);
}


////
// Iterator
PluginRegistry::Iterator::Iterator(std::vector<Plugin*>& cont)
    : iter_end(cont.end()),
      iter_curr(cont.begin())
{}

PluginRegistry::Iterator::~Iterator()
{}

bool PluginRegistry::Iterator::hasNext() const
{
    bool result = (iter_end != iter_curr);

    return result;
}

Plugin* PluginRegistry::Iterator::next()
{
    Plugin* result = *iter_curr;
    iter_curr++;
    return result;
}

void PluginRegistry::update (const Observable* variable)
{
    Variable* var = (Variable*) variable;
    
    logger << DEBUG << "About to invoke dispatcher: " << var->get_name () << " - " << var->get_value() << endl;

    dispatcher.dispatch (var);
}
