
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

const Variable PluginRegistry::LOGGER  = Variable("Presage.PluginRegistry.LOGGER");
const Variable PluginRegistry::PLUGINS = Variable("Presage.PluginRegistry.PLUGINS");

PluginRegistry::PluginRegistry(Configuration* configuration)
    : config(configuration),
      contextTracker(0),
      logger("PluginRegistry", std::cerr)
{
    // read config values
    Value value;

    try {
	value = config->get(LOGGER);
	logger << setlevel(value);
	logger << INFO << "LOGGER: " << value << endl;
    } catch (Configuration::ConfigurationException ex) {
	logger << WARN << "Caught ConfigurationException: " << ex.what() << endl;
    }

    try {
	value = config->get(PLUGINS);
	logger << INFO << "PLUGINS: " << value << endl;
	plugins_list = value;
    } catch (Configuration::ConfigurationException ex) {
	logger << ERROR << "Caught ConfigurationException: " << ex.what() << endl;
	throw PresageException("");
    }

}


PluginRegistry::~PluginRegistry()
{
    removePlugins();
}

void PluginRegistry::setContextTracker(ContextTracker* ct) {
    if (contextTracker != ct) {
	contextTracker = ct;
	removePlugins();
	setPlugins(plugins_list);
    }
}

void PluginRegistry::setPlugins(const std::string& pluginList)
{
    std::stringstream ss(pluginList);
    std::string pluginName;
    while (ss >> pluginName) {
	logger << INFO << "Initializing predictive plugin: " << pluginName << endl;
	addPlugin(pluginName);
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
    }
    plugins.clear();
}

PluginRegistry::Iterator PluginRegistry::iterator()
{
    // lazily initialize plugins
    if (contextTracker) {
	// plugins need tracker, only initialize them if available

	if (plugins_list.empty()) {
	    // plugins not init'd yet
	    plugins_list = config->get(PLUGINS);
	    setPlugins(plugins_list);

	} else {
	    // plugins init'd before, check whether list changed
	    std::string new_plugins_list = config->get(PLUGINS);
	    if (plugins_list != new_plugins_list) {
		removePlugins();
		plugins_list = new_plugins_list;
		setPlugins(plugins_list);
	    }

	}
    }

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
