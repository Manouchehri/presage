
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

#ifndef PRESAGE_DEJAVUPLUGIN
#define PRESAGE_DEJAVUPLUGIN

#include "plugins/plugin.h"

#include <list>
#include <string>
#include <fstream>

#include "core/dispatcher.h"

/** Dejavu plugin learns and reproduces previously seen text tokens, once its memory is triggered by a known token sequence.
 *   
 * Dejavu plugin is able to learn and then later reproduce previously
 * seen text sequences. The goal is dejavu plugin is to write a simple
 * plugin that demonstrates how learning can be implemented in the
 * presage system and exposes what functionality is required for
 * learning plugins to work within the presage framework.
 *
 */
class DejavuPlugin : public Plugin, public Observer {
public:
    DejavuPlugin(Configuration*, ContextTracker*);
    ~DejavuPlugin();

    virtual Prediction predict(const size_t size, const char** filter) const;

    virtual void learn(const std::vector<std::string>& change);
    virtual void extract();
    virtual void train();

    virtual void update (const Observable* variable);

private:
    typedef void (DejavuPlugin::* mbr_func_ptr_t) (const std::string& value);
    std::map<std::string, mbr_func_ptr_t> dispatch_map;

    bool init_memory_trigger(std::list<std::string>&) const;
    bool match(const std::list<std::string>&, const std::list<std::string>&) const;
    bool init_rolling_window(std::list<std::string>&, std::ifstream&) const;
    void update_rolling_window(std::list<std::string>&, const std::string&) const;

    void set_memory  (const std::string& filename);
    void set_trigger (const std::string& number);

    static const char* LOGGER;
    static const char* MEMORY;
    static const char* TRIGGER;

    std::string memory;
    int trigger;

    Dispatcher<DejavuPlugin> dispatcher;
};

#endif // SOOTH_DEJAVUPLUGIN
